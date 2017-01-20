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
		key_map_ = m;

		MouseButtonMap mm =
		{
			{ L"Shoot", sf::Mouse::Button::Left },
			{ L"Select", sf::Mouse::Button::Left }
		};
		mouse_button_map_ = mm;

		JoyStickButtonMap mj =
		{
			{ L"Jump", 0 }
		};
		joy_stick_button_map_ = mj;
	}

	void Input::init(KeyMap map)
	{
		key_map_ = map;
	}



	void Input::updateEvents(sf::Event event)
	{
		if (event.type == sf::Event::GainedFocus)
			has_focus_ = true;
		else if (event.type == sf::Event::LostFocus)
			has_focus_ = false;

		else if (event.type == sf::Event::KeyReleased)
		{
			if (was_pressed_map_.count(event.key.code))
			{
				was_pressed_map_[event.key.code] = false;
			}
		} 
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (was_mouse_pressed_map_.count(event.mouseButton.button))
			{
				was_mouse_pressed_map_[event.mouseButton.button] = false;
			}
		}
		else if (event.type == sf::Event::JoystickButtonReleased)
		{
			if (was_joy_stick_button_pressed_map_.count(event.joystickButton.button))
			{
				was_joy_stick_button_pressed_map_[event.joystickButton.button] = false;
			}
		}
		else if (text_input_enabled_ && event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == L'\b')
			{
				if (entered_text_.length() > 0)
					entered_text_.erase(entered_text_.end() - 1);
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
								entered_text_ += std::wstring(text.begin(), text.end());

								std::wcout << entered_text_ << "\n";
							}
						}
						GlobalUnlock(hData);
					}
					CloseClipboard();
				}  
#endif
				if (event.text.unicode == L'\r' || event.text.unicode == L'\n')
				{
					if (!single_line_text_)
						entered_text_ += L"\n";
				} else {
					if (std::isprint(static_cast<wchar_t>(event.text.unicode)))
						entered_text_ += static_cast<wchar_t>(event.text.unicode);
				}
			}
		}
	}

	bool Input::isKeyPressed(std::wstring key)
	{
		if (has_focus_)
		{
			if (key_map_.count(key) && !text_input_enabled_)
				return sf::Keyboard::isKeyPressed(key_map_[key]);
			if (mouse_button_map_.count(key))
				return sf::Mouse::isButtonPressed(mouse_button_map_[key]);
			if (joy_stick_button_map_.count(key))
			{
				if (sf::Joystick::isConnected(0))											//TODO: Overlaod for multiple Players
					if (sf::Joystick::getButtonCount(0) > joy_stick_button_map_[key])
						return sf::Joystick::isButtonPressed(0, joy_stick_button_map_[key]);
			}
		}
		return false;
	}

	bool Input::wasKeyPressed(std::wstring key)
	{
		if (has_focus_)
		{
			if (key_map_.count(key) && !text_input_enabled_)
			{
				bool isPressed = sf::Keyboard::isKeyPressed(key_map_[key]);
				bool wasPressed = was_pressed_map_[key_map_[key]];
				if (isPressed && !wasPressed)
				{
					was_pressed_map_[key_map_[key]] = true;
					return true;
				}
			}
			if (mouse_button_map_.count(key))
			{
				bool isPressed = sf::Mouse::isButtonPressed(mouse_button_map_[key]);
				bool wasPressed = was_mouse_pressed_map_[mouse_button_map_[key]];
				if (isPressed && !wasPressed)
				{
					was_mouse_pressed_map_[mouse_button_map_[key]] = true;
					return true;
				}
			}
			if (joy_stick_button_map_.count(key))
			{
				if (sf::Joystick::isConnected(0))											//TODO: Overlaod for multiple Players
					if (sf::Joystick::getButtonCount(0) > joy_stick_button_map_[key])
					{
						bool isPressed = sf::Joystick::isButtonPressed(0, joy_stick_button_map_[key]);
						bool wasPressed = was_joy_stick_button_pressed_map_[joy_stick_button_map_[key]];
						if (isPressed && !wasPressed)
						{
							was_joy_stick_button_pressed_map_[joy_stick_button_map_[key]] = true;
							return true;
						}
					}
			}
		}
		return false;
	}

	Input::KeyMap Input::getKeyMap()
	{
		return key_map_;
	}

	void Input::setTextInput(bool enabled)
	{
		text_input_enabled_ = enabled;
		entered_text_ = L"";
	}

	void Input::setTextInput(bool enabled, bool singleLine)
	{
		text_input_enabled_ = enabled;
		single_line_text_ = singleLine;
		entered_text_ = L"";
	}

	std::wstring Input::getEnteredText()
	{
		return entered_text_;
	}

	void Input::setEnteredText(std::wstring str)
	{
		entered_text_ = str;
	}
}
