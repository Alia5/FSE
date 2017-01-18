#include "BaseLight.h"
#include "../Application.h"


namespace fse
{

	BaseLight::BaseLight()
	{
	}

	BaseLight::BaseLight(const BaseLight& other) : m_scene(other.m_scene), m_LightTexture(other.m_LightTexture)
	{
		m_light = m_scene->getLightWorld()->getLightSystem()->createLightPointEmission();
		*m_light = *other.m_light;
	}

	BaseLight::BaseLight(Scene* scene, const sf::Vector2f& spawnPos, const std::string& path, bool smooth) : m_scene(scene)
	{
		m_LightTexture = scene->getApplication()->getAssetLoader().getTexture(path); //"lights/pointLightTexture.png"
		m_LightTexture->setSmooth(smooth);

		m_light = scene->getLightWorld()->getLightSystem()->createLightPointEmission();
		m_light->setOrigin(sf::Vector2f(m_LightTexture->getSize().x * 0.5f, m_LightTexture->getSize().y * 0.5f));
		m_light->setTexture(*m_LightTexture);
		m_light->setColor(sf::Color::White);
		m_light->setPosition(spawnPos * FSE_PIXELS_PER_METER);

	}

	BaseLight::~BaseLight()
	{
		if (m_scene != nullptr && m_light != nullptr)
			m_scene->getLightWorld()->getLightSystem()->removeLight(m_light);
	}

	void BaseLight::setPosition(const sf::Vector2f& pos)
	{
		m_light->setPosition(pos * FSE_PIXELS_PER_METER);
	}

	void BaseLight::setColor(const sf::Color& color)
	{
		m_light->setColor(color);
	}

	void BaseLight::setScale(const sf::Vector2f& scale)
	{
		m_light->setScale(scale);
	}

	sf::Vector2f BaseLight::getPosition() const
	{
		return m_light->getPosition() * FSE_METERS_PER_PIXEL;
	}

	sf::Color BaseLight::getColor() const
	{
		return m_light->getColor();
	}

	sf::Vector2f BaseLight::getScale() const
	{
		return m_light->getScale();
	}

	BaseLight& BaseLight::operator=(const BaseLight& other)
	{
		if (&other == this)
			return *this;

		BaseLight tmp(other);
		std::swap(m_light, tmp.m_light);
		std::swap(m_scene, tmp.m_scene);
		std::swap(m_LightTexture, tmp.m_LightTexture);

		return *this;
	}
}
