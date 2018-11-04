#include "Renderer.h"

#include "FSEObject/FSEObject.h"

namespace fse
{
	Renderer::Renderer(sf::RenderTarget *renderTarget) : render_target_(renderTarget)
	{


	}

	Renderer::~Renderer()
	{
	}

	void Renderer::setNormalTarget(sf::RenderTarget* normalTarget)
	{
		normal_target_ = normalTarget;
	}

	void Renderer::setSpecularTarget(sf::RenderTarget* specularTarget)
	{
		specular_target_ = specularTarget;
	}


	void Renderer::render(const std::vector<std::shared_ptr<FSEObject> > &objects) const
	{
		sf::View view = render_target_->getView();
		sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
		//it's actually faster to iterate over all objects 3 times instead of doing a lot of target switching...
		for (const auto & object : objects)
		{
			if (viewRect.intersects(object->GetAABBs()))
				object->drawNormals(*normal_target_);
		}
		for (const auto & object : objects)
		{
			if (viewRect.intersects(object->GetAABBs()))
				object->drawSpecular(*specular_target_);
		}
		for (const auto & object : objects)
		{
			if (viewRect.intersects(object->GetAABBs()))
				object->draw(*render_target_);
		}
	}
}


