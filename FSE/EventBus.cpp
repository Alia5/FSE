//
//#include "EventBus.h"
//
//
//template<typename Event>
//template<typename Observer>
//std::list<std::function<void()>>::iterator EventBus<Event>::subscribeEvent(const Event & evnt, Observer && observer)
//{
//	return std::list<std::function<void()>>::iterator();
//}
//
//template<typename Event>
//template<typename Observer>
//std::list<std::function<void()>>::iterator EventBus<Event>::subscribeEvent(Event && evnt, Observer && observer)
//{
//	obsersvers_[std::move(evnt)].push_back(std::forward<Observer>(observer));
//	auto it = observers_[evnt].end() - 1;
//	return it;
//}
//
