#pragma once

#include <memory>

#include <SFML/Graphics.hpp>



namespace fse
{
	class FSEObject;

	class Renderer
	{
	public:
		Renderer(sf::RenderTarget *renderTarget);
		~Renderer();

		void setNormalTarget(sf::RenderTarget* normalTarget);
		void setSpecularTarget(sf::RenderTarget* specularTarget);

		void render(const std::vector<std::unique_ptr<FSEObject> > &objects) const;

	private:
		sf::RenderTarget* render_target_;
		sf::RenderTarget* normal_target_ = nullptr;
		sf::RenderTarget* specular_target_ = nullptr;


	};
}