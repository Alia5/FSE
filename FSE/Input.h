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
		typedef std::map<std::wstring, sf::Keyboard::Key> KeyMap;
		typedef std::map<sf::Keyboard::Key, bool> WasPressedMap;

		typedef std::map<std::wstring, sf::Mouse::Button> MouseButtonMap;
		typedef std::map<sf::Mouse::Button, bool> WasMousePressedMap;

		typedef std::map<std::wstring, unsigned int> JoyStickButtonMap;
		typedef std::map<unsigned int, bool> WasJoyStickButtonPressedMap;

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

		bool hasFocus = true;

		KeyMap keyMap;
		WasPressedMap wasPressedMap;

		MouseButtonMap mouseButtonMap;
		WasMousePressedMap wasMousePressedMap;

		JoyStickButtonMap joyStickButtonMap;
		WasJoyStickButtonPressedMap wasJoyStickButtonPressedMap; //TODO: more for multiple players

		bool textInputEnabled = false;
		bool singleLineText = false;
		std::wstring enteredText;

		//TODO: XINPUT FOR VIBRATION!

	};
}