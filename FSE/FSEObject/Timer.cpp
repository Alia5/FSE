#include "Timer.h"

#include "../Application.h"
#include <regex>

namespace fse
{
	Timer::Timer() : FSEObject()
	{

	}

	Timer::~Timer()
	{
		
	}

	void Timer::update(float deltaTime)
	{
		if (active_)
		{
			elapsed_time_ += deltaTime;
			while (elapsed_time_ * 1000 >= interval_)
			{
				elapsed_time_ -= interval_*0.001f;
				////since chaiscript functions can be run, we have to catch chai exceptions
				//try
				//{
				//	timeout_();
				//} catch(chaiscript::exception::eval_error& e) {
				//	const std::string evalString = "puts(\"" + std::regex_replace(e.pretty_print(), std::regex("(\")"), "\\\"") + "\");";
				//	scene_->getApplication()->getChai()->eval(evalString);
				//} catch (std::exception& e) {
				//	const std::string evalString = "puts(\"" + std::regex_replace(e.what(), std::regex("(\")"), "\\\"") + "\");";
				//	scene_->getApplication()->getChai()->eval(evalString);
				//} catch (...) {
				//	
				//}
				if (single_shot_)
				{
					active_ = false;
					getScene()->destroyFSEObject(this);
					break;
				}
			}


		}
	}

	void Timer::draw(sf::RenderTarget & target)
	{
	}

	void Timer::spawned()
	{
	}

	void Timer::onDespawn()
	{
	}

	void Timer::stop()
	{
		active_ = false;
	}

	void Timer::setInterval(int msecs)
	{
		interval_ = msecs;
	}

	bool Timer::isActive() const
	{
		return active_;
	}

	void Timer::setSingleShot(bool singleShot)
	{
		single_shot_ = singleShot;
	}

	FSE_V8_REGISTER(Timer)
	{

		v8::HandleScope handle_scope(isolate);
		v8pp::class_<Timer, v8pp::shared_ptr_traits>Timer_class(isolate);
		Timer_class.inherit<FSEObject>();
		fse::addV8DownCastHelper<fse::FSEObject, fse::Timer>();
		Timer_class.auto_wrap_objects(true);
		Timer_class.ctor<void>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return std::make_shared<Timer>();
			});
		Timer_class.var("active", &Timer::active_);
		Timer_class.function("isActive", &Timer::isActive);
		Timer_class.var("singleShot", &Timer::single_shot_);
		Timer_class.function("setSingleShot", &Timer::setSingleShot);
		Timer_class.var("interval", &Timer::interval_);
		Timer_class.function("setInterval", &Timer::setInterval);
		Timer_class.function("stop", &Timer::stop);
		Timer_class.function("start", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::Isolate* isolate = args.GetIsolate();
				auto object = v8pp::from_v8<std::shared_ptr<Timer>>(isolate, args.This());
				object->start(v8pp::from_v8<std::function<void()>>(isolate, args[0]));
			});
		Timer_class.function("startSingleShot", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::Isolate* isolate = args.GetIsolate();
				return Timer::singleShot(v8pp::from_v8<Scene*>(isolate, args[0]),
					v8pp::from_v8<int>(isolate, args[1]),
					v8pp::from_v8<std::function<void()>>(isolate, args[2]));
			});
		module.class_("Timer", Timer_class);
		
		//RegisterJSUserTypeFromRTTR<Timer>(isolate);
		//chai.add(chaiscript::base_class<fse::FSEObject, Timer>());
		//chai.add(chaiscript::constructor<Timer()>(), "Timer");
		//chai.add(chaiscript::fun((&Timer::active_)), "active");
		//chai.add(chaiscript::fun((&Timer::single_shot_)), "singleShot");
		//chai.add(chaiscript::fun((&Timer::interval_)), "interval");
		//chai.add(chaiscript::fun(((&Timer::stop))), "stop");
		//chai.add(chaiscript::fun(([](Timer* timer, std::function<void()> slot) { timer->start(slot); })), "start");
		//chai.add(chaiscript::fun(([](Scene* scene, int msecs, std::function<void()> slot) { Timer::singleShot(scene, msecs, slot); })),
		//	"startSingleShotTimer");
	}
}

#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<fse::Timer>("fse::Timer")
	.constructor<>()
	(
		policy::ctor::as_std_shared_ptr
	)
	.property_readonly("active_", &fse::Timer::active_)
	.property("single_shot_", &fse::Timer::single_shot_)
	.property("interval_", &fse::Timer::interval_)
	//.method("start", [](fse::Timer* timer, auto slot) { timer->start(slot); })
	.method("stop", &fse::Timer::stop)
	;
}
