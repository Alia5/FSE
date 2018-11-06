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
				//since chaiscript functions can be run, we have to catch chai exceptions
				try
				{
					timeout_();
				} catch(chaiscript::exception::eval_error& e) {
					const std::string evalString = "puts(\"" + std::regex_replace(e.pretty_print(), std::regex("(\")"), "\\\"") + "\");";
					scene_->getApplication()->getChai()->eval(evalString);
				} catch (std::exception& e) {
					const std::string evalString = "puts(\"" + std::regex_replace(e.what(), std::regex("(\")"), "\\\"") + "\");";
					scene_->getApplication()->getChai()->eval(evalString);
				} catch (...) {
					
				}
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
