#pragma once
#include "FSEObject.h"

namespace fse
{
	class LoadingScreen : public FSEObject
	{
	public:
		LoadingScreen(Scene* scene);
		LoadingScreen(Scene* scene, const sf::Vector2f& spawnPos);
		~LoadingScreen();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;
	protected:

	private:
		sf::RectangleShape shape_;
	};
}
