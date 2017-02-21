#pragma once

#include "Utils.hpp"
#include <memory>

namespace ltbl
{
	class LightSystem;

	//////////////////////////////////////////////////////////////////////////
	/// \brief Sprite with normals
	//////////////////////////////////////////////////////////////////////////
	class Sprite : public priv::BaseLight, public sf::Sprite
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

		void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
		void renderNormals(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());
		void renderSpecular(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates());

	private:

		sf::Texture* mTexture;
		sf::Texture* mNormalsTexture;
		sf::Texture* mSpecularTexture;

		ltbl::LightSystem* light_system_;

		sf::RenderTarget* mNormalsTarget;
		sf::RenderTarget* mSpecularTarget;

		bool mRenderNormals;
		bool mRenderSpecular;
	};

} // namespace ltbl
