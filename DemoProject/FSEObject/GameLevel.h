#pragma once
#include <FSE/FSEObject/FSEObject.h>
#include <FSE/SceneDebugger.h>


class GameLevel : public fse::FSEObject
{
public:
	explicit GameLevel(fse::Scene* scene);


	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) override;
	void spawned() override;
private:


	void handleInputs(float deltaTime) const;


	void drawSubScene(sf::RenderTarget& target) const;

private:
	void spawnLevel();

	Signal<chaiscript::ChaiScript&>::Connection on_chai_init_connection_;


	std::unique_ptr<fse::Scene> sub_scene_ = nullptr;
	sf::RenderTexture* sub_scene_target_ = nullptr;



	fse::SceneDebugger scene_debugger_;

};
