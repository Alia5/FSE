#pragma once

#include <map>
#include <cctype>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>


namespace fse
{
	class Input
	{
	public:
		using KeyMap = std::map<std::wstring, sf::Keyboard::Key>;
		using WasPressedMap = std::map<sf::Keyboard::Key, bool>;

		using MouseButtonMap = std::map<std::wstring, sf::Mouse::Button>;
		using WasMousePressedMap = std::map<sf::Mouse::Button, bool>;

		using JoyStickButtonMap = std::map<std::wstring, unsigned int>;
		using WasJoyStickButtonPressedMap = std::map<unsigned int, bool>;

		Input();
		~Input();

		void init();
		void init(KeyMap map);

		/// Called from MainLoop for each sf::Event to update key and mouse UP events
		void updateEvents(sf::Event event);

		/// Returns true while a key is beeing held down
		///
		/// @param key key beeing checked
		bool isKeyPressed(std::wstring key);
		
		/// Returns true if a key is pressed only once until this key is released again
		///
		/// @param key key beeing checked
		bool wasKeyPressed(std::wstring key);

		KeyMap getKeyMap();

		void setTextInput(bool enabled);
		void setTextInput(bool enabled, bool singleLine);

		std::wstring getEnteredText();
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

		//TODO: XINPUT FOR VIBRATION!

	};
}