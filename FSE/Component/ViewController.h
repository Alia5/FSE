#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "Component.h"

namespace fse
{
	/*!
	 * \brief Class to control sf::View;
	 */
	class ViewController : public Component {
	public:
		ViewController();
		/*!
		 * \brief instantiate ViewController for Rendertarget
		 * \param render_target Ptr to sf::RenderTarget which's view to control
		 */
		explicit ViewController(sf::RenderTarget* const render_target);
		virtual ~ViewController() = default;


		void onAttach() override;

		void update(float deltaTime) override;

		/*!
		 * \brief is the ViewControllerComponent currently following the attached object
		 */
		bool isFollowingObject() const;
		/*!
		 * \brief Set if the ViewControllerComponent should follow the attached object
		 */
		void setFollowObject(bool follow);

		/*!
		 * \brief Ptr to sf::RenderTarget which's view to control
		 */
		void setRenderTarget(sf::RenderTarget* const render_target);

		/*!
		 * Update View position
		 * \param objectPos Position to go to
		 * \param deltaTime elapsed time since last call (in seconds)
		 */
		virtual void moveView(sf::Vector2f objectPos, float deltaTime);

		float getZoomLevel() const;
		virtual void setZoomLevel(float zoom);

	protected:
		sf::RenderTarget* render_target_;
		sf::Vector2f oldPosition = sf::Vector2f(0, 0);
		
		bool follow_object_ = true;

		float zoom_level_ = 1.0f;

	private:

		RTTR_ENABLE(fse::Component)
		RTTR_REGISTRATION_FRIEND

	};
}
