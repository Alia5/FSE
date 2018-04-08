#pragma once
#include <FSE/FSEObject/FSEObject.h>

class MainMenu : public fse::FSEObject
{
public:
	explicit MainMenu(fse::Scene* scene);


	void spawned() override;
	void update(float deltaTime) override;
	void startGame();

private:

};
