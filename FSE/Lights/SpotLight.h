#pragma once

#include "Light.h"

namespace fse
{
	class SpotLight : public Light
	{
	public:
		SpotLight();
		SpotLight(Scene* scene, const sf::Vector2f& spawnPos);
		~SpotLight();

		void setLenght(float lenght);
		float getLenght() const;

		void setAngle(float angle);
		float getAngle() const;

	private:
		float lenght_ = 1;
		float angle_ = 90;

	};
}
