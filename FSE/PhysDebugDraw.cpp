#include "PhysDebugDraw.h"

PhysDebugDraw::PhysDebugDraw()
{
	
}

PhysDebugDraw::PhysDebugDraw(sf::RenderTarget &window)
{
	this->window = &window;

	uint32 flags = 0;
	flags += true			* b2Draw::e_shapeBit;
	flags += true			* b2Draw::e_jointBit;
	flags += false			* b2Draw::e_aabbBit;
	flags += true				* b2Draw::e_centerOfMassBit;
	flags += true * b2Draw::e_particleBit;
	//drawContactPoints = false;
	//drawContactNormals = false;
	//drawContactImpulse = false;
	//drawFrictionImpulse = false;
	//drawStats = false;
	//drawProfile = false;
	this->SetFlags(flags);

}

PhysDebugDraw::~PhysDebugDraw()
{

}

//convert a Box2D (float 0.0f - 1.0f range) color to a SFML color (uint8 0 - 255 range)
sf::Color PhysDebugDraw::B2SFColor(const b2Color &color, int alpha = 255)
{
	sf::Color result((sf::Uint8)(color.r * 255), (sf::Uint8)(color.g * 255), (sf::Uint8)(color.b * 255), (sf::Uint8) alpha);
	return result;
}

void PhysDebugDraw::DrawParticles(const b2Vec2* centers, float32 radius, const b2ParticleColor* colors, int32 count)
{
	sf::CircleShape shape;
	shape.setRadius(radius * RATIO);
	for (int i = 0; i < count; i++)
	{
		shape.setPosition(centers[i].x*RATIO - radius*RATIO, centers[i].y*RATIO - radius*RATIO);
		shape.setFillColor(this->B2SFColor(colors[i].GetColor(), 64));
		this->window->draw(shape);
	}
}

void PhysDebugDraw::drawAABBs(bool aabbs)
{
	uint32 flags = 0;
	flags += true * b2Draw::e_shapeBit;
	flags += true * b2Draw::e_jointBit;
	flags += aabbs * b2Draw::e_aabbBit;
	flags += true * b2Draw::e_centerOfMassBit;
	flags += true * b2Draw::e_particleBit;
	this->SetFlags(flags);
	aabbs_ = aabbs;
}

bool PhysDebugDraw::getDrawAABBs() const
{
	return aabbs_;
}

void PhysDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	sf::ConvexShape polygon(4);

	polygon.setPoint(0, sf::Vector2f(aabb->lowerBound.x*RATIO, aabb->lowerBound.y*RATIO));
	polygon.setPoint(1, sf::Vector2f(aabb->upperBound.x*RATIO, aabb->lowerBound.y*RATIO));
	polygon.setPoint(2, sf::Vector2f(aabb->upperBound.x*RATIO, aabb->upperBound.y*RATIO));
	polygon.setPoint(3, sf::Vector2f(aabb->lowerBound.x*RATIO, aabb->upperBound.y*RATIO));

	polygon.setFillColor(this->B2SFColor(color, 50));
	polygon.setOutlineColor(this->B2SFColor(color, 64));
	polygon.setOutlineThickness(1.0f);
	this->window->draw(polygon);
}



void PhysDebugDraw::DrawTransform(const b2Transform& xf)
{
	float lineProportion = 0.4f;
	b2Vec2 p1 = xf.p, p2;

	//red (X axis)
	p2 = p1 + (lineProportion * xf.q.GetXAxis());
	this->DrawSegment(p1, p2, b2Color(255, 0, 0));

	//green (Y axis)
	p2 = p1 + (lineProportion * xf.q.GetYAxis());
	this->DrawSegment(p1, p2, b2Color(0, 255, 0));
}

void PhysDebugDraw::DrawString(int x, int y, const char* string, ...)
{
}

void PhysDebugDraw::DrawString(const b2Vec2& p, const char* string, ...)
{
}

void PhysDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	sf::VertexArray line;
	line.append(sf::Vertex(sf::Vector2f(p1.x*RATIO, p1.y*RATIO), this->B2SFColor(color)));
	line.append(sf::Vertex(sf::Vector2f(p2.x*RATIO, p2.y*RATIO), this->B2SFColor(color)));
	line.setPrimitiveType(sf::Lines);

	this->window->draw(line);
}

void PhysDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	//no converion in cordinates of center and upper left corner, Circle in sfml is managed by default with the center
	sf::CircleShape circle(radius*RATIO);
	circle.setPosition(center.x*RATIO - radius*RATIO, center.y*RATIO - radius*RATIO);
	circle.setFillColor(this->B2SFColor(color, 32));
	circle.setOutlineColor(this->B2SFColor(color));
	circle.setOutlineThickness(1.f);

	// line of the circle wich shows the angle
	b2Vec2 p = center + (radius * axis);
	this->DrawSegment(center, p, color);

	this->window->draw(circle);
}

void PhysDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	sf::CircleShape circle((radius*RATIO));
	circle.setPosition(center.x*RATIO - radius*RATIO, center.y*RATIO - radius*RATIO);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(this->B2SFColor(color));
	circle.setOutlineThickness(1.f);
	this->window->draw(circle);
}

void PhysDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int32 i = 0; i<vertexCount; i++)
	{
		b2Vec2 vertex = vertices[i];
		polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
	}
	polygon.setFillColor(this->B2SFColor(color, 32));
	polygon.setOutlineColor(this->B2SFColor(color));
	polygon.setOutlineThickness(1.0f);
	this->window->draw(polygon);
}

void PhysDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int32 i = 0; i<vertexCount; i++)
	{
		b2Vec2 vertex = vertices[i];
		polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
	}
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(this->B2SFColor(color));
	polygon.setOutlineThickness(1.0f);
	this->window->draw(polygon);
}
