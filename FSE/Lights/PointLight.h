#pragma once

#include "Light.h"

namespace fse
{
	class PointLight : public Light
	{
	public:
		PointLight();
		PointLight(Scene* scene, const sf::Vector2f& spawnPos);
		~PointLight();

		void setRadius(float radius) const;
		float getRadius() const;

	private:


	};
}
