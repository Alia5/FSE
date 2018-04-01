#include "Application.h"
#include <../imgui-sfml-config/imconfig.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include "FSEObject/FSEObject.h"
#include "FSEChaiLib.h"

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#endif

namespace fse
{
	Application::Application() : root_scene_(this)
	{
		input_.init();
		base_chai_state_ = chai_.get_state();
	}

	Application::~Application()
	{
		if (render_window_ != nullptr)
			ImGui::SFML::Shutdown();
	}

	void Application::update()
	{
		if (render_window_ != nullptr)
		{
			sf::Event event;
			while (render_window_->pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed)
				{
					render_window_->close();
					return;
				}

#ifdef ANDROID
				if (event.type == sf::Event::MouseEntered)
				{
					LOGI("MOUSE_ENTERED");
					isActive_ = true;
				}

				if (event.type == sf::Event::MouseLeft)
				{
					LOGI("MOUSE_LEFT");
					isActive_ = false;
					return;
				}
#endif
				if (event.type == sf::Event::Resized)
				{
					on_window_resized_();
				}

				input_.updateEvents(event);

			}
#ifdef ANDROID
			if (!isActive_)
				return;
#endif

			sf::Time time = application_clock_.restart();
			ImGui::SFML::Update(*render_window_, time);

			render_window_->clear();

			root_scene_.update(time.asSeconds());

			root_scene_.draw();

			ImGui::Render();

			render_window_->display();

		} else {
			root_scene_.update(application_clock_.restart().asSeconds());
		}

		network_handler_.updateSignals();

	}

	void Application::initChai()
	{
		chai_.set_state(base_chai_state_);
		priv::FSEChaiLib::Init(chai_);
	}

	void Application::setWindow(sf::RenderWindow * window)
	{
		render_window_ = window;
		root_scene_.setRenderTarget(render_window_);

		ImGui::SFML::Init(*window);
	}

	void Application::setServerClientType(int type)
	{
		if (type == 1)
			is_server_ = true;
	}

	void Application::init()
	{
		initChai();
	}

	bool Application::isServer() const
	{
		return is_server_;
	}

	sf::RenderWindow* Application::getWindow() const
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

	chaiscript::ChaiScript* Application::getChai() 
	{
		return &chai_;
	}
}
