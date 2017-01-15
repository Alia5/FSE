#include "LoadingScreen.h"
namespace fse
{

	LoadingScreen::LoadingScreen(Scene* scene) : LoadingScreen(scene, sf::Vector2f(0,0))
	{
	}


	LoadingScreen::LoadingScreen(Scene* scene, const sf::Vector2f& spawnPos) : GameObject(scene, spawnPos)
	{
		shape_.setSize(sf::Vector2f(1,1));
		shape_.setFillColor(sf::Color(32, 32, 32));
	}


	LoadingScreen::~LoadingScreen()
	{
	}

	void LoadingScreen::update(float deltaTime)
	{
	}

	void LoadingScreen::draw(sf::RenderTarget& target)
	{
		sf::View origView = target.getView();
		sf::View my_view;
		my_view.setSize(1, 1);
		target.setView(my_view);
		target.draw(shape_);
		target.setView(origView);
	}

	void LoadingScreen::spawned()
	{
	}
}
