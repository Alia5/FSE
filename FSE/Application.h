#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Input.h"
#include "AssetLoader.h"

namespace fse
{
	/*!
	 * \brief Base Application class
	 * Handles input forwarding, window events, updating network handler, manages root_scene, manages ImGUI events and rendering \n  
	 * Usage: Inherit from
	 */
	class Application
	{
	PUBLIC_SIGNALS:
		Signal<> on_window_resized_;

	public:
		Application();
		virtual ~Application();

		/*!
		 * \brief Called once per frame
		 * Handles windoe events, updates and renders rootscene / ImGui
		 */
		virtual void update();

		/*!
		 * Set window to handle. \n 
		 * Called by EngineCore
		 * \param window Ptr to sf::RenderWindow
		 */
		void setWindow(sf::RenderWindow* window);
		/*!
		 * Set server client type
		 * TODO: implement
		 */
		void setServerClientType(int type);

		/*!
		 *	Called before first frame is rendered, after window creation \n
		 *	Init your stuff here
		 */
		virtual void init() = 0;

		bool isServer() const;

		/*!
		 * \return Ptr to RenderWindow
		 */
		sf::RenderWindow* getWindow() const;

		/*!
		 * \return Ptr to input handler
		 */
		Input* getInput();

		/*!
		 * \return Ptr to network handler
		 */
		NetworkHandler* getNetworkHandler();

		/*!
		 * \return Reference to asset loader
		 */
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
