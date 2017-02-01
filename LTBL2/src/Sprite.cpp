#include "Sprite.hpp"

namespace ltbl
{

	Sprite::Sprite()
		: BaseLight()
		, sf::Sprite()
		, mTexture(nullptr)
		, mNormalsTexture(nullptr)
		, mSpecularTexture(nullptr)
		, mRenderNormals(true)
		, mRenderSpecular(true)
		, mZOrder(0)
	{
	}

	void Sprite::setTexture(sf::Texture& texture, bool resetRect)
	{
		sf::Sprite::setTexture(texture, resetRect);
		mTexture = &texture;
	}

	void Sprite::setNormalsTexture(sf::Texture& normalsTexture)
	{
		mNormalsTexture = &normalsTexture;
	}

	const sf::Texture* Sprite::getNormalsTexture() const
	{
		return mNormalsTexture;
	}

	void Sprite::setSpecularTexture(sf::Texture& specularTexture)
	{
		mSpecularTexture = &specularTexture;
	}

	const sf::Texture* Sprite::getSpecularTexture() const
	{
		return mSpecularTexture;
	}

	void Sprite::render(sf::RenderTarget& target, sf::RenderStates states)
	{
		target.draw(*this, states);
	}

	void Sprite::renderNormals(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (mNormalsTexture != nullptr && mRenderNormals)
		{
			sf::Sprite::setTexture(*mNormalsTexture);
			target.draw(*this, states);
			sf::Sprite::setTexture(*mTexture);
		}
	}

	void Sprite::renderSpecular(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (mSpecularTexture != nullptr && mRenderSpecular)
		{
			sf::Sprite::setTexture(*mSpecularTexture);
			target.draw(*this, states);
			sf::Sprite::setTexture(*mTexture);
		}
	}

	void Sprite::setZOrder(int order)
	{
		mZOrder = order;
	}

	int Sprite::getZOrder() const
	{
		return mZOrder;
	}
} // namespace ltbl