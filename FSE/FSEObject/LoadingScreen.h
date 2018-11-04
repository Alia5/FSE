#pragma once
#include "FSEObject.h"

namespace fse
{
	/*!
	 * \brief Base class for Loadingscreens \n 
	 * 
	 * Default implementation creates a "black" pixel that fills the screen.
	 */
	class LoadingScreen : public FSEObject
	{
	public:
		LoadingScreen();
		explicit LoadingScreen(const sf::Vector2f& spawnPos);
		~LoadingScreen();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

	protected:
		sf::RectangleShape shape_;
	};
}
