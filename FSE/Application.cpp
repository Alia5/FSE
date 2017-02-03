#include "Application.h"

namespace fse
{
	Application::Application() : root_scene_(this)
	{
		input_.init();
	}

	Application::~Application()
	{
		std::wcout << "Destroying Application Base" <<  std::endl;
	}

	void Application::update()
	{
		if (render_window_ != nullptr)
		{
			sf::Event event;
			while (render_window_->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					render_window_->close();
					return;
				}

				if (event.type == sf::Event::Resized)
				{
					on_window_resized_();
					//root_scene_.notifyResize();
				}

				input_.updateEvents(event);

			}

			render_window_->clear();

			root_scene_.update(application_clock_.restart().asSeconds());

			root_scene_.draw();

			render_window_->display();

		} else {
			root_scene_.update(application_clock_.restart().asSeconds());
		}

		network_handler_.updateSignals();

	}

	void Application::setWindow(sf::RenderWindow * window)
	{
		render_window_ = window;
		root_scene_.setRenderTarget(render_window_);
	}

	void Application::setServerClientType(int type)
	{
		if (type == 1)
			is_server_ = true;

	}

	bool Application::isServer() const
	{
		return is_server_;
	}

	Scene * Application::getRootScene()
	{
		return &root_scene_;
	}

	sf::RenderWindow* Application::getWindow()
	{
		return render_window_;
	}

	Input* Application::getInput()
	{
		return &input_;
	}

	NetworkHandler* Application::getNetworkHandler()
	{
		return &network_handler_;
	}

	fse::AssetLoader& Application::getAssetLoader()
	{
		return asset_loader_;
	}
}
