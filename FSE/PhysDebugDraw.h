#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
//#include "Globals.h"



// 1 meter (box2d) is more or less 64 pixels (sfml) //changed the ratio...
#define RATIO 25.0f
#define PIXELS_PER_METER RATIO

// 64 pixels (sfml) are more or less 1 meter (box2d)
#define UNRATIO (1.0F/RATIO)
#define METERS_PER_PIXEL UNRATIO

//formula to convert radians to degrees = (radians * (pi/180))
#define RADTODEG (b2_pi / 180.0)

struct b2AABB;
struct GLRenderPoints;
struct GLRenderLines;
struct GLRenderTriangles;

class PhysDebugDraw : public b2Draw
{
public:
	PhysDebugDraw();
	PhysDebugDraw(sf::RenderTarget &window);
	virtual ~PhysDebugDraw();

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;
	void DrawString(int x, int y, const char* string, ...);
	void DrawString(const b2Vec2& p, const char* string, ...);
	void DrawAABB(b2AABB* aabb, const b2Color& color);
	sf::Color B2SFColor(const b2Color &color, int alpha);

	void drawAABBs(bool aabbs);
	bool getDrawAABBs() const;

private:
	sf::RenderTarget *window;
	bool aabbs_ = false;
};