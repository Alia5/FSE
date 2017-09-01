#pragma once
#include "FSE/FSEObject/FSEObject.h"
#include "FSE/SceneDebugger.h"
#include "../TestApp.h"

class GameHandler : public fse::FSEObject {
public:

	GameHandler(fse::Scene* scene);
	~GameHandler();

	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) override;
	void spawned() override;

private:

	void handleInputs(float deltaTime) const;


	void drawSubScene(sf::RenderTarget& target) const;






	void spawnLevel();


	std::unique_ptr<fse::Scene> sub_scene_ = nullptr;
	sf::RenderTexture* sub_scene_target_ = nullptr;


	fse::SceneDebugger scene_debugger_;

};
