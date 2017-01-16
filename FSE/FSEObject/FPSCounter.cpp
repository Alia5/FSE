#include "FPSCounter.h"
#include "../Application.h"

namespace fse
{
	FPSCounter::FPSCounter(Scene* scene) : FPSCounter(scene, sf::Vector2f(0, 0))
	{
	}

	FPSCounter::FPSCounter(Scene* scene, const sf::Vector2f& spawnPos) : FSEObject(scene, spawnPos)
	{
		m_font.loadFromFile("./data/fonts/bitstream-vera/VeraMoBd.ttf");


		m_fpsText.setFont(m_font);
		m_fpsText.setFillColor(sf::Color::Green);
		m_fpsText.setString(L"FPS: 0");
		m_fpsText.setCharacterSize(16);

		sf::FloatRect rekt = m_fpsText.getLocalBounds();
		m_background.setSize(sf::Vector2f(rekt.width + 10, rekt.height + 10));
		m_background.setPosition(rekt.left - 10, rekt.top - 10);
		m_background.setFillColor(sf::Color(0, 0, 0, 128));

		setZOrder(257);

		counterView = sf::View(sf::FloatRect(0, 0, getScene()->getApplication()->getWindow()->getSize().x, getScene()->getApplication()->getWindow()->getSize().y));
		onResizeConnection = getScene()->getApplication()->onWindowResized.connect([this]()
		{
			counterView = sf::View(sf::FloatRect(0, 0, getScene()->getApplication()->getWindow()->getSize().x, getScene()->getApplication()->getWindow()->getSize().y));
		});

	}

	FPSCounter::~FPSCounter()
	{
		getScene()->getApplication()->onWindowResized.disconnect(onResizeConnection);
	}

	void FPSCounter::update(float deltaTime)
	{
	}

	void FPSCounter::draw(sf::RenderTarget& target)
	{
		currentTime = measureclock.restart().asSeconds();
		fps = (fps * smoothing) + ((1 / currentTime) * (1.0 - smoothing));
		if (updclock.getElapsedTime().asMilliseconds() > 250)
		{
			m_fpsText.setString(std::wstring(L"FPS: " + std::to_wstring(static_cast<int>(fps))));
			sf::FloatRect rekt = m_fpsText.getLocalBounds();
			m_background.setSize(sf::Vector2f(rekt.width + 10, rekt.height + 10));
			m_background.setPosition(rekt.left - 5, rekt.top - 5);
			updclock.restart();
		}

		sf::View origView = target.getView();
		target.setView(counterView);
		target.draw(m_background);
		target.draw(m_fpsText);
		target.setView(origView);


	}

	void FPSCounter::spawned()
	{
	}
}
