#pragma once

#include "ViewController.h"

namespace fse
{
	/*!
	* \brief Class to control sf::View in a lerped way;
	*/
	class SmoothViewController : public ViewController {
	public:
		SmoothViewController();
		/*!
		* \brief instantiate ViewController for Rendertarget
		* \param render_target Ptr to sf::RenderTarget which's view to controll
		*/
		explicit SmoothViewController(sf::RenderTarget* render_target);

		/*!
		* Update View position lerped
		* \param objectPos Position to go to
		* \param deltaTime elapsed time since last call (in seconds)
		*/
		void moveView(sf::Vector2f objectPos, float deltaTime) override;
		/*!
		* Update View position instantaneously
		* \param objectPos Position to go to
		*/
		void setView(sf::Vector2f objectPos);

		/*!
		* Get screen size percentage in which some movement slack is allowed
		* \return current size percentage
		*/
		float getSizePercentage() const;
		/*!
		 * Get vertical offset for view center (in percent)
		 * \return current vertical offset percentage
		 */
		float getVerticalOffset() const;
		/*!
		* Set screen size percentage in which some movement slack is allowed
		* \param size_percentage Percentage
		*/
		void setSizePercentage(float size_percentage);
		/*!
		 * \brief Set vertical offset for view center (in percent)
		 * 0.0 = Center on position
		 * 0.5 = Center on position - 50% of vertical screen height
		 * \param vertical_offset Percentage
		 */
		void setVerticalOffset(float vertical_offset);

	private:
		float size_percentage_ = 0.75f;
		float vertical_offset_ = 0.0f;

		RTTR_ENABLE(fse::ViewController)
		RTTR_REGISTRATION_FRIEND
	};
}
