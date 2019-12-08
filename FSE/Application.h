#pragma once

#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Input.h"
#include "AssetLoader.h"

#include <libplatform/libplatform.h>
#include <v8.h>


namespace fse
{
	/*!
	 * \brief Base Application class \n
	 * Handles input forwarding, window events, updating network handler, manages root_scene, manages ImGUI events and rendering \n  
	 * Usage: \n
	 * \code
	 *		//.h
	 *		class YourApplicationClass : public fse::Application
	 *		{
	 *		public:
	 *		YourApplicationClass();
	 *		~YourApplicationClass();
	 *
	 *		void init() override;
	 *		};
	 *		//.cpp
	 *		YourApplicationClass::YourApplicationClass() {
	 *		}
	 *		YourApplicationClass::~YourApplicationClass() {
	 *		}
	 *		void YourApplicationClass::init() {
	 *			Application::init();
	 *			root_scene_.createFSEObject<MainMenu>();
	 *			input_.init("keybinds.cfg");
	 *		}
	 * \endcode
	 */
	class Application
	{
	PUBLIC_SIGNALS:
		Signal<> on_window_resized_;
		Signal<> on_v8_ctx_init_;

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
		virtual void init();

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

		virtual void initV8Ctx();
		

	protected:
		Input input_;
		Scene root_scene_;
		NetworkHandler network_handler_;
		fse::AssetLoader asset_loader_;

#ifdef ANDROID
		bool isActive_ = false;
#endif

	private:
		bool is_server_ = false;

		sf::Clock application_clock_;

		sf::RenderWindow *render_window_ = nullptr;

		v8::Isolate::CreateParams create_params_;
		std::unique_ptr<v8::Platform> platform_;
		v8::Isolate* isolate_;
		v8::Local<v8::Context> v8_context_;

	};
}
