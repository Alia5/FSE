#pragma once

#include "FSELightWorld.h"
#include "../AssetLoader.h"

namespace fse
{
	class BaseLight
	{
	public:
		BaseLight();
		BaseLight(const BaseLight& other);
		BaseLight(Scene* scene, const sf::Vector2f& spawnPos, const std::string& path, bool smooth);
		virtual ~BaseLight();

		void setPosition(const sf::Vector2f& pos);
		void setColor(const sf::Color& color);
		void setScale(const sf::Vector2f& scale);

		sf::Vector2f getPosition() const;
		sf::Color getColor() const;
		sf::Vector2f getScale() const;

		BaseLight& operator=(const BaseLight& other);

	protected:
		Scene* m_scene = nullptr;
		fse::Texture m_LightTexture;
		ltbl::LightPointEmission* m_light = nullptr;
	private:

	};
}