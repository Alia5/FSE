#pragma once
#include "GameObject.h"

namespace fse
{
	class LoadingScreen : public GameObject
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
