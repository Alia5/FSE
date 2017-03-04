#pragma once

#include <SFML/System.hpp>

#include "../Signals.h"
#include "FSEObject.h"

namespace fse
{
	/*!
	 * \brief Prebuilt Timer class
	 * Timer is not accurate for time measuring! \n 
	 * Emits signal once timer runs out (mid frame)
	 */
	class Timer : public FSEObject
	{
	public:
		Timer(Scene* scene);
		~Timer();

		void update(float deltaTime) override;

		void draw(sf::RenderTarget& target) override;

		void spawned() override;

		/*!
		 * Connects a slot to the timer and starts it
		 * \param slot Slot
		 */
		template <typename Slot>
		void start(Slot&& slot)
		{
			active_ = true;
			timeout_.connect(slot);
		}

		/*!
		 * Stops the timer
		 */
		void stop();
		/*!
		 * Sets interval of timer
		 * \param msecs interval (in millis)
		 */
		void setInterval(int msecs);

		/*!
		 * \return is timer active
		 */
		bool isActive() const;
		
		/*!
		 * Set timer to fire **once**
		 * \param singleShot set timer single shot
		 */
		void setSingleShot(bool singleShot);

		/*!
		 * \brief Create and start a single shot timer
		 * \param scene Ptr to scene
		 * \param msecs Timeout in millis
		 * \param slot Slot  
		 */
		template <typename Slot>
		static void singleShot(Scene* scene,int msecs, Slot&& slot)
		{
			auto timer = std::make_unique<Timer>(scene);
			timer->setInterval(msecs);
			timer->setSingleShot(true);

			timer->start(slot);

			scene->spawnFSEObject(std::move(timer));
		}

	private:	

		bool active_ = false;
		bool single_shot_ = false;
		int interval_ = 0;
		float elapsed_time_ = 0.f;
	


	PUBLIC_SIGNALS:
		Signal<> timeout_;

	};
}