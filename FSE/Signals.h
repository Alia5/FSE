#ifndef SIGNALS_H
#define SIGNALS_H

#include <functional>
#include <list>
#include <memory>

#include "FSEV8Lib.h"

#define PUBLIC_SIGNALS public


template <typename... Args>
class ConnectionItem
{
    using Slot = std::function<void(Args...)>;
public:
    explicit ConnectionItem(Slot slot) : slot_(slot)
    {
        connected_ = true;
    }

    explicit ConnectionItem(
       v8::Local<v8::Function>& js_cb
    ) : slot_(nullptr)
    {
        js_cb_.Reset(v8::Isolate::GetCurrent(),js_cb);
        connected_ = true;
    }

    ~ConnectionItem()
    {
        disconnect();
    }

    void operator()(Args... args)
    {
        if (connected_ && slot_)
        {
            slot_(args...);
            return;
        }
        if (connected_ && !js_cb_.IsEmpty())
        {
            const auto iso = v8::Isolate::GetCurrent();
            v8::HandleScope handle_scope(iso);
            if (sizeof...(args) == 0)
            {
                js_cb_.Get(iso)->Call(
                    iso->GetCurrentContext(),
                    v8::Undefined(iso),
                    0,
                    nullptr
                );
                return;
            }
            std::vector<v8::Local<v8::Value>> argv;
            argv.reserve(sizeof...(args));
            (argv.push_back(v8pp::to_v8(iso, args)), ...);

            js_cb_.Get(iso)->Call(
                iso->GetCurrentContext(),
                v8::Undefined(iso),
                sizeof...(args),
                argv.data()
            );
        }
    }

    bool isJs() const
    {
        return !js_cb_.IsEmpty();
    }

    bool isConnected() const
    {
        return connected_;
    }

    void disconnect()
    {
        connected_ = false;
        js_cb_.Reset();
    }

private:

    bool connected_ = false;
    Slot slot_;
    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> js_cb_;
};

template<typename... Args>
class Signal;

template<typename... Args>
class SConnection;

template<typename... Args>
class Connection
{
    using Item = std::shared_ptr<ConnectionItem<Args...>>;
public:

    Connection() : item_(nullptr)
    {

    }

    Connection(Item item) : item_(item)
    {

    }

    ~Connection()
    {

    }

    bool isConnected() const
    {
        if (item_ == nullptr)
            return false;
        return item_->isConnected();
    }

private:
    Item item_;

    void disconnect()
    {
        item_->disconnect();
    }

    bool operator==(const Item &item)
    {
        return item == item_;
    }

    friend class Signal<Args...>;
    friend class SConnection<Args...>;

    static void v8_init_func(fse::Application* app, v8::Isolate* isolate, v8pp::module& module)
    {
        v8::HandleScope handle_scope(isolate);
        v8pp::class_<Connection<Args...>> ConnectionClass(isolate);
        ConnectionClass.auto_wrap_objects();
        ConnectionClass.function("isConnected", &Connection<Args...>::isConnected);
        module.class_(typeid(Connection<Args...>).name(), ConnectionClass);
    }
    inline static v8_init_helper init_helper = v8_init_helper(&Connection<Args...>::v8_init_func);

};


template<typename... Args>
class SConnection
{
    using Connection = Connection<Args...>;

public:

    SConnection() : signal_(nullptr)
    {

    }

    SConnection(Signal<Args...>& signal, Connection connection) : connection_(connection), signal_(&signal)
    {
        has_connection_ = true;
    }

    SConnection(SConnection&& other) noexcept
        : has_connection_(std::exchange(other.has_connection_, nullptr)),
        signal_(std::exchange(other.signal_, nullptr)),
        connection_(std::exchange(other.connection_, nullptr))
    {}


    ~SConnection()
    {
        if (has_connection_ && signal_)
        {
            signal_->disconnect(connection_);
        }
    }

    SConnection& operator=(SConnection&& other)
    {
        if (this != &other)
        {
            std::swap(has_connection_, other.has_connection_);
            std::swap(signal_, other.signal_);
            std::swap(connection_, other.connection_);
            return *this;
        }
        return *this;
    }

private:
    bool has_connection_ = false;
    Connection connection_;
    Signal<Args...> *signal_;
};

