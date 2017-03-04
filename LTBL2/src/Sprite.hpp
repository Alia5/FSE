#pragma once

#include "Utils.hpp"
#include <memory>

namespace ltbl
{
	class LightSystem;

	//////////////////////////////////////////////////////////////////////////
	/// \brief Sprite with normal and specular maps
	//////////////////////////////////////////////////////////////////////////
	class Sprite : public priv::BaseLight, public sf::Transformable
	{
	public:
		Sprite();
		explicit Sprite(LightSystem* light_system);
		~Sprite();

		void setTexture(sf::Texture& texture, bool resetRect = false);

		void setNormalsTexture(sf::Texture& normalsTexture);

		const sf::Texture* getNormalsTexture() const;

		void setSpecularTexture(sf::Texture& normalsTexture);

		const sf::Texture* getSpecularTexture() const;

		void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;
		void renderNormals(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;
		void renderSpecular(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const;

		void setTextureRect(const sf::IntRect& rect);


		void setColor(const sf::Color& color);
		void setAlphaNormals(uint32_t alpha); 		//You wouldn't want to tint the normal map ;)
		void setColorSpecular(const sf::Color& color);

		sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;

	private:

		sf::Texture* mTexture;
		sf::Texture* mNormalsTexture;
		sf::Texture* mSpecularTexture;

		sf::Vertex m_vertices_[3][4];
		sf::IntRect m_texture_rect;

		ltbl::LightSystem* light_system_;

		sf::RenderTarget* mNormalsTarget;
		sf::RenderTarget* mSpecularTarget;

		bool mRenderNormals;
		bool mRenderSpecular;
	};

} // namespace ltbl
