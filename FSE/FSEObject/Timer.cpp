#include "Timer.h"

namespace fse
{
	Timer::Timer(Scene* scene) : FSEObject(scene)
	{

	}

	Timer::~Timer()
	{
		std::wcout << "destroying timer with id: " << getID() << "\n";
	}

	void Timer::update(float deltaTime)
	{
		if (mActive)
		{
			mElapsedTime += deltaTime;
			while (mElapsedTime * 1000 >= mInterval)
			{
				mElapsedTime -= mInterval*0.001f;
				timeout();
				if (mSingleShot)
				{
					mActive = false;
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

	//void Timer::start()
	//{
	//	mActive = true;

	//}

	void Timer::stop()
	{
		mActive = false;
	}

	void Timer::setInterval(int msecs)
	{
		mInterval = msecs;
	}

	bool Timer::isActive()
	{
		return false;
	}

	void Timer::setSingleShot(bool singleShot)
	{
		mSingleShot = singleShot;
	}

}