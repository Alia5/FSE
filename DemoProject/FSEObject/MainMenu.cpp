﻿#include "MainMenu.h"

#include <FSE/Application.h>

#include <imgui/imgui.h>
#include "GameLevel.h"

MainMenu::MainMenu(fse::Scene* scene) : FSEObject(scene)
{
}



void MainMenu::spawned()
{
}

void MainMenu::update(float deltaTime)
{
	////////// DEBUG BUTTONS //////////
	//// some debug menu via ImGui ////
	//TODO: Remove later

	ImGui::Begin("DebugMenu##MainMenu", nullptr, ImVec2(200, 100), -1,
		ImGuiWindowFlags_NoResize);

	if (ImGui::Button(" Start Game ##MainMenu"))
	{
		startGame();
	}

	ImGui::End();

	////////// DEBUG BUTTONS //////////
}

void MainMenu::startGame()
{
	auto gh = std::make_unique<GameLevel>(scene_);
	scene_->spawnFSEObject(std::move(gh));
	destroy();
}