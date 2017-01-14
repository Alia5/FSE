#pragma once
#include <Box2D/Common/b2Math.h>
#include <SFML/System/Vector2.hpp>

namespace fse
{
	class FMath
	{
	public:
		static float Lerp(float a, float b, float t);
		static b2Vec2 Lerp(b2Vec2 a, b2Vec2 b, float t);
		static sf::Vector2f Lerp(sf::Vector2f a, sf::Vector2f b, float t);

		static sf::Vector2f b2Vec2ToSfVec2f(b2Vec2 b2Vec);
		static b2Vec2 sfVec2fTob2Vec2(sf::Vector2f sfVec2f);

		static float sqrtVec(const sf::Vector2f& vec);

		static float sqrtVec(const b2Vec2& vec);


	};

}
