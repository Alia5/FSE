#pragma once

#include <map>
#include <unordered_map>
#include <cctype>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include "FSEV8Lib.h"


namespace fse
{
	/*!
	 * \brief maps input events to pollable actions
	 */
	class Input
	{
	public:
		using KeyMap = std::unordered_map<std::wstring, sf::Keyboard::Key>;
		using WasPressedMap = std::unordered_map<sf::Keyboard::Key, bool>;

		using MouseButtonMap = std::unordered_map<std::wstring, sf::Mouse::Button>;
		using WasMousePressedMap = std::unordered_map<sf::Mouse::Button, bool>;

		using JoyStickButtonMap = std::unordered_map<std::wstring, unsigned int>;
		using WasJoyStickButtonPressedMap = std::unordered_map<unsigned int, bool>;

		Input();
		~Input();

		void init();
		void init(KeyMap map);
		void init(std::string keymap_path);

		/*!
		 * Called from MainLoop for each sf::Event to update key and mouse UP events
		 */
		void updateEvents(sf::Event event);

		/*!
		* Returns true while a key is beeing held down
		* \param key action to check
		*/
		bool isKeyPressed(std::wstring key);
		
		/*!
		* Returns true if a key is pressed only once until this key is released again
		* \param key action to check
		*/
		bool wasKeyPressed(std::wstring key);

		KeyMap getKeyMap() const;

		void setTextInput(bool enabled, bool singleLine = false);

		std::wstring getEnteredText() const;
		void setEnteredText(std::wstring str);

	private:

		bool has_focus_ = true;

		KeyMap key_map_;
		WasPressedMap was_pressed_map_;

		MouseButtonMap mouse_button_map_;
		WasMousePressedMap was_mouse_pressed_map_;

		JoyStickButtonMap joy_stick_button_map_;
		WasJoyStickButtonPressedMap was_joy_stick_button_pressed_map_; //TODO: more for multiple players

		bool text_input_enabled_ = false;
		bool single_line_text_ = false;
		std::wstring entered_text_;

		FSE_V8_ENABLE(Input);
		FSE_V8_REGISTRATION_FRIEND
		
		//TODO: Proper GamepadInput
		//TODO: XINPUT!
		//TODO: SCAPI!

	};


	namespace priv
	{
		static const std::unordered_map<std::string, int> mouse_names_ =
		{
			{ "Mouse_Button_Left", sf::Mouse::Button::Left},
			{ "Mouse_Button_Right", sf::Mouse::Button::Right },
			{ "Mouse_Button_Middle", sf::Mouse::Button::Middle },
			{ "Mouse_Button_XButton1", sf::Mouse::Button::XButton1 },
			{ "Mouse_Button_XButton2", sf::Mouse::Button::XButton2 },
		};

		static const std::unordered_map<std::string, int> mouse_wheel_names_ =
		{
			{ "Mouse_Wheel_Horizontal", sf::Mouse::Wheel::HorizontalWheel },
			{ "Mouse_Wheel_Vertical", sf::Mouse::Wheel::VerticalWheel },
		};

