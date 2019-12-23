#include "Application.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "FSEObject/FSEObject.h"
#include "FSEV8Lib.h"

#include <v8pp/class.hpp>
#include <v8pp/module.hpp>

#include <v8-inspector.h>
#include <v8-inspector-protocol.h>


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
		
		// Initialize V8.
		v8::V8::Initialize();
		create_params_.array_buffer_allocator =
			v8::ArrayBuffer::Allocator::NewDefaultAllocator();
		platform_ = v8::platform::NewDefaultPlatform();
		v8::V8::InitializePlatform(platform_.get());		
	}

	Application::Application(int argc, char* argv[], char** env) : Application()
	{
		argc_ = argc;
		argv_ = argv;
		env_ = env;
	}

	Application::~Application()
	{
		requireLib.clearCache();
		v8pp_context_ = nullptr;
		v8pp::cleanup(isolate_);
		isolate_->Exit();
		isolate_->Dispose();
		v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
		delete create_params_.array_buffer_allocator;
		
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

			ImGui::SFML::Render(*render_window_);

			render_window_->display();

		} else {
			root_scene_.update(application_clock_.restart().asSeconds());
		}

		network_handler_.updateSignals();

	}

	void Application::initV8Ctx()
	{
		if (isolate_)
		{
			requireLib.clearCache();
			v8pp_context_ = nullptr;
			v8pp::cleanup(isolate_);
			isolate_->Exit();
			isolate_->Dispose();
		}
		isolate_ = v8::Isolate::New(create_params_);
		isolate_->Enter();
		//v8pp_context_ = std::make_shared<v8pp::context>(isolate_, create_params_.array_buffer_allocator, true);
		//v8pp_context_->set_lib_path("js/node_modules");
		v8::HandleScope handle_scope(isolate_);
		v8_context_ = v8::Context::New(isolate_);
		v8_context_->Enter();
		priv::FSEV8Lib::Init(argc_, argv_, env_, isolate_, &requireLib);
		on_v8_ctx_init_();
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
		initV8Ctx();
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

	std::weak_ptr<v8pp::context> Application::getV8PPContext()
	{
		return v8pp_context_;
	}
}
