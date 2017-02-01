#pragma once

#include "Utils.hpp"

namespace ltbl
{

//////////////////////////////////////////////////////////////////////////
/// \brief Sprite with normals
//////////////////////////////////////////////////////////////////////////
class Sprite : public priv::BaseLight, public sf::Sprite
{
	public:
		Sprite();

		void setTexture(sf::Texture& texture, bool resetRect = false);

		void setNormalsTexture(sf::Texture& normalsTexture);

		const sf::Texture* getNormalsTexture() const;

		void setSpecularTexture(sf::Texture& normalsTexture);

		const sf::Texture* getSpecularTexture() const;

		void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
		void renderNormals(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
		void renderSpecular(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
		
		void setZOrder(int order);
		int getZOrder() const;

	private:
		sf::Texture* mTexture;
		sf::Texture* mNormalsTexture;
		sf::Texture* mSpecularTexture;
		bool mRenderNormals;
		bool mRenderSpecular;
		int mZOrder;
};

} // namespace ltbl
