#pragma once

#include <SFML/System.hpp>

#include "../Signals.h"
#include "FSEObject.h"

namespace fse
{
	class Timer : public FSEObject
	{
	public:
		Timer(Scene* scene);
		~Timer();

		virtual void update(float deltaTime) override;

		virtual void draw(sf::RenderTarget& target) override;

		virtual void spawned() override;

		template <typename Slot>
		void start(Slot&& slot)
		{
			mActive = true;
			timeout.connect(slot);
		}

		void stop();
		void setInterval(int msecs);

		bool isActive();
		
		void setSingleShot(bool singleShot);

		template <typename Slot>
		static void singleShot(Scene* scene,int msecs, Slot&& slot)
		{
			auto timer = FSEObject::createObject<Timer>(scene);
			timer->setInterval(msecs);
			timer->setSingleShot(true);

			timer->start(slot);

			scene->spawnFSEObject(std::move(timer));
		}

	private:

		bool mActive = false;
		bool mSingleShot = false;
		int mInterval = 0;
		float mElapsedTime = 0.f;
	


	PUBLIC_SIGNALS:
		Signal<> timeout;

	};
}