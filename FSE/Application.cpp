#include "Application.h"

namespace fse
{
	Application::Application() : m_RootScene(this)
	{
		m_input.init();
	}

	Application::~Application()
	{
		std::wcout << "Destroying Application Base" <<  std::endl;
	}

	void Application::update()
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

			m_RootScene.update(m_ApplicationClock.restart().asSeconds());

			m_RootScene.draw();

			m_RWindow->display();

		} else {
			m_RootScene.update(m_ApplicationClock.restart().asSeconds());
		}


	}

	void Application::setWindow(sf::RenderWindow * window)
	{
		m_RWindow = window;
		m_RootScene.setRenderTarget(m_RWindow);
	}

	void Application::setServerClientType(int type)
	{
		if (type == 1)
			m_isServer = true;

	}

	bool Application::isServer() const
	{
		return m_isServer;
	}

	Scene * Application::getRootScene()
	{
		return &m_RootScene;
	}

	sf::RenderWindow* Application::getWindow()
	{
		return m_RWindow;
	}

	Input* Application::getInput()
	{
		return &m_input;
	}

	NetworkHandler* Application::getNetworkHandler()
	{
		return &m_network_handler;
	}

	fse::AssetLoader& Application::getAssetLoader()
	{
		return m_assetLoader;
	}
}
