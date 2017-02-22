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

		void setPosition(const sf::Vector2f& pos) const;
		void setColor(const sf::Color& color) const;
		void setScale(const sf::Vector2f& scale) const;

		sf::Vector2f getPosition() const;
		sf::Color getColor() const;
		sf::Vector2f getScale() const;

		void rotate(float angle) const;
		void setRotation(float angle) const;
		float getRotation() const;

		void setTurnedOn(bool on) const;
		bool isTurnedOn() const;
		void toggleTurnedOn() const;

		Light& operator=(const Light& other);
		
	protected:
		Scene* scene_ = nullptr;
		fse::Texture light_texture_;
		ltbl::LightPointEmission* light_ = nullptr;

	};
}