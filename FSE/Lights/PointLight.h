#pragma once

#include "BaseLight.h"

namespace fse
{
	class PointLight : public BaseLight
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
