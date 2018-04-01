#pragma once

#include "Light.h"

namespace fse
{
	/*!
	 * \brief Spotlight (light cone)
	 */
	class SpotLight : public Light
	{
	public:
		SpotLight();
		SpotLight(Scene* scene, const sf::Vector2f& spawnPos);
		~SpotLight();

		/*!
		 * set lenght of light cone (in meters)
		 * \param lenght lenght in meters
		 */
		void setLenght(float lenght);
		/*!
		 * \return lenght in meters
		 */
		float getLenght() const;

		/*!
		 * Set light cone angle
		 * \param angle angle in degrees
		 */
		void setAngle(float angle);
		/*!
		 * \return angle of lightcone in degrees
		 */
		float getAngle() const;

	private:
		float lenght_ = 1;
		float angle_ = 90;

		RTTR_ENABLE(fse::Light)
		RTTR_REGISTRATION_FRIEND

		friend class fse::priv::FSEChaiLib;
	};
}
