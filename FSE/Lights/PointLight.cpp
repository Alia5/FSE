#include "PointLight.h"
#include "../Application.h"


namespace fse
{
	PointLight::PointLight() : BaseLight()
	{
		
	}

	PointLight::PointLight(Scene* scene, const sf::Vector2f& spawnPos) : BaseLight(scene, spawnPos, "lights/pointLightTexture.png", true)
	{
		m_light->setScale((FSE_PIXELS_PER_METER / m_LightTexture->getSize().x), (FSE_PIXELS_PER_METER / m_LightTexture->getSize().x));
	}

	PointLight::~PointLight()
	{
		
	}

	void PointLight::setRadius(float radius) const
	{
		m_light->setScale(sf::Vector2f(radius,radius) * (FSE_PIXELS_PER_METER / m_LightTexture->getSize().x));
	}

	float PointLight::getRadius() const
	{
		return m_light->getScale().x / (FSE_PIXELS_PER_METER / m_LightTexture->getSize().x);
	}

}
