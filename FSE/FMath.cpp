#include "FMath.h"

namespace fse
{
	float FMath::Lerp(float a, float b, float t)
	{
		if (t > 1.0f)
			t = 1;
		return (a* (1.0f - t)) + (b * t);
	}

	b2Vec2 FMath::Lerp(b2Vec2 a, b2Vec2 b, float t)
	{
		if (t > 1.0f)
			t = 1;
		return b2Vec2((a.x * (1.0f - t)) + (b.x * t), (a.y * (1.0f - t)) + (b.y * t));
	}

	sf::Vector2f FMath::Lerp(sf::Vector2f a, sf::Vector2f b, float t)
	{
		if (t > 1.0f)
			t = 1;
		return sf::Vector2f((a.x * (1.0f - t)) + (b.x * t), (a.y * (1.0f - t)) + (b.y * t));
	}

	sf::Vector2f FMath::b2Vec2ToSfVec2f(b2Vec2 b2Vec)
	{
		return sf::Vector2f(b2Vec.x, b2Vec.y);
	}

	b2Vec2 FMath::sfVec2fTob2Vec2(sf::Vector2f sfVec2f)
	{
		return b2Vec2(sfVec2f.x, sfVec2f.y);
	}

	float FMath::sqrtVec(const sf::Vector2f& vec)
	{
		return sqrtf(vec.x*vec.x + vec.y * vec.y);
	}

	float FMath::sqrtVec(const b2Vec2& vec)
	{
		return sqrtf(vec.x*vec.x + vec.y * vec.y);
	}
}
