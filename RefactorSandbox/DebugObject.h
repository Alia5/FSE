#pragma once
#include <FSE/FSEObject/FSEObject.h>
class DebugObject : public fse::FSEObject
{
public:
	explicit DebugObject(fse::Scene* scene);
	~DebugObject();


	void spawned() override;

	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) override;
};

