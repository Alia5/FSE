#include "EngineCore.h"

#include "Application.h"

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

	int EngineCore::exec(Application * application)
	{

		m_Application = application;

		if (showWindow)
		{
			sf::RenderWindow window(sf::VideoMode(1280, 720), "FSE");

			window.setFramerateLimit(120);

			m_Application->setWindow(&window);
			m_Application->init();

			while (window.isOpen() && run)
			{
				if (m_Application != nullptr)
				{
					m_Application->update();
				}

			}

		} else {						//No window => Dedicated server....
			/////////// restrict update rate ///////////
			sf::Clock ApplicationTime;
			float ticktime = 1000 / 60.f;
			while (run)
			{
				float time = ApplicationTime.restart().asSeconds();

				float sleeptime = ticktime - time * 1000;
				if (sleeptime > 0)
				{
					sf::sleep(sf::milliseconds(static_cast<sf::Int32>(sleeptime)));
				}
				/////////// timehandling end ///////////

				m_Application->update();
			}

		}

		return 0;
	}
}