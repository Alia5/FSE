#pragma once

#include "FSELightWorld.h"
#include "../AssetLoader.h"

namespace fse
{
	class Light
	{
	public:
		Light();
		Light(const Light& other);
		Light(Scene* scene, const sf::Vector2f& spawnPos, const std::string& path, bool smooth);
		virtual ~Light();

		void setPosition(const sf::Vector2f& pos);
		void setColor(const sf::Color& color);
		void setScale(const sf::Vector2f& scale);

		sf::Vector2f getPosition() const;
		sf::Color getColor() const;
		sf::Vector2f getScale() const;

		void rotate(float angle);
		void setRotation(float angle);
		float getRotation() const;

		void setTurnedOn(bool on);
		bool isTurnedOn() const;
		void toggleTurnedOn();

		Light& operator=(const Light& other);
		
	protected:
		Scene* m_scene = nullptr;
		fse::Texture m_LightTexture;
		ltbl::LightPointEmission* m_light = nullptr;
	private:

	};
}