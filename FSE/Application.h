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
		~Application();

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
		//Scene m_RootScene;
		Scene *getRootScene();

	PUBLIC_SIGNALS:
		Signal<> onWindowResized;

	private:
		bool m_isServer = false;

		Input m_input;

		sf::Clock m_ApplicationClock;

		sf::RenderWindow *m_RWindow = nullptr;

		Scene m_RootScene;

		NetworkHandler m_network_handler;

		fse::AssetLoader m_assetLoader;
	};
}
