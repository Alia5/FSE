#include "GameHandler.h"
#include "FSE/FSEObject/FPSCounter.h"

GameHandler::GameHandler(fse::Scene* scene) : FSEObject(scene)
{
	TestApp* app = static_cast<TestApp*>(scene_->getApplication());
	sub_scene_target_ = const_cast<sf::RenderTexture*>(app->getInGameTarget());
	sub_scene_ = std::make_unique<fse::Scene>(app);
	sub_scene_->setRenderTarget(sub_scene_target_);
	sub_scene_->setPaused(true);
}

GameHandler::~GameHandler()
{
}


void GameHandler::update(float deltaTime)
{
	handleInputs(deltaTime);
	sub_scene_->update(deltaTime);

	scene_debugger_.update();
}


void GameHandler::draw(sf::RenderTarget& target)
{
	drawSubScene(target);

}


void GameHandler::spawned()
{
	spawnLevel();
}

void GameHandler::handleInputs(float deltaTime) const
{
	if (input_->wasKeyPressed(L"TogglePhysDebug"))
	{
		sub_scene_->setPhysDrawDebug(!sub_scene_->getPhysDrawDebug());
	}

	if (input_->wasKeyPressed(L"TogglePhysDebugAABBs"))
	{
		sub_scene_->getPhysDebugDraw().drawAABBs(!sub_scene_->getPhysDebugDraw().getDrawAABBs());
	}
}

void GameHandler::drawSubScene(sf::RenderTarget& target) const
{

	// Seems like a bug in SFML to me...
	// We have to draw something before we actually can draw the subscene...
	sf::RectangleShape shape;
	target.draw(shape);
	////////////

	auto view = target.getView();
	auto oldView = view;
	view.setCenter(sf::Vector2f(target.getSize().x, target.getSize().y) / 2.f);
	target.setView(view);

	sub_scene_target_->clear(sf::Color::White); //TODO: Remove arg once there's something to see...
	sub_scene_->draw();
	sub_scene_target_->display();
	sf::Sprite sprt(sub_scene_target_->getTexture());
	sprt.setPosition(view.getCenter() - view.getSize() / 2.f); //TODO: remove when viewcontroller is present...
	target.draw(sprt);

	target.setView(oldView);
}


void GameHandler::spawnLevel()
{
	//TODO Init level

	sub_scene_->setPaused(false);

	scene_debugger_ = fse::SceneDebugger(sub_scene_.get());
	scene_debugger_.enableVector2fToMousePos();
}

