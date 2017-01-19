#include "Light.h"
#include "../Application.h"


namespace fse
{

	Light::Light()
	{
	}

	Light::Light(const Light& other) : m_scene(other.m_scene), m_LightTexture(other.m_LightTexture)
	{
		m_light = m_scene->getLightWorld()->getLightSystem()->createLightPointEmission();
		*m_light = *other.m_light;
	}

	Light::Light(Scene* scene, const sf::Vector2f& spawnPos, const std::string& path, bool smooth) : m_scene(scene)
	{
		m_LightTexture = scene->getApplication()->getAssetLoader().getTexture(path); //"lights/pointLightTexture.png"
		m_LightTexture->setSmooth(smooth);

		m_light = scene->getLightWorld()->getLightSystem()->createLightPointEmission();
		m_light->setOrigin(sf::Vector2f(m_LightTexture->getSize().x * 0.5f, m_LightTexture->getSize().y * 0.5f));
		m_light->setTexture(*m_LightTexture);
		m_light->setColor(sf::Color::White);
		m_light->setPosition(spawnPos * FSE_PIXELS_PER_METER);

	}

	Light::~Light()
	{
		if (m_scene != nullptr && m_light != nullptr)
			m_scene->getLightWorld()->getLightSystem()->removeLight(m_light);
	}

	void Light::setPosition(const sf::Vector2f& pos)
	{
		m_light->setPosition(pos * FSE_PIXELS_PER_METER);
	}

	void Light::setColor(const sf::Color& color)
	{
		m_light->setColor(color);
	}

	void Light::setScale(const sf::Vector2f& scale)
	{
		m_light->setScale(scale);
	}

	sf::Vector2f Light::getPosition() const
	{
		return m_light->getPosition() * FSE_METERS_PER_PIXEL;
	}

	sf::Color Light::getColor() const
	{
		return m_light->getColor();
	}

	sf::Vector2f Light::getScale() const
	{
		return m_light->getScale();
	}

	Light& Light::operator=(const Light& other)
	{
		if (&other == this)
			return *this;

		Light tmp(other);
		std::swap(m_light, tmp.m_light);
		std::swap(m_scene, tmp.m_scene);
		std::swap(m_LightTexture, tmp.m_LightTexture);

		return *this;
	}
}
