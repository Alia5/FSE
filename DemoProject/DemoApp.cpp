#include "DemoApp.h"

#include <FSE/Lights/FSELightWorld.h>
#include <FSE/FSEObject/ChaiConsole.h>
#include <FSE/FSEObject/FPSCounter.h>
#include "FSEObject/MainMenu.h"


void DemoApp::init()
{

	fse::Application::init();
	initGlobalSettings(); //TODO: create neatly wrapped settings class

	const auto winSize = getWindow()->getSize();
	in_game_target_.create(winSize.x, winSize.y);
	on_resize_connection_ = on_window_resized_.connect(this, &DemoApp::onWindowResized);

	root_scene_.getLightWorld()->setLighting(false);

	auto intro = std::make_unique<MainMenu>(&root_scene_);
	root_scene_.spawnFSEObject(std::move(intro));

	root_scene_.createFSEObject<fse::ChaiConsole>([this](fse::FSEObject* console)
	{
		base_chai_state_ = (dynamic_cast<fse::ChaiConsole*>(console))->replaceDefaultPrints(base_chai_state_);
	});

}

const sf::RenderTexture* DemoApp::getInGameTarget() const
{
	return &in_game_target_;
}

void DemoApp::onWindowResized()
{
	const auto winSize = getWindow()->getSize();
	in_game_target_.create(winSize.x, winSize.y);
}

void DemoApp::initGlobalSettings()
{

	//TODO actually get settings from config

	input_.init("keybinds.cfg");
	sf::RenderWindow* window = getWindow();
	window->create(sf::VideoMode(1920, 1080), "DemoApp");
	window->setFramerateLimit(144);

	root_scene_.createFSEObject<fse::FPSCounter>([](fse::FSEObject* fpscounter) {
		(dynamic_cast<fse::FPSCounter*>(fpscounter))->setShowDetailed(true);
	});
}
