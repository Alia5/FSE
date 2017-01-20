#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "FSEObject/DummyObject.h"
#include "Input.h"
#include "AssetLoader.h"

namespace fse
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void update();

		void setWindow(sf::RenderWindow* window);
		void setServerClientType(int type);

		virtual void init() = 0;

		bool isServer() const;


		sf::RenderWindow* getWindow();

		Input* getInput();

		NetworkHandler* getNetworkHandler();

		fse::AssetLoader& getAssetLoader();

	protected:
		//Scene root_scene_;
		Scene *getRootScene();

	PUBLIC_SIGNALS:
		Signal<> on_window_resized_;

	private:
		bool is_server_ = false;

		Input input_;

		sf::Clock application_clock_;

		sf::RenderWindow *render_window_ = nullptr;

		Scene root_scene_;

		NetworkHandler network_handler_;

		fse::AssetLoader asset_loader_;
	};
}
