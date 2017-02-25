#include "Sprite.hpp"
#include "LightSystem.hpp"

namespace ltbl
{

	Sprite::Sprite()
		: BaseLight()
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
		// Recompute the texture area if requested, or if there was no valid texture & rect before
		if (resetRect || (!mTexture && (m_texture_rect == sf::IntRect())))
			setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
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

	void Sprite::render(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mTexture)
		{
			states.transform *= getTransform();
			states.texture = mTexture;
			target.draw(m_vertices_[0], 4, sf::TriangleStrip, states);
		}

	}

	void Sprite::renderNormals(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mNormalsTexture != nullptr && mRenderNormals)
		{
			states.transform *= getTransform();
			states.texture = mNormalsTexture;
			target.draw(m_vertices_[1], 4, sf::TriangleStrip, states);
		}
	}

	void Sprite::renderSpecular(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mSpecularTexture != nullptr && mRenderSpecular)
		{
			states.transform *= getTransform();
			states.texture = mSpecularTexture;
			target.draw(m_vertices_[2], 4, sf::TriangleStrip, states);
		}
	}

	void Sprite::setTextureRect(const sf::IntRect& rect)
	{
		m_texture_rect = rect;

		sf::FloatRect bounds = getLocalBounds();

		float left, right, top, bottom;

		for (int i = 0; i < 3; i++)
		{
			m_vertices_[i][0].position = sf::Vector2f(0, 0);
			m_vertices_[i][1].position = sf::Vector2f(0, bounds.height);
			m_vertices_[i][2].position = sf::Vector2f(bounds.width, 0);
			m_vertices_[i][3].position = sf::Vector2f(bounds.width, bounds.height);
			 left = static_cast<float>(m_texture_rect.left);
			 right = left + m_texture_rect.width;
			 top = static_cast<float>(m_texture_rect.top);
			 bottom = top + m_texture_rect.height;

			 m_vertices_[i][0].texCoords = sf::Vector2f(left, top);
			 m_vertices_[i][1].texCoords = sf::Vector2f(left, bottom);
			 m_vertices_[i][2].texCoords = sf::Vector2f(right, top);
			 m_vertices_[i][3].texCoords = sf::Vector2f(right, bottom);

		}

	}

	void Sprite::setColor(const sf::Color& color)
	{
		m_vertices_[0][0].color = color;
		m_vertices_[0][1].color = color;
		m_vertices_[0][2].color = color;
		m_vertices_[0][3].color = color;
	}

	void Sprite::setAlphaNormals(uint32_t alpha)
	{
		sf::Color color(255, 255, 255, alpha);
		m_vertices_[1][0].color = color;
		m_vertices_[1][1].color = color;
		m_vertices_[1][2].color = color;
		m_vertices_[1][3].color = color;
	}

	void Sprite::setColorSpecular(const sf::Color& color)
	{
		m_vertices_[2][0].color = color;
		m_vertices_[2][1].color = color;
		m_vertices_[2][2].color = color;
		m_vertices_[2][3].color = color;
	}

	sf::FloatRect Sprite::getLocalBounds() const
	{
		float width = static_cast<float>(std::abs(m_texture_rect.width));
		float height = static_cast<float>(std::abs(m_texture_rect.height));

		return sf::FloatRect(0.f, 0.f, width, height);
	}

	sf::FloatRect Sprite::getGlobalBounds() const
	{
		return getTransform().transformRect(getLocalBounds());
	}
} // namespace ltbl