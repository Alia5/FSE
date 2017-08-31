#include "EngineCore.h"

#include "Application.h"


namespace fse
{

	EngineCore::EngineCore()
	{
		
	}


	EngineCore::~EngineCore()
	{
		
	}

	int EngineCore::exec(Application * application)
	{

		application_ = application;

		if (show_window_)
		{
#ifdef ANDROID
			sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
            window.setFramerateLimit(60);
#else
			sf::RenderWindow window(sf::VideoMode(1280, 720), "FSE");
            window.setFramerateLimit(120);
#endif

			application_->setWindow(&window);
			application_->init();
#ifdef ANDROID
            while (window.isOpen() && window.hasFocus() && run_)
#else
			while (window.isOpen() && run_)
#endif
			{
				if (application_ != nullptr)
				{
					application_->update();
				}

			}

		} else {						//No window => Dedicated server....
			/////////// restrict update rate ///////////
			sf::Clock ApplicationTime;
			float ticktime = 1000 / 60.f;
			while (run_)
			{
				float time = ApplicationTime.restart().asSeconds();

				float sleeptime = ticktime - time * 1000;
				if (sleeptime > 0)
				{
					sf::sleep(sf::milliseconds(static_cast<sf::Int32>(sleeptime)));
				}
				/////////// timehandling end ///////////

				application_->update();
			}

		}

		return 0;
	}
}