#include "Game.h"

namespace fse
{
	Game::Game() : m_RootScene(this)
	{
		m_input.init();
	}

	Game::~Game()
	{
		std::wcout << "Destroying Game Base" <<  std::endl;
	}

	void Game::update()
	{
		if (m_RWindow != nullptr)
		{
			sf::Event event;
			while (m_RWindow->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					m_RWindow->close();
					return;
				}

				if (event.type == sf::Event::Resized)
				{
					onWindowResized();
					//m_RootScene.notifyResize();
				}

				m_input.updateEvents(event);

			}

			m_RWindow->clear();

			m_RootScene.update(m_GameClock.restart().asSeconds());

			m_RootScene.draw();

			m_RWindow->display();

		} else {
			m_RootScene.update(m_GameClock.restart().asSeconds());
		}


	}

	void Game::setWindow(sf::RenderWindow * window)
	{
		m_RWindow = window;
		m_RootScene.setRenderTarget(m_RWindow);
	}

	void Game::setServerClientType(int type)
	{
		if (type == 1)
			m_isServer = true;

	}

	bool Game::isServer() const
	{
		return m_isServer;
	}

	Scene * Game::getRootScene()
	{
		return &m_RootScene;
	}

	sf::RenderWindow* Game::getWindow()
	{
		return m_RWindow;
	}

	Input* Game::getInput()
	{
		return &m_input;
	}

	NetworkHandler* Game::getNetworkHandler()
	{
		return &m_network_handler;
	}

	fse::AssetLoader& Game::getAssetLoader()
	{
		return m_assetLoader;
	}
}