		static const std::unordered_map<std::string, int> key_names_ =
		{
			{ "Key_A", sf::Keyboard::Key::A },
			{ "Key_B", sf::Keyboard::Key::B },
			{ "Key_C", sf::Keyboard::Key::C },
			{ "Key_D", sf::Keyboard::Key::D },
			{ "Key_E", sf::Keyboard::Key::E },
			{ "Key_F", sf::Keyboard::Key::F },
			{ "Key_G", sf::Keyboard::Key::G },
			{ "Key_H", sf::Keyboard::Key::H },
			{ "Key_I", sf::Keyboard::Key::I },
			{ "Key_J", sf::Keyboard::Key::J },
			{ "Key_K", sf::Keyboard::Key::K },
			{ "Key_L", sf::Keyboard::Key::L },
			{ "Key_M", sf::Keyboard::Key::M },
			{ "Key_N", sf::Keyboard::Key::N },
			{ "Key_O", sf::Keyboard::Key::O },
			{ "Key_P", sf::Keyboard::Key::P },
			{ "Key_Q", sf::Keyboard::Key::Q },
			{ "Key_R", sf::Keyboard::Key::R },
			{ "Key_S", sf::Keyboard::Key::S },
			{ "Key_T", sf::Keyboard::Key::T },
			{ "Key_U", sf::Keyboard::Key::U },
			{ "Key_V", sf::Keyboard::Key::V },
			{ "Key_W", sf::Keyboard::Key::W },
			{ "Key_X", sf::Keyboard::Key::X },
			{ "Key_Y", sf::Keyboard::Key::Y },
			{ "Key_Z", sf::Keyboard::Key::Z },
			{ "Key_Num0", sf::Keyboard::Key::Num0 },
			{ "Key_Num1", sf::Keyboard::Key::Num1 },
			{ "Key_Num2", sf::Keyboard::Key::Num2 },
			{ "Key_Num3", sf::Keyboard::Key::Num3 },
			{ "Key_Num4", sf::Keyboard::Key::Num4 },
			{ "Key_Num5", sf::Keyboard::Key::Num5 },
			{ "Key_Num6", sf::Keyboard::Key::Num6 },
			{ "Key_Num7", sf::Keyboard::Key::Num7 },
			{ "Key_Num8", sf::Keyboard::Key::Num8 },
			{ "Key_Num9", sf::Keyboard::Key::Num9 },
			{ "Key_Escape", sf::Keyboard::Key::Escape },
			{ "Key_LControl", sf::Keyboard::Key::LControl },
			{ "Key_LShift", sf::Keyboard::Key::LShift },
			{ "Key_LAlt", sf::Keyboard::Key::LAlt },
			{ "Key_LSystem", sf::Keyboard::Key::LSystem },
			{ "Key_RControl", sf::Keyboard::Key::RControl },
			{ "Key_RShift",	sf::Keyboard::Key::RShift },
			{ "Key_RAlt", sf::Keyboard::Key::RAlt },
			{ "Key_RSystem", sf::Keyboard::Key::RSystem },
			{ "Key_Menu", sf::Keyboard::Key::Menu },
			{ "Key_LBracket", sf::Keyboard::Key::LBracket },
			{ "Key_RBracket", sf::Keyboard::Key::RBracket },
			{ "Key_SemiColon", sf::Keyboard::Key::SemiColon },
			{ "Key_Comma", sf::Keyboard::Key::Comma },
			{ "Key_Period", sf::Keyboard::Key::Period },
			{ "Key_Quote", sf::Keyboard::Key::Quote },
			{ "Key_Slash", sf::Keyboard::Key::Slash },
			{ "Key_BackSlash", sf::Keyboard::Key::BackSlash },
			{ "Key_Tilde", sf::Keyboard::Key::Tilde },
			{ "Key_Equal", sf::Keyboard::Key::Equal },
			{ "Key_Dash", sf::Keyboard::Key::Dash },
			{ "Key_Space", sf::Keyboard::Key::Space },
			{ "Key_Return", sf::Keyboard::Key::Return },
			{ "Key_BackSpace", sf::Keyboard::Key::BackSpace },
			{ "Key_Tab", sf::Keyboard::Key::Tab },
			{ "Key_PageUp", sf::Keyboard::Key::PageUp },
			{ "Key_PageDown", sf::Keyboard::Key::PageDown },
			{ "Key_End", sf::Keyboard::Key::End },
			{ "Key_Home", sf::Keyboard::Key::Home },
			{ "Key_Insert", sf::Keyboard::Key::Insert },
			{ "Key_Delete", sf::Keyboard::Key::Delete },
			{ "Key_Add", sf::Keyboard::Key::Add },
			{ "Key_Subtract", sf::Keyboard::Key::Subtract },
			{ "Key_Multiply", sf::Keyboard::Key::Multiply },
			{ "Key_Divide", sf::Keyboard::Key::Divide },
			{ "Key_Left", sf::Keyboard::Key::Left },
			{ "Key_Right", sf::Keyboard::Key::Right },
			{ "Key_Up", sf::Keyboard::Key::Up },
			{ "Key_Down", sf::Keyboard::Key::Down },
			{ "Key_Numpad0", sf::Keyboard::Key::Numpad0 },
			{ "Key_Numpad1", sf::Keyboard::Key::Numpad1 },
			{ "Key_Numpad2", sf::Keyboard::Key::Numpad2 },
			{ "Key_Numpad3", sf::Keyboard::Key::Numpad3 },
			{ "Key_Numpad4", sf::Keyboard::Key::Numpad4 },
			{ "Key_Numpad5", sf::Keyboard::Key::Numpad5 },
			{ "Key_Numpad6", sf::Keyboard::Key::Numpad6 },
			{ "Key_Numpad7", sf::Keyboard::Key::Numpad7 },
			{ "Key_Numpad8", sf::Keyboard::Key::Numpad8 },
			{ "Key_Numpad9", sf::Keyboard::Key::Numpad9 },
			{ "Key_F1", sf::Keyboard::Key::F1 },
			{ "Key_F2", sf::Keyboard::Key::F2 },
			{ "Key_F3", sf::Keyboard::Key::F3 },
			{ "Key_F4", sf::Keyboard::Key::F4 },
			{ "Key_F5", sf::Keyboard::Key::F5 },
			{ "Key_F6", sf::Keyboard::Key::F6 },
			{ "Key_F7", sf::Keyboard::Key::F7 },
			{ "Key_F8", sf::Keyboard::Key::F8 },
			{ "Key_F9", sf::Keyboard::Key::F9 },
			{ "Key_F10", sf::Keyboard::Key::F10 },
			{ "Key_F11", sf::Keyboard::Key::F11 },
			{ "Key_F12", sf::Keyboard::Key::F12 },
			{ "Key_F13", sf::Keyboard::Key::F13 },
			{ "Key_F14", sf::Keyboard::Key::F14 },
			{ "Key_F15", sf::Keyboard::Key::F15 },
			{ "Key_Pause", sf::Keyboard::Key::Pause },
		};
	}

}
