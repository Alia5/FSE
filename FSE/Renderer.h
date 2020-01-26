#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <v8.h>


namespace fse
{
	class FSEObject;
	class ScriptObject;
	class Renderer
	{
	public:
		Renderer(sf::RenderTarget *renderTarget);
		~Renderer();

		void setNormalTarget(sf::RenderTarget* normalTarget);
		void setSpecularTarget(sf::RenderTarget* specularTarget);

		void render(const std::vector<std::shared_ptr<FSEObject> > &objects) const;

	private:
		sf::RenderTarget* render_target_;
		sf::RenderTarget* normal_target_ = nullptr;
		sf::RenderTarget* specular_target_ = nullptr;

		v8::Local<v8::Object> v8_render_target;
		
		friend class ScriptObject;
		inline static v8::Local<v8::Object> _v8_render_target;
		

	};
}
