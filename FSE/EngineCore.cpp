#include "EngineCore.h"

#include "Application.h"
#include "Random.h"


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
		fse::Random::InitRandom();
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
			while (window.isOpen() && run_)
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

#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;

registration::class_<sf::Vector2f>("sf::Vector2f")
.property("x", &sf::Vector2f::x)
.property("y", &sf::Vector2f::y)
;

registration::class_<sf::Vector2i>("sf::Vector2i")
.property("x", &sf::Vector2i::x)
.property("y", &sf::Vector2i::y)
;

registration::class_<sf::IntRect>("sf::IntRect")
.property("top", &sf::IntRect::top)
.property("left", &sf::IntRect::left)
.property("width", &sf::IntRect::width)
.property("height", &sf::IntRect::height)
;

registration::class_<sf::FloatRect>("sf::FloatRect")
.property("top", &sf::FloatRect::top)
.property("left", &sf::FloatRect::left)
.property("width", &sf::FloatRect::width)
.property("height", &sf::FloatRect::height)
;

registration::class_<sf::Color>("sf::Color")
.property("r", &sf::Color::r)
.property("g", &sf::Color::g)
.property("b", &sf::Color::b)
.property("a", &sf::Color::a)
;

}