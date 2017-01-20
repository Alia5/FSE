#include "PointLight.h"
#include "../Application.h"


namespace fse
{
	PointLight::PointLight() : Light()
	{
		
	}

	PointLight::PointLight(Scene* scene, const sf::Vector2f& spawnPos) : Light(scene, spawnPos, "lights/pointLightTexture.png", true)
	{
		light_->setScale((FSE_PIXELS_PER_METER / light_texture_->getSize().x), (FSE_PIXELS_PER_METER / light_texture_->getSize().x));
	}

	PointLight::~PointLight()
	{
		
	}

	void PointLight::setRadius(float radius) const
	{
		light_->setScale(sf::Vector2f(radius,radius) * (FSE_PIXELS_PER_METER / light_texture_->getSize().x));
	}

	float PointLight::getRadius() const
	{
		return light_->getScale().x / (FSE_PIXELS_PER_METER / light_texture_->getSize().x);
	}

}
