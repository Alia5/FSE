#include "SpotLight.h"
#include "../Application.h"


namespace fse
{
	SpotLight::SpotLight()
	{
	}

	SpotLight::SpotLight(Scene* scene, const sf::Vector2f& spawnPos) : BaseLight(scene, spawnPos, "lights/spotLightTexture.png", true)
	{
		m_light->setOrigin(sf::Vector2f(m_LightTexture->getSize().x / 2, 0.f));
		m_lenght = m_LightTexture->getSize().y * FSE_METERS_PER_PIXEL;
	}


	SpotLight::~SpotLight()
	{
	}

	void SpotLight::setLenght(float lenght)
	{
		m_lenght = lenght;
		m_light->setScale(m_light->getScale().x * lenght * 5.12f * FSE_METERS_PER_PIXEL, lenght * 5.12f * FSE_METERS_PER_PIXEL);
	}

	float SpotLight::getLenght() const
	{
		return m_lenght;
	}

	void SpotLight::setAngle(float angle)
	{
		m_angle = std::min(angle, 179.9f);
		m_light->setScale(m_light->getScale().x * m_light->getScale().y * std::tan((m_angle * 3.14159265 / 180.0) / 2), m_light->getScale().y);

	}

	float SpotLight::getAngle() const
	{
		return m_angle;
	}
}
