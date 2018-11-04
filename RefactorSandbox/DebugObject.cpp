#include "DebugObject.h"



DebugObject::DebugObject() : fse::FSEObject()
{
}


DebugObject::~DebugObject()
{
}

void DebugObject::spawned()
{
}

void DebugObject::update(float deltaTime)
{
}

void DebugObject::draw(sf::RenderTarget& target)
{
	sf::RectangleShape shape({ 500,500 });
	shape.setFillColor(sf::Color::Red);
	target.draw(shape);
}
