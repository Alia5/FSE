#include "EngineCore.h"

#include "Game.h"

namespace fse
{

	EngineCore::EngineCore()
	{
		std::wcout << "Creating Core" << std::endl;
	}


	EngineCore::~EngineCore()
	{
		std::wcout << "Destructing Core" << std::endl;
	}

	int EngineCore::exec(Game * game)
	{

		m_game = game;

		if (showWindow)
		{
			sf::RenderWindow window(sf::VideoMode(1280, 720), "ProjectGame");

			window.setFramerateLimit(500);

			m_game->setWindow(&window);
			m_game->init();

			while (window.isOpen() && run)
			{
				if (m_game != nullptr)
				{
					m_game->update();
				}

			}

		} else {						//No window => Dedicated server....
			/////////// restrict update rate ///////////
			sf::Clock gameTime;
			float ticktime = 1000 / 60.f;
			while (run)
			{
				float time = gameTime.restart().asSeconds();

				float sleeptime = ticktime - time * 1000;
				if (sleeptime > 0)
				{
					sf::sleep(sf::milliseconds(static_cast<sf::Int32>(sleeptime)));
				}
				/////////// timehandling end ///////////

				m_game->update();
			}

		}

		return 0;
	}
}