/*!
 * \brief Signal class used for event handling \n
 * 
 * 
 * 
 * Usage \n
 *		\code
 *		//Emitter-Side 
 *		
 *		//Define and declare signal and arguments
 *		PUBLIC_SIGNALS: 
 *		using ExampleSignal = Signal<float, int>; 
 *		ExampleSignal sig;
 *		
 *		//Call the signal
 *		void stuff() 
 *		{
 *			sig(1.3f, 4);
 *		}
 *		
 *		////////////////////
 *		
 *		//Slot side: 
 *		
 *		//A slot can be any function or functor
 *		//free function pointers, member functions, lambdas, or std::function
 *		
 *		auto conn = sig.connect(&function);
 *		
 *		//or using lambdas
 *		auto conn = sig.connect([](float a, int b){ 
 *			//Code here 
 *		});
 *		
 *		//or using scoped connections with function ptrs or lambdas
 *		auto conn = Signal::ScopedConnection(sig, sig.connect(&function));
 *		
 *		//member functions:
 *		sig.connect(this, &ClassName::functionName);
 *		
 *		//or using std::bind
 *		sig.Connect(std::bind(&DummyObject::functionName, this, std::placeholders::_1, std::placeholders::_2));
 *		
 *		
 *		\endcode
 */
template <typename... Args>
class Signal
{
    using Item = std::shared_ptr<ConnectionItem<Args...>>;

public:

    using Slot = std::function<void(Args...)>;
    using Connection = Connection<Args...>;
    using ScopedConnection = SConnection<Args...>;

    Signal()
    {

    }

    ~Signal()
    {
        disconnectAll();
    }

    template <typename SlotF>
    Connection connect(SlotF&& slot)
    {
        Item item = std::make_shared<ConnectionItem<Args...>>(std::forward<SlotF>(slot));
        items_.push_back(item);
        return Connection(item);
    }

    //Binding with placeholders is tedious :poop:... 
    //Don't want i.e. this: sig.Connect(std::bind(&DummyObject::test, this, std::placeholders::_1, std::placeholders::_2));
    template<class O, typename R, typename ... A>
    Connection connect(O* o, R(O::*f)(A...))
    {
        Item item = std::make_shared<ConnectionItem<Args...>>(std::forward<Slot>([=](A... args) { return (o->*f)(args...); }));
        items_.push_back(item);
        return Connection(item);
    }

    void operator()(Args... args)
    {
        for (const auto & item : items_)
        {
            (*item)(args...);
        }
    }

    bool disconnect(Connection &connection)
    {

        for (const auto & item : items_)
        {
            if (connection == item)
            {
                connection.disconnect();
                removeDisconnected();
                return true;
            }

        }
        return false;
    }


    void disconnectAll()
    {
        for (const auto & item : items_)
        {
            item->disconnect();
        }
        items_.clear();
    }

    void disconnectAllJs()
    {
        for (const auto& item : items_)
        {
            if (item->isJs())
                item->disconnect();
        }
        removeDisconnected();
    }

    Connection connectJs(v8::Local<v8::Function>& slot)
    {
        Item item = std::make_shared<ConnectionItem<Args...>>(slot);
        items_.push_back(item);
        return Connection(item);
    }

private:
    std::list<Item> items_;

    void removeDisconnected()
    {
        items_.erase(std::remove_if(items_.begin(), items_.end(), [](Item &item) {
            return !item->isConnected();
        }), items_.end());
    }

    FSE_V8_REGISTRATION_FRIEND;
    static void v8_init_func(fse::Application* app, v8::Isolate* isolate, v8pp::module& module)
    {
        v8::HandleScope handle_scope(isolate);
        v8pp::class_<Signal<Args...>> SignalClass(isolate);
        SignalClass.auto_wrap_objects();
        SignalClass.function("connect", [](v8::FunctionCallbackInfo<v8::Value> const& v8_args)
        {
            const auto iso = v8_args.GetIsolate();
            auto This = v8pp::from_v8<Signal<Args...>*>(iso, v8_args.This());
            v8::EscapableHandleScope scope(iso);
            auto js_func = v8_args[0].As<v8::Function>();
            auto connection = This->connectJs(js_func);
            return scope.Escape(v8pp::to_v8(iso, connection));
        });
        SignalClass.function("disconnect", &Signal<Args...>::disconnect);
        SignalClass.function("disconnectAll", &Signal<Args...>::disconnect);
        module.class_(typeid(Signal<Args...>).name(), SignalClass);
    }
    inline static v8_init_helper init_helper = v8_init_helper(&Signal<Args...>::v8_init_func);
};

#endif // !SIGNALS_H