#pragma once

#include "../FSEObject/FSEObject.h"

#include "../../LTBL2/src/LightSystem.hpp"

namespace fse
{
	/*!
	 * \brief Class holds and manages LTBL2 Light System
	 * Default z-order = 255
	 */
	class FSELightWorld : public FSEObject
	{
	public:
		FSELightWorld(Scene* scene);
		FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos);
		~FSELightWorld();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

		/*!
		 * \brief initializes Lighting system
		 * \param target Ptr to RenderTarget 
		 */
		void init(sf::RenderTarget* target) const;

		/*!
		 * \return lighting active
		 */
		bool getLighting() const;
		/*!
		 * \brief Enable / disable lighting
		 */
		void setLighting(bool lighting);

		/*!
		 * \brief Update view for light system
		 */
		void updateView();

		/*!
		 * \brief Get ambient (==unlit) shadow color
		 * \return ambient color
		 */
		sf::Color getAmbientColor() const;
		/*!
		 * \brief Set ambient (==unlit) shadow color
		 * \param color color
		 */
		void setAmbientColor(const sf::Color color) const;

		/*!
		 * \brief get directional sunlight
		 * Returns a ptr to or creates and returns a ptr to directional sunlight \n 
		 * Is very performance heavy when using lots of light shapes
		 * \return sunlight
		 */
		ltbl::LightDirectionEmission* getSun();

		/*!
		 * \brief Get LTBL2 Light system
		 * \return Ptr to LTBL2 light system
		 */
		ltbl::LightSystem* getLightSystem() const;

		sf::RenderTarget* getNormalTarget();
		sf::RenderTarget* getSpecularTarget();


	private:

		sf::RenderTexture normal_texture_;
		sf::RenderTexture specular_texture_;

		std::unique_ptr<ltbl::LightSystem> light_system_;
		bool lighting_ = true;

		ltbl::LightDirectionEmission* sun_ = nullptr;


		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND
	};
}
