#include "Renderer.h"

#include "GameObject/GameObject.h"

namespace fse
{
	Renderer::Renderer(sf::RenderTarget *renderTarget) : m_renderTarget(renderTarget)
	{


	}

	Renderer::~Renderer()
	{
	}


	void Renderer::render(const std::vector<std::unique_ptr<GameObject> > &objects)
	{
		for (auto & object : objects)
		{
			object->draw(*m_renderTarget);
		}
	}
}


