#include "ViewController.h"
#include "../Scene.h"
#include "../FSEObject/FSEObject.h"

#include <rttr/registration>

namespace fse
{
	ViewController::ViewController()  : render_target_(nullptr)
	{

	}

	ViewController::ViewController(sf::RenderTarget* const render_target) : render_target_(render_target)
	{

	}

	void ViewController::update(float deltaTime)
	{
		if (follow_object_)
			moveView(object_->getPosition(), deltaTime);
	}

	bool ViewController::isFollowingObject() const
	{
		return follow_object_;
	}

	void ViewController::setFollowObject(bool follow)
	{
		follow_object_ = follow;
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

	float ViewController::getZoomLevel() const
	{
		return zoom_level_;
	}

	void ViewController::setZoomLevel(float zoom)
	{
		if (render_target_ == nullptr)
			return;

		if (zoom <= 0.0f)
			zoom = 0.01f;

		auto size = render_target_->getSize();
		auto view = render_target_->getView();
		auto center = view.getCenter();
		view.setSize(size.x / zoom, size.y / zoom);
		view.setCenter(center);
		render_target_->setView(view);
		zoom_level_ = zoom;
	}
}

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

registration::class_<ViewController>("fse::ViewController")
.property("follow_object_", &ViewController::isFollowingObject, &ViewController::setFollowObject)
.property("zoom_level_", &ViewController::getZoomLevel, &ViewController::setZoomLevel )
;
}