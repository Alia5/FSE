#pragma once

#include "Light.h"

namespace fse
{
	/*!
	 * \brief Simple point light
	 */
	class PointLight : public Light
	{
	public:
		PointLight();
		PointLight(Scene* scene, const sf::Vector2f& spawnPos);
		~PointLight();

		/*!
		 * Set radius of light (in meters)
		 * \param radius radius in meters
		 */
		void setRadius(float radius) const;
		/*!
		 * \return radius of light in meters
		 */
		float getRadius() const;

	private:
		RTTR_ENABLE(fse::Light)
		RTTR_REGISTRATION_FRIEND

		FSE_CHAI_ENABLE(PointLight);
		FSE_CHAI_REGISTRATION_FRIEND

	};
}
