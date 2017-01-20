#ifndef SIGNALS_H
#define SIGNALS_H

#include <functional>
#include <list>
#include <memory>


#define PUBLIC_SIGNALS public

template <typename... Args>
class ConnectionItem
{
	using Slot = std::function<void(Args...)>;
public:
	ConnectionItem(Slot slot) : slot_(slot)
	{
		connected_ = true;
	}

	~ConnectionItem()
	{
		connected_ = false;
	}

	void operator()(Args... args)
	{
		if (connected_ && slot_)
			slot_(args...);
	}

	bool isConnected() const
	{
		return connected_;
	}

	void disconnect()
	{
		connected_ = false;
	}

private:

	bool connected_ = false;

	Slot slot_;
	

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

	~SConnection()
	{
		if (has_connection_ && signal_)
		{
			signal_->disconnect(connection_);
		}
	}

private:
	bool has_connection_ = false;
	Connection connection_;
	Signal<Args...> *signal_;
};

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

private:
	std::list<Item> items_;

	void removeDisconnected()
	{
		items_.erase(std::remove_if(items_.begin(), items_.end(), [](Item &item) {
			return !item->isConnected();
		}), items_.end());
	}
};

#endif // !SIGNALS_H