#pragma once
#include <FSE/FSEObject/FSEObject.h>

class MainMenu : public fse::FSEObject
{
public:
	MainMenu();


	void spawned() override;
	void onDespawn() override;
	void update(float deltaTime) override;
	void startGame();

private:

};
