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
		for (auto & object : objects)
		{
			object->draw(*render_target_);
		}
	}
}


