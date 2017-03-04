#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
namespace fse
{
	/*!
	 * \brief Class to control sf::View;
	 */
	class ViewController {
	public:
		ViewController();
		/*!
		 * \brief instantiate ViewController for Rendertarget
		 * \param render_target Ptr to sf::RenderTarget which's view to controll
		 */
		explicit ViewController(sf::RenderTarget* const render_target);
		virtual ~ViewController() = default;

		/*!
		 * Update View position
		 * \param objectPos Position to go to
		 * \param deltaTime elapsed time since last call (in seconds)
		 */
		virtual void moveView(sf::Vector2f objectPos, float deltaTime);

	protected:
		sf::RenderTarget* render_target_;
		sf::Vector2f oldPosition = sf::Vector2f(0, 0);
	};
}
