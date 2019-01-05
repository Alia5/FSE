#include "GameLevel.h"
#include "DemoProject/DemoApp.h"
#include "MainMenu.h"


#include <imgui.h>


GameLevel::GameLevel()
{

}

void GameLevel::update(float deltaTime)
{
	handleInputs(deltaTime);
	sub_scene_->update(deltaTime);

	scene_debugger_.update();

	////////// DEBUG BUTTONS //////////
	//// some debug menu via ImGui ////
	//TODO: Remove later

	ImGui::Begin("DebugMenu##SettingsMenu", nullptr, ImVec2(200, 100), -1,
		ImGuiWindowFlags_NoResize);

	if (ImGui::Button(" Back ##SettingsMenu"))
	{
		scene_->createFSEObject<MainMenu>();
		destroy();
	}


	ImGui::End();

	////////// DEBUG BUTTONS //////////
}

void GameLevel::draw(sf::RenderTarget& target)
{
	drawSubScene(target);
}

void GameLevel::spawned()
{
	auto app = dynamic_cast<DemoApp*>(scene_->getApplication());
	sub_scene_target_ = const_cast<sf::RenderTexture*>(app->getInGameTarget());
	sub_scene_ = std::make_unique<fse::Scene>(app);
	sub_scene_->setRenderTarget(sub_scene_target_);
	sub_scene_->setPaused(true);

	on_chai_init_connection_ = app->on_chaiscript_init_.connect([this](chaiscript::ChaiScript& chai)
	{
		if (!isPendingKill())
		{
			chai.set_global(chaiscript::var(std::ref(*sub_scene_)), "gameScene");
		}
	});
	scene_->getApplication()->initChai();
	spawnLevel();
}

void GameLevel::onDespawn()
{
}

void GameLevel::handleInputs(float deltaTime) const
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

void GameLevel::drawSubScene(sf::RenderTarget& target) const
{
	auto view = target.getView();
	const auto oldView = view;
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

void GameLevel::spawnLevel()
{
	//TODO Init level
	sub_scene_->setPaused(false);

	scene_debugger_ = fse::SceneDebugger(sub_scene_.get());
	scene_debugger_.enableVector2fToMousePos();
}
