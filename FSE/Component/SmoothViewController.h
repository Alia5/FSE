#pragma once

#include "../FMath.h"
#include "ViewController.h"

namespace fse
{
	class SmoothViewController : public ViewController {
	public:
		SmoothViewController();
		SmoothViewController(sf::RenderTarget* render_target);

		void moveView(sf::Vector2f objectPos, float deltaTime) override;
		void setView(sf::Vector2f objectPos);


		float getSizePercentage() const;
		float getVerticalOffset() const;
		void setSizePercentage(float size_percentage);
		void setVerticalOffset(float vertical_offset);
	private:
		float size_percentage_ = 0.0f;
		float vertical_offset_ = 0.0f;
	};
}
