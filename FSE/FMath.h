#pragma once
#include <Box2D/Common/b2Math.h>
#include <SFML/System/Vector2.hpp>

namespace fse
{
	namespace FMath
	{
#define PI 3.14159265358979323846

		static float Lerp(float a, float b, float t)
		{
			if (t > 1.0f)
				t = 1;
			return (a* (1.0f - t)) + (b * t);
		}

		static b2Vec2 Lerp(b2Vec2 a, b2Vec2 b, float t)
		{
			if (t > 1.0f)
				t = 1;
			return b2Vec2((a.x * (1.0f - t)) + (b.x * t), (a.y * (1.0f - t)) + (b.y * t));
		}

		static sf::Vector2f Lerp(sf::Vector2f a, sf::Vector2f b, float t)
		{
			if (t > 1.0f)
				t = 1;
			return sf::Vector2f((a.x * (1.0f - t)) + (b.x * t), (a.y * (1.0f - t)) + (b.y * t));
		}

		static sf::Vector2f b2Vec2ToSfVec2f(b2Vec2 b2Vec)
		{
			return sf::Vector2f(b2Vec.x, b2Vec.y);
		}

		static b2Vec2 sfVec2fTob2Vec2(sf::Vector2f sfVec2f)
		{
			return b2Vec2(sfVec2f.x, sfVec2f.y);
		}

		static float sqrtVec(const sf::Vector2f& vec)
		{
			return sqrtf(vec.x*vec.x + vec.y * vec.y);
		}

		static float sqrtVec(const b2Vec2& vec)
		{
			return sqrtf(vec.x*vec.x + vec.y * vec.y);
		}

		static float angle(const sf::Vector2f p1, const sf::Vector2f p2)
		{
			return std::atan2(p2.y - p1.y, p2.x - p1.x);
		}

		static float angleDegree(const sf::Vector2f p1, const sf::Vector2f p2)
		{
			return std::atan2(p2.y - p1.y, p2.x - p1.x) * (180 / PI);
		}


	}

}
