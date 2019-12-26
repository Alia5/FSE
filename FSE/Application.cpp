#include "Application.h"
#include <imgui-SFML.h>
#include "FSEObject/FSEObject.h"
#include "FSEV8Lib.h"

#include <v8pp/class.hpp>
#include "V8Debug/v8Inspector.h"

#include <regex>

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
		if (fse_inspector_ != nullptr)
			fse_inspector_->quit();
		requireLib.clearCache();
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
		if (fse_inspector_ != nullptr)
			fse_inspector_->poll();

	}

	void Application::initV8Ctx()
	{
		if (isolate_)
		{
			if (fse_inspector_ != nullptr)
				fse_inspector_->quit();
			requireLib.clearCache();
			v8pp::cleanup(isolate_);
			isolate_->Exit();
			isolate_->Dispose();
		}
		isolate_ = v8::Isolate::New(create_params_);
		isolate_->Enter();
		v8::HandleScope handle_scope(isolate_);
		v8_context_ = v8::Context::New(isolate_);
		v8_context_->Enter();
		priv::FSEV8Lib::Init(argc_, argv_, env_, isolate_, &requireLib);
		bool debug = false;
		unsigned short port = 9229; //node default debug port
		bool block = false;
		for (int i = 0; i < argc_; i++)
		{
			const std::string argStr(argv_[i]);
			std::smatch base_match;
			if (std::regex_match(argStr, base_match, std::regex("--inspect((-brk)*)(=*)(.*)")))
			{
				debug = true;
				if (std::regex_search(argv_[i], std::regex("-brk")))
					block = true;
				if (base_match.size() == 5 && base_match[4].length() > 0)
					port = std::stoi(base_match[4]);
			}
		}
		if (debug)
		{
			fse_inspector_ = std::make_unique<FSEInspector>(platform_.get(), port, block);
			fse_inspector_->startAgent();
		}
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


}
