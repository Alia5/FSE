#pragma once

#include "FSEObject.h"

#include <iostream>

#include "../EventBus.h"

#include "../Signals.h"

#include <Box2D/Box2D.h>

class DummyObject : public fse::FSEObject
{
public:
	DummyObject(fse::Scene *scene);
	~DummyObject();

	virtual void update(float deltaTime) override;

	virtual void draw(sf::RenderTarget& target) override;

	virtual void spawned() override;

	virtual bool destroy() override;


	void setID(int id);

	void makeBlue();

	void testEventCallback();

private:
	int id = -1;

	bool blue = false;

	bool done = false;

	sf::CircleShape shape;

	float totalTime = 0;


	int test(std::wstring s, int i)
	{
		std::wcout << "Hello From TestMethod!\n" << s << i << "\n";
		return i;
	}

	typedef Signal<std::wstring, int> Signal;
	Signal sig;
	//ScopedConnection<std::wstring, int> connection_;


};