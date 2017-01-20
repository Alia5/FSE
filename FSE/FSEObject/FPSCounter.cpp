#include "FPSCounter.h"
#include "../Application.h"

namespace fse
{
	FPSCounter::FPSCounter(Scene* scene) : FPSCounter(scene, sf::Vector2f(0, 0))
	{
	}

	FPSCounter::FPSCounter(Scene* scene, const sf::Vector2f& spawnPos) : FSEObject(scene, spawnPos)
	{
		font_.loadFromFile("./data/fonts/bitstream-vera/VeraMoBd.ttf");


		fps_text_.setFont(font_);
		fps_text_.setFillColor(sf::Color::Green);
		fps_text_.setString(L"FPS: 0");
		fps_text_.setCharacterSize(16);

		sf::FloatRect rekt = fps_text_.getLocalBounds();
		background_.setSize(sf::Vector2f(rekt.width + 10, rekt.height + 10));
		background_.setPosition(rekt.left - 10, rekt.top - 10);
		background_.setFillColor(sf::Color(0, 0, 0, 128));

		setZOrder(257);

		counter_view_ = sf::View(sf::FloatRect(0, 0, getScene()->getApplication()->getWindow()->getSize().x, getScene()->getApplication()->getWindow()->getSize().y));
		on_resize_connection_ = getScene()->getApplication()->on_window_resized_.connect([this]()
		{
			counter_view_ = sf::View(sf::FloatRect(0, 0, getScene()->getApplication()->getWindow()->getSize().x, getScene()->getApplication()->getWindow()->getSize().y));
		});

	}

	FPSCounter::~FPSCounter()
	{
		getScene()->getApplication()->on_window_resized_.disconnect(on_resize_connection_);
	}

	void FPSCounter::update(float deltaTime)
	{
	}

	void FPSCounter::draw(sf::RenderTarget& target)
	{
		current_time_ = measureclock_.restart().asSeconds();
		fps_ = (fps_ * smoothing_) + ((1 / current_time_) * (1.0 - smoothing_));
		if (updclock_.getElapsedTime().asMilliseconds() > 250)
		{
			fps_text_.setString(std::wstring(L"FPS: " + std::to_wstring(static_cast<int>(fps_))));
			sf::FloatRect rekt = fps_text_.getLocalBounds();
			background_.setSize(sf::Vector2f(rekt.width + 10, rekt.height + 10));
			background_.setPosition(rekt.left - 5, rekt.top - 5);
			updclock_.restart();
		}

		sf::View origView = target.getView();
		target.setView(counter_view_);
		target.draw(background_);
		target.draw(fps_text_);
		target.setView(origView);


	}

	void FPSCounter::spawned()
	{
	}
}
