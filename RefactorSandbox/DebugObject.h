#pragma once
#include <FSE/FSEObject/FSEObject.h>
class DebugObject : public fse::FSEObject
{
public:
	 DebugObject();
	~DebugObject();


	void spawned() override;

	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) override;
};

