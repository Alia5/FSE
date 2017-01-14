#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
namespace fse
{
	class ViewController {
	public:
		ViewController();
		ViewController(sf::RenderTarget* render_target);
		virtual ~ViewController() = default;

		virtual void moveView(sf::Vector2f objectPos, float deltaTime);

	protected:
		sf::RenderTarget* render_target_;
		sf::Vector2f oldPosition = sf::Vector2f(0, 0);
	};
}
