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