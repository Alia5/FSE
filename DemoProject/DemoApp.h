#pragma once

#include <FSE/Application.h>

class DemoApp : public fse::Application
{
public:
	DemoApp() = default;
	void init() override;
	const sf::RenderTexture* getInGameTarget() const;

private:
	void onWindowResized();
	void initGlobalSettings();

	//Before you ask: This is here to save rendertexture recreations
	//No, the subsequent calls to .create() on resize dont actually recreate
	//the rendertexture the same way as the initial one does.
	sf::RenderTexture in_game_target_;

	Signal<>::Connection on_resize_connection_;

};