#include "Renderer.h"

#include "FSEObject/FSEObject.h"

namespace fse
{
	Renderer::Renderer(sf::RenderTarget *renderTarget) : m_renderTarget(renderTarget)
	{


	}

	Renderer::~Renderer()
	{
	}


	void Renderer::render(const std::vector<std::unique_ptr<FSEObject> > &objects)
	{
		for (auto & object : objects)
		{
			object->draw(*m_renderTarget);
		}
	}
}


