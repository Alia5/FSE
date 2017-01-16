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

		void init(sf::RenderTarget* target);

		bool getLighting() const;
		void setLighting(bool lighting);

		ltbl::LightDirectionEmission* getSun() const;

		ltbl::LightSystem* getLightSystem();

	protected:


	private:
		std::unique_ptr<ltbl::LightSystem> m_light_system;
		bool m_lighting;

		ltbl::LightDirectionEmission* m_Sun = nullptr;

	};
}
