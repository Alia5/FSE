#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Input.h"
#include "AssetLoader.h"

#include "../imgui-1.49/imgui.h"
#include "../imgui-1.49/imgui-SFML.h"

namespace fse
{
	class Application
	{
	PUBLIC_SIGNALS:
		Signal<> on_window_resized_;

	public:
		Application();
		virtual ~Application();

		virtual void update();

		void setWindow(sf::RenderWindow* window);
		void setServerClientType(int type);

		virtual void init() = 0;

		bool isServer() const;


		sf::RenderWindow* getWindow() const;

		Input* getInput();

		NetworkHandler* getNetworkHandler();

		fse::AssetLoader& getAssetLoader();

	protected:
		Input input_;
		Scene root_scene_;
		NetworkHandler network_handler_;
		fse::AssetLoader asset_loader_;

	private:
		bool is_server_ = false;

		sf::Clock application_clock_;

		sf::RenderWindow *render_window_ = nullptr;
	};
}
