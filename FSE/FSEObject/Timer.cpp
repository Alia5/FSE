#include "Timer.h"

namespace fse
{
	Timer::Timer(Scene* scene) : FSEObject(scene)
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

}

#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<fse::Timer>("fse::Timer")
	.constructor<>([](fse::Scene* scene)
	{
		scene->createFSEObject<fse::Timer>();
		return nullptr;
	})
	(
		parameter_names("scene"),
		metadata("CHAI_CTOR", true)
	)
	.constructor<>([](fse::Scene* scene, std::function<void(fse::FSEObject*)> func)
	{
		scene->createFSEObject<fse::Timer>(func);
		return nullptr;
	})
	(
		parameter_names("scene", "spawnedSlot"),
		metadata("CHAI_CTOR", true)
	)
	.property_readonly("active_", &fse::Timer::active_)
	.property("single_shot_", &fse::Timer::single_shot_)
	.property("interval_", &fse::Timer::interval_)
	//.method("start", [](fse::Timer* timer, auto slot) { timer->start(slot); })
	.method("stop", &fse::Timer::stop)
	;
}
