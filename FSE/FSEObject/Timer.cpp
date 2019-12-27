#include "Timer.h"

#include "../Application.h"

namespace fse
{
	Timer::Timer() : FSEObject()
	{

	}

	Timer::~Timer()
	{
		persistent_callback_.Reset();
		persistent_this_.Reset();
	}

	void Timer::update(float deltaTime)
	{
		if (active_)
		{
			elapsed_time_ += deltaTime;
			while (elapsed_time_ * 1000 >= interval_)
			{
				elapsed_time_ -= interval_*0.001f;
				timeout_();
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
				object->persistent_callback_.Reset(isolate, args[0].As<v8::Function>());
				object->persistent_this_.Reset(isolate, args.This());
				object->start([object, isolate]()
				{
						v8::HandleScope handle_scope(isolate);
						object->persistent_callback_.Get(isolate)->Call(isolate->GetCurrentContext(), object->persistent_this_.Get(isolate), 0, nullptr);
				});
			});
		
		Timer_class.function("startSingleShot", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::Isolate* isolate = args.GetIsolate();
				v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> persistentCallback(isolate, args[2].As<v8::Function>());
				v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentThis(isolate, args.This());
				return Timer::singleShot(v8pp::from_v8<Scene*>(isolate, args[0]),
					v8pp::from_v8<int>(isolate, args[1]),
					[persistentCallback, persistentThis, isolate]()
				{
						v8::HandleScope handle_scope(isolate);
						persistentCallback.Get(isolate)->Call(isolate->GetCurrentContext(), persistentThis.Get(isolate),  0, nullptr);
						const_cast<v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>&>(persistentCallback).Reset();
						const_cast<v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>&>(persistentThis).Reset();
				});
			});
		isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
			v8::String::NewFromUtf8(isolate, "setTimeout").ToLocalChecked(),
			v8pp::wrap_function(isolate, "__setTimeout",[app](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
					v8::Isolate* isolate = args.GetIsolate();
					v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> persistentCallback(isolate, args[0].As<v8::Function>());
					v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentThis(isolate, args.This());
					return Timer::singleShot(&app->getRootScene(),
						v8pp::from_v8<int>(isolate, args[1]),
						[persistentCallback, persistentThis, isolate]()
						{
							v8::HandleScope handle_scope(isolate);
							persistentCallback.Get(isolate)->Call(isolate->GetCurrentContext(), persistentThis.Get(isolate), 0, nullptr);
							const_cast<v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>&>(persistentCallback).Reset();
							const_cast<v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>&>(persistentThis).Reset();
						});
		}));
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
