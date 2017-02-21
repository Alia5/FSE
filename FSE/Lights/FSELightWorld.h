#pragma once

#include "../FSEObject/FSEObject.h"

#include "../../LTBL2/src/LightSystem.hpp"

namespace fse
{
	class FSELightWorld : public FSEObject
	{
	public:
		FSELightWorld(Scene* scene);
		FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos);
		~FSELightWorld();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

		void init(sf::RenderTarget* target) const;

		bool getLighting() const;
		void setLighting(bool lighting);

		void updateView();

		ltbl::LightDirectionEmission* getSun();

		ltbl::LightSystem* getLightSystem() const;

		sf::RenderTarget* getNormalTarget();
		sf::RenderTarget* getSpecularTarget();

	protected:


	private:

		sf::RenderTexture normal_texture_;
		sf::RenderTexture specular_texture_;

		std::unique_ptr<ltbl::LightSystem> light_system_;
		bool lighting_ = true;

		ltbl::LightDirectionEmission* sun_ = nullptr;

	};
}
