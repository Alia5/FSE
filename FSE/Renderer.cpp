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


	void Renderer::render(const std::vector<std::unique_ptr<FSEObject> > &objects)
	{
		sf::View view = render_target_->getView();
		sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
		for (auto & object : objects)
		{
			if (viewRect.intersects(object->GetAABBs()))
				object->draw(*render_target_);
		}
	}
}


