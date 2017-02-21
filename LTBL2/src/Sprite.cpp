#include "Sprite.hpp"
#include "LightSystem.hpp"

namespace ltbl
{

	Sprite::Sprite()
		: BaseLight()
		, sf::Sprite()
		, mTexture(nullptr)
		, mNormalsTexture(nullptr)
		, mSpecularTexture(nullptr)
		, light_system_(nullptr)
		, mNormalsTarget(nullptr)
		, mSpecularTarget(nullptr)
		, mRenderNormals(false)
		, mRenderSpecular(false)
	{
	}

	Sprite::Sprite(ltbl::LightSystem* light_system) : BaseLight()
		, sf::Sprite()
		, mTexture(nullptr)
		, mNormalsTexture(nullptr)
		, mSpecularTexture(nullptr)
		, light_system_(light_system)
		, mNormalsTarget(&light_system->getNormalTexture())
		, mSpecularTarget(&light_system->getSpecularTexture())
		, mRenderNormals(true)
		, mRenderSpecular(true)
	{
	}

	Sprite::~Sprite()
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
		sf::Sprite::setTexture(*mTexture);
		target.draw(*this, states);

	}

	void Sprite::renderNormals(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (mNormalsTexture != nullptr && mRenderNormals)
		{
			sf::Sprite::setTexture(*mNormalsTexture);
			target.draw(*this, states);
		}
	}

	void Sprite::renderSpecular(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (mSpecularTexture != nullptr && mRenderSpecular)
		{
			sf::Sprite::setTexture(*mSpecularTexture);
			target.draw(*this, states);
		}
	}

} // namespace ltbl