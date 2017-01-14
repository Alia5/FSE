#pragma once

#include <functional>
#include <utility>
#include <map>
#include <vector>
#include <list>



template <typename Event>
class EventBus
{
public:

	typedef std::function<void()> Func;
	typedef std::list<Func>::iterator EventCookie;
	//template <typename Observer>
	//EventCookie subscribeEvent(const Event& evnt, Observer&& observer)
	//{
	//	observers_[evnt].push_back(std::forward<Observer>(observer));
	//	auto it = observers_[evnt].end();
	//	--it;
	//	return it;
	//}


	EventCookie subscribeEvent(const Event& evnt, Func&& observer)
	{
		observers_[evnt].push_back(std::forward<Func>(observer));
		auto it = observers_[evnt].end();
		--it;
		return it;
	}

	void unsubEvent(const Event& evnt, EventCookie it)
	{
		observers_.at(evnt).erase(it);
	}

	void sendEvent(const Event& event) const
	{
		for (const auto & obs : observers_.at(event))
		{
			obs();
		}
		
	}

private:	
	std::map <Event, std::list<std::function<void()>>> observers_;

};


template <typename Event, typename... Args>
class EventBus2
{
public:

	typedef std::function<void(Args...)> Func;
	//typedef std::list<Func>::iterator EventCookie;
	//template <typename Observer>
	//EventCookie subscribeEvent(const Event& evnt, Observer&& observer)
	//{
	//	observers_[evnt].push_back(std::forward<Observer>(observer));
	//	auto it = observers_[evnt].end();
	//	--it;
	//	return it;
	//}


	void subscribeEvent(const Event& evnt, Func&& observer)
	{
		observers_[evnt].push_back(std::forward<Func>(observer));
		//auto it = observers_[evnt].end();
		//--it;
		//return it;
	}

	//void unsubEvent(const Event& evnt, EventCookie it)
	//{
	//	observers_.at(evnt).erase(it);
	//}

	void sendEvent(const Event& event, Args... args) const
	{
		for (const auto & obs : observers_.at(event))
		{
			obs(args...);
		}

	}

private:
	std::map <Event, std::list<Func>> observers_;

};





template <typename Ret, typename... Args>
class RetSignal
{

	typedef std::function<Ret(Args...)> Slot;

public:

	//template <typename sSlot>
	void Connect(Slot&& slot)
	{
		slots_.push_back(std::forward<Slot>(slot));
	}

	template<class O, typename R, typename ... A>
	void Connect(O* o, R(O::*f)(A...))
	{
		slots_.push_back((std::forward<Slot>([=](A... args) { return (o->*f)(args...); })));
	}

	//template<typename O, typename R, typename... A>
	//void Stuff(O* o, R(O::*f)(A...))
	//{
	//	Connect(std::bind(f, o, std::placeholders<A>));
	//}


	std::vector<Ret> operator()(Args... args) const
	{

		std::vector<Ret> ret;
		for (const auto & slot : slots_)
		{
			 ret.push_back(slot(args...));
		}

		return ret;
	}

private:
	std::vector<Slot> slots_;
};






//template <typename... Args>
//class Signal
//{
//
//	typedef std::function<void(Args...)> Slot;
//
//public:
//
//	typedef std::shared_ptr<ConnectionItem> Connection;
//
//	void Connect(Slot&& slot)
//	{
//		slots_.push_back(std::forward<Slot>(slot));
//	}
//
//	//Bindign with placeholders is tedious... 
//	//Don't want i.e. this: sig.Connect(std::bind(&DummyObject::test, this, std::placeholders::_1, std::placeholders::_2));
//	template<class O, typename R, typename ... A>
//	void Connect(O* o, R(O::*f)(A...))
//	{
//		slots_.push_back((std::forward<Slot>([=](A... args) { return (o->*f)(args...); })));
//	}
//
//	void operator()(Args... args) const
//	{
//		for (const auto & slot : slots_)
//		{
//			slot(args...);
//		}
//
//	}
//
//private:
//	std::list<Slot> slots_;
//};


//template <typename... Args>
//class Signal
//{
//public:
//
//	typedef std::function<void(Args...)> Func;
//
//	//void subscribe(Func&& f)
//	//{
//	//	funs_.push_back(std::forward<Func>(f));
//	//}
//
//	void subscribe(Func f)
//	{
//		funs_.push_back(f);
//	}
//	//TODO:: HANDLE UNSUB
//
//	void send(Args... args)
//	{
//		for (auto & fun : funs_)
//		{
//			fun(args...);
//		}
//	}
//
//private:
//
//	std::list<Func> funs_;
//
//};
//
//
//
//
//template <typename... Args>
//class Stuff
//{
//	Stuff(Args... args)
//	{
//
//	}
//};
//
//
//
//template <typename Ev, typename... Args>
//class SignalBus
//{
//public:
//
//	//void registerSignal(const Signal<Args...>& signal)
//	//{
//	//	//signals[signal]
//	//}
//
//	//void subscribeSignal(const Ev &evnt, const Signal<Args...>& signal, Func&& fun)
//	//{
//	//	signals.at(signal).subscribe(fun);
//	//}
//
//	template<typename... TArgs, typename SlotFun>
//	void connect(Signal<TArgs...>& sig, SlotFun&& slot)
//	{
//		sig.subscribe(std::forward<SlotFun>(slot));
//	}
//
//
//	typedef std::function<void(Args...)> Func;
//
//	void subscribe(const Ev& evnt, Func&& fun)
//	{
//		for (auto & sig : signals_.at(evnt))
//		{
//			sig->subscribe(std::forward<Func>(fun));
//		}
//	}
//
//	void registerSignal(const Ev& evnt, Signal<Args...>* sig)
//	{
//		signals_[evnt].push_back(sig);
//	}
//
//	//void send(const Ev& evnt, Args... args)
//	//{
//
//	//	for (auto & sig : signals_.at(evnt))
//	//	{
//	//		sig.send(args);
//	//	}
//	//}
//
//	
//	//void registerSignal(const Ev& evnt, Func&& f)
//	//{
//	//	
//	//	Signal<Args...> sig;
//	//	signals_[evnt].push_back(std::forward<Signal<Args...>>(sig));
//	//}
//
//
//private:
//	std::map<Ev, std::list<Signal<Args...>*>> signals_;
//};



//template <typename Event>
//class EventBus
//{
//public:
//
//	typedef std::list<std::function<void()>>::iterator EventCookie;
//	template <typename Observer>
//	void subscribeEvent(const Event& evnt, Observer&& observer)
//	{
//		observers_[evnt].push_back(std::forward<Observer>(observer));
//		auto it = observers_[evnt].end();
//		--it;
//		return it;
//	}
//
//	void unsubEvent(const Event& evnt, std::list<std::function<void()>>::iterator it)
//	{
//		observers_.at(evnt).erase(it);
//	}
//
//	void sendEvent(const Event& event, Args... args) const
//	{
//		for (const auto & obs : observers_.at(event))
//		{
//			obs(args...);
//		}
//		
//	}
//
//private:	
//	std::map <Event, std::list<std::function<void()>>> observers_;
//
//};