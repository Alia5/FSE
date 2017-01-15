#pragma once

#include <memory>

#include <SFML/Graphics.hpp>



namespace fse
{
	class FSEObject;

	class Renderer
	{
	public:
		Renderer( sf::RenderTarget *renderTarget);
		~Renderer();

		void render(const std::vector<std::unique_ptr<FSEObject> > &objects);

	private:
		 sf::RenderTarget *m_renderTarget;

	};
}