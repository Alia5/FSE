#pragma once

#include <FSE/Application.h>
#include <FSE/SceneDebugger.h>

class TestApp : public fse::Application
{
public:
    TestApp();
    ~TestApp();

    enum EntityCategory {
        BOUNDARY = 0b00000001,
        PLAYER = 0b00000010,
        PLAYER_BULLET = 0b00000100,
        PLAYER_SENSOR = 0b00001000,
        ENEMY = 0b00010000,
        ENEMY_BULLET = 0b00100000,
        ENEMY_SENSOR = 0b01000000
    };

    void init() override;

    const sf::RenderTexture* getInGameTarget() const;


private:

    sf::RenderTexture in_game_target_;

    void onWindowResized();

    void initGlobalSettings();

    Signal<>::Connection on_resize_connection_;
};