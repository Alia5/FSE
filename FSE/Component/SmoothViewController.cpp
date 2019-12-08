#include "SmoothViewController.h"
#include "../Scene.h"
#include "../FMath.h"

#include <rttr/registration.h>

#include "../FSEObject/FSEObject.h"

namespace fse
{
	SmoothViewController::SmoothViewController() : ViewController()
	{

	}

	SmoothViewController::SmoothViewController(sf::RenderTarget* render_target) : ViewController(render_target)
	{

	}

	void SmoothViewController::moveView(sf::Vector2f objectPos, float deltaTime)
	{
		if (render_target_ == nullptr)
			return;

		sf::View view = render_target_->getView();

		sf::Vector2f sz = view.getSize() * size_percentage_;
		sf::Vector2f ct = view.getCenter();

		const sf::FloatRect viewRekt(ct.x - (sz.x * 0.5f), ct.y - (sz.y * 0.5f) - view.getSize().y*vertical_offset_, sz.x, sz.y);
		
		objectPos = sf::Vector2f(objectPos.x, objectPos.y - (view.getSize().y * object_->getScene()->getMetersPerPixel() * vertical_offset_));

		sf::Vector2f toCenter = fse::FMath::Lerp(view.getCenter(), objectPos*object_->getScene()->getPixelsPerMeter(), deltaTime /** 0.75f*/);
		if (!viewRekt.contains(objectPos * object_->getScene()->getPixelsPerMeter()))
		{
			if (objectPos == oldPosition)
			{
				view.setCenter(toCenter);
			}
			view.move((objectPos - oldPosition) * object_->getScene()->getPixelsPerMeter());
		}
		else
		{
			view.setCenter(toCenter);
		}

		oldPosition = objectPos;

		render_target_->setView(view);
	}

	void SmoothViewController::setView(sf::Vector2f objectPos)
	{
		if (render_target_ == nullptr)
			return;

		sf::View view = render_target_->getView();
		objectPos = sf::Vector2f(objectPos.x, objectPos.y - (view.getSize().y * object_->getScene()->getMetersPerPixel() * vertical_offset_));
		view.setCenter(objectPos * object_->getScene()->getPixelsPerMeter());
		oldPosition = objectPos;
		render_target_->setView(view);
	}

	void SmoothViewController::setSizePercentage(float size_percentage)
	{
		size_percentage_ = size_percentage;
	}

	void SmoothViewController::setVerticalOffset(float vertical_offset)
	{
		vertical_offset_ = vertical_offset;
	}

	void SmoothViewController::setZoomLevel(float zoom)
	{
		if (zoom <= 0.0f)
			zoom = 0.0000001f;
		zoom_level_ = zoom;


		if (render_target_ == nullptr)
			return;

		const auto size = render_target_->getSize();
		auto view = render_target_->getView();
		view.setSize(size.x / zoom, size.y / zoom);
		render_target_->setView(view);
		const auto objectPos = sf::Vector2f(oldPosition.x, oldPosition.y + (view.getSize().y * object_->getScene()->getMetersPerPixel() * vertical_offset_));
		setView(objectPos);
	}

	float SmoothViewController::getSizePercentage() const
	{
		return size_percentage_;
	}

	float SmoothViewController::getVerticalOffset() const
	{
		return vertical_offset_;
	}

	FSE_V8_REGISTER(SmoothViewController)
	{
		RegisterJSUserTypeFromRTTR<SmoothViewController>(isolate);
		//chai.add(chaiscript::base_class<fse::Component, SmoothViewController>());
		//chai.add(chaiscript::base_class<fse::ViewController, SmoothViewController>());
		//chai.add(chaiscript::constructor<SmoothViewController()>(), "SmoothViewController");
		//chai.add(chaiscript::constructor<SmoothViewController(sf::RenderTarget* const)>(), "SmoothViewController");
	}
}

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

registration::class_<SmoothViewController>("fse::SmoothViewController")
	.constructor<>()
	(
		policy::ctor::as_std_shared_ptr
	)
	.property("size_percentage_", &SmoothViewController::getSizePercentage, &SmoothViewController::setSizePercentage)	
	.property("vertical_offset_", &SmoothViewController::getVerticalOffset, &SmoothViewController::setVerticalOffset)
;
}