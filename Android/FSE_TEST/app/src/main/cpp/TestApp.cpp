#include "TestApp.h"
#include <FSE/Lights/FSELightWorld.h>
#include <imgui-1.51/imgui.h>
#include "FSE/FSEObject/FPSCounter.h"

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
    on_resize_connection_ = on_window_resized_.connect(this, &TestApp::onWindowResized);

    root_scene_.getLightWorld()->setLighting(false);
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
}

void TestApp::update() {
    fse::Application::update();
}
