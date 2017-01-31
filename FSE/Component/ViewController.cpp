#include "ViewController.h"
#include "../Scene.h"

namespace fse
{
	ViewController::ViewController()  : render_target_(nullptr)
	{

	}

	ViewController::ViewController(sf::RenderTarget* const render_target) : render_target_(render_target)
	{

	}

	void ViewController::moveView(sf::Vector2f objectPos, float deltaTime)
	{
		if (render_target_ == nullptr)
			return;

		sf::View view = render_target_->getView();

		view.move((objectPos - oldPosition) * FSE_PIXELS_PER_METER);

		oldPosition = objectPos;

		render_target_->setView(view);
	}
}