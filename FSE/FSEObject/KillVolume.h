#pragma once
#include "FSEObject.h"

namespace fse
{
	/*!
	 *	\brief Kills contacted objects.
	 *	
	 *	A rectengular body that destroys every object, with at least one dynamic body, that comes in contact with it.
	 */
	class KillVolume : public fse::FSEObject
	{
	public:

		KillVolume();
		explicit KillVolume(const sf::Vector2f& spawnPos);
		explicit KillVolume(const sf::Vector2f& spawnPos, const sf::Vector2f& size);

		~KillVolume();

		void setPosition(const sf::Vector2f position) override;
		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

		void BeginContact(FSEObject* otherObject, b2Contact* contact) override;

		sf::FloatRect GetAABBs() const override;

		/*!
		 * \brief Set size of the KillVolume (in meters).
		 */
		void setSize(const sf::Vector2f& size);

		/*!
		* \brief Get size of the KillVolume (in meters).
		* 
		* \returns Size of the KillVolume (in meters)
		*/
		const sf::Vector2f& getSize() const;

	private:
		sf::Vector2f size_;
		b2Body* phys_body_;
		b2Fixture* sensor_ficture_;

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND
	};
}