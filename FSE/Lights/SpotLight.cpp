#include "SpotLight.h"
#include "../Application.h"


namespace fse
{
	SpotLight::SpotLight()
	{
	}

	SpotLight::SpotLight(Scene* scene, const sf::Vector2f& spawnPos) : Light(scene, spawnPos, "lights/spotLightTexture.png", true)
	{
		light_->setOrigin(sf::Vector2f(light_texture_->getSize().x / 2.f, 0.f));
		lenght_ = light_texture_->getSize().y * FSE_METERS_PER_PIXEL;
	}


	SpotLight::~SpotLight()
	{
	}

	void SpotLight::setLenght(float lenght)
	{
		lenght_ = lenght;
		light_->setScale(light_->getScale().x * lenght * 5.12f * FSE_METERS_PER_PIXEL, lenght * 5.12f * FSE_METERS_PER_PIXEL);
	}

	float SpotLight::getLenght() const
	{
		return lenght_;
	}

	void SpotLight::setAngle(float angle)
	{
		angle_ = std::min(angle, 179.9f);
		light_->setScale(light_->getScale().x * light_->getScale().y * std::tan((angle_ * 3.14159265f / 180.0f) / 2.f), light_->getScale().y);

	}

	float SpotLight::getAngle() const
	{
		return angle_;
	}
}
