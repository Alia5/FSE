#pragma once

#include "FSELightWorld.h"
#include "../AssetLoader.h"
#include "../FSEChaiLib.h"

namespace fse
{
	/*!
	 * \brief Base class for different Light types
	 */
	class Light
	{
	public:
		Light();
		Light(const Light& other);
		Light(Scene* scene, const sf::Vector2f& spawnPos, const std::string& path, bool smooth);
		virtual ~Light();

		/*!
		 * Set position of light (in meters)
		 * \param pos position in meters
		 */
		void setPosition(const sf::Vector2f pos) const;
		/*!
		* Set color of light 
		* \param color color
		*/
		void setColor(const sf::Color color) const;
		/*!
		* Set scale of light (as percentage)
		* \param scale scale
		*/
		void setScale(const sf::Vector2f scale) const;

		/*!
		 * \return position in meters
		 */
		sf::Vector2f getPosition() const;
		/*!
		 * \return color
		 */
		sf::Color getColor() const;
		/*!
		 * \return scale percentage
		 */
		sf::Vector2f getScale() const;

		void rotate(float angle) const;
		void setRotation(float angle) const;
		float getRotation() const;

		void setTurnedOn(bool on) const;
		bool isTurnedOn() const;
		void toggleTurnedOn() const;

		float getZPosition() const;
		void setZPosition(const float zPos);

		Light& operator=(const Light& other);
		
	protected:
		Scene* scene_ = nullptr;
		fse::Texture light_texture_;
		ltbl::LightPointEmission* light_ = nullptr;

	private:
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND

		FSE_CHAI_ENABLE(Light);
		FSE_CHAI_REGISTRATION_FRIEND
	};
}
