#include "TestApp.h"
#include <FSE/Lights/FSELightWorld.h>
#include <imgui-1.51/imgui.h>
#include "FSE/FSEObject/FPSCounter.h"
#include "FSEObject/GameHandler.h"

TestApp::TestApp() : Application()
{

}

TestApp::~TestApp()
{
    on_window_resized_.disconnect(on_resize_connection_);
}

void TestApp::init()
{
    initGlobalSettings(); //TODO: create neatly wrapped settings class

    auto window_size_ = getWindow()->getSize();
    in_game_target_.create(window_size_.x, window_size_.y);
    on_resize_connection_ = on_window_resized_.connect(this, &TestApp::onWindowResized);

    root_scene_.getLightWorld()->setLighting(false);
}

const sf::RenderTexture* TestApp::getInGameTarget() const
{
    return &in_game_target_;
}

void TestApp::onWindowResized()
{
    auto window_size_ = getWindow()->getSize();
    std::cout << "TestApp: WinResized\n";
}

void TestApp::initGlobalSettings()
{
    input_.init("keybinds.cfg");
#ifndef ANDROID
    sf::RenderWindow* window = getWindow();
    window->create(sf::VideoMode(1920, 1080), "TestApp");
    window->setFramerateLimit(160);
#endif

    root_scene_.createFSEObject<fse::FPSCounter>([](fse::FSEObject* fpscounter) {
        (static_cast<fse::FPSCounter*>(fpscounter))->setShowDetailed(true);
    });

    root_scene_.createFSEObject<GameHandler>();

    /*auto gh = std::make_unique<GameHandler>(root_scene_);
    root_scene_.spawnFSEObject(std::move(gh));*/
}