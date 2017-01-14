#include "Input.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif


namespace fse
{	
	Input::Input()
	{
		
	}

	Input::~Input()
	{

	}

	void Input::init()
	{
		KeyMap m =
		{
			{ L"ShowMenu", sf::Keyboard::Key::Escape },
			{ L"Jump", sf::Keyboard::Key::Space },
			{ L"Left", sf::Keyboard::Key::A },
			{ L"Right", sf::Keyboard::Key::D },
			{ L"TogglePhysDebug", sf::Keyboard::Key::F5},
			{ L"TogglePhysDebugAABBs", sf::Keyboard::Key::F6}
		};
		keyMap = m;

		MouseButtonMap mm =
		{
			{ L"Shoot", sf::Mouse::Button::Left },
			{ L"Select", sf::Mouse::Button::Left }
		};
		mouseButtonMap = mm;

		JoyStickButtonMap mj =
		{
			{ L"Jump", 0 }
		};
		joyStickButtonMap = mj;
	}

	void Input::init(KeyMap map)
	{
		keyMap = map;
	}



	void Input::updateEvents(sf::Event event)
	{
		if (event.type == sf::Event::GainedFocus)
			hasFocus = true;
		else if (event.type == sf::Event::LostFocus)
			hasFocus = false;

		else if (event.type == sf::Event::KeyReleased)
		{
			if (wasPressedMap.count(event.key.code))
			{
				wasPressedMap[event.key.code] = false;
			}
		} 
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (wasMousePressedMap.count(event.mouseButton.button))
			{
				wasMousePressedMap[event.mouseButton.button] = false;
			}
		}
		else if (event.type == sf::Event::JoystickButtonReleased)
		{
			if (wasJoyStickButtonPressedMap.count(event.joystickButton.button))
			{
				wasJoyStickButtonPressedMap[event.joystickButton.button] = false;
			}
		}
		else if (textInputEnabled && event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == L'\b')
			{
				if (enteredText.length() > 0)
					enteredText.erase(enteredText.end() - 1);
			}
			else
			{
#ifdef _WIN32
				if (event.text.unicode == L'\x16') //windows Clipboard support! YAY!
				{
					if (OpenClipboard(nullptr))
					{
						HANDLE hData = GetClipboardData(CF_TEXT);
						if (hData != nullptr)
						{
							char * pszText = static_cast<char*>(GlobalLock(hData));
							if (pszText != nullptr)
							{
								std::string text(pszText);
								enteredText += std::wstring(text.begin(), text.end());

								std::wcout << enteredText << "\n";
							}
						}
						GlobalUnlock(hData);
					}
					CloseClipboard();
				}  
#endif
				if (event.text.unicode == L'\r' || event.text.unicode == L'\n')
				{
					if (!singleLineText)
						enteredText += L"\n";
				} else {
					if (std::isprint(static_cast<wchar_t>(event.text.unicode)))
						enteredText += static_cast<wchar_t>(event.text.unicode);
				}
			}
		}
	}

	bool Input::isKeyPressed(std::wstring key)
	{
		if (hasFocus)
		{
			if (keyMap.count(key) && !textInputEnabled)
				return sf::Keyboard::isKeyPressed(keyMap[key]);
			if (mouseButtonMap.count(key))
				return sf::Mouse::isButtonPressed(mouseButtonMap[key]);
			if (joyStickButtonMap.count(key))
			{
				if (sf::Joystick::isConnected(0))											//TODO: Overlaod for multiple Players
					if (sf::Joystick::getButtonCount(0) > joyStickButtonMap[key])
						return sf::Joystick::isButtonPressed(0, joyStickButtonMap[key]);
			}
		}
		return false;
	}

	bool Input::wasKeyPressed(std::wstring key)
	{
		if (hasFocus)
		{
			if (keyMap.count(key) && !textInputEnabled)
			{
				bool isPressed = sf::Keyboard::isKeyPressed(keyMap[key]);
				bool wasPressed = wasPressedMap[keyMap[key]];
				if (isPressed && !wasPressed)
				{
					wasPressedMap[keyMap[key]] = true;
					return true;
				}
			}
			if (mouseButtonMap.count(key))
			{
				bool isPressed = sf::Mouse::isButtonPressed(mouseButtonMap[key]);
				bool wasPressed = wasMousePressedMap[mouseButtonMap[key]];
				if (isPressed && !wasPressed)
				{
					wasMousePressedMap[mouseButtonMap[key]] = true;
					return true;
				}
			}
			if (joyStickButtonMap.count(key))
			{
				if (sf::Joystick::isConnected(0))											//TODO: Overlaod for multiple Players
					if (sf::Joystick::getButtonCount(0) > joyStickButtonMap[key])
					{
						bool isPressed = sf::Joystick::isButtonPressed(0, joyStickButtonMap[key]);
						bool wasPressed = wasJoyStickButtonPressedMap[joyStickButtonMap[key]];
						if (isPressed && !wasPressed)
						{
							wasJoyStickButtonPressedMap[joyStickButtonMap[key]] = true;
							return true;
						}
					}
			}
		}
		return false;
	}

	Input::KeyMap Input::getKeyMap()
	{
		return keyMap;
	}

	void Input::setTextInput(bool enabled)
	{
		textInputEnabled = enabled;
		enteredText = L"";
	}

	void Input::setTextInput(bool enabled, bool singleLine)
	{
		textInputEnabled = enabled;
		singleLineText = singleLine;
		enteredText = L"";
	}

	std::wstring Input::getEnteredText()
	{
		return enteredText;
	}

	void Input::setEnteredText(std::wstring str)
	{
		enteredText = str;
	}
}
