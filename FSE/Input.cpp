#include "Input.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <imgui.h>
#include "Scene.h"
#include "Application.h"
#include "FSEObject/FSEObject.h"
#include "Component/Component.h"

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
			{ L"TogglePhysDebugAABBs", sf::Keyboard::Key::F6},
			{ L"ToggleConsole0", sf::Keyboard::Key::Tab},
			{ L"ToggleConsole1", sf::Keyboard::Key::LControl}
		};
		key_map_ = m;

		MouseButtonMap mm =
		{
			{ L"Shoot", sf::Mouse::Button::Left },
			{ L"Select", sf::Mouse::Button::Left },
			{ L"RightClick", sf::Mouse::Button::Right },
			{ L"DebugCameraMove", sf::Mouse::Button::Left },
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

	void Input::init(std::string keymap_path)
	{
		KeyMap km;
		MouseButtonMap mm;

		std::string keymapString;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

		auto it = priv::key_names_.end();

		std::ifstream file;
		file.open("./config/" + keymap_path);

		if (file.is_open())
		{
			int pos;
			std::wstring actionName;
			std::string keyName;
			while (std::getline(file, keymapString))
			{
				keymapString.erase(std::remove_if(keymapString.begin(), keymapString.end(), [](char x)
				{
					return std::isspace(x);
				}), keymapString.end());
				keymapString.erase(std::remove_if(keymapString.begin(), keymapString.end(), [](char x)
				{
					return (x == '"');
				}), keymapString.end());

				if ((pos = keymapString.find("="), pos) != std::wstring::npos)
				{
					actionName = conv.from_bytes(keymapString.substr(0, pos));
					keyName = keymapString.substr(pos + 1);

					if (keyName.find("Key") != std::string::npos)
					{
						it = priv::key_names_.find(keyName);
						if (it != priv::key_names_.end())
							km[actionName] = static_cast<sf::Keyboard::Key>(it->second);
						else
							km[actionName] = sf::Keyboard::Key::Unknown;
					} 
					else if (keyName.find("Mouse_Button") != std::string::npos)
					{
						it = priv::mouse_names_.find(keyName);
						if (it != priv::mouse_names_.end())
							mm[actionName] = static_cast<sf::Mouse::Button>(it->second);
					}
				}

			}
			file.close();
		}
		else
			return;

		key_map_ = km;
		mouse_button_map_ = mm;

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
			if (key_map_.count(key) && !text_input_enabled_ && !ImGui::IsAnyItemActive())
				return sf::Keyboard::isKeyPressed(key_map_[key]);
			if (mouse_button_map_.count(key) && !ImGui::IsMouseHoveringAnyWindow())
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
			if (key_map_.count(key) && !text_input_enabled_ && !ImGui::IsAnyItemActive())
			{
				bool isPressed = sf::Keyboard::isKeyPressed(key_map_[key]);
				bool wasPressed = was_pressed_map_[key_map_[key]];
				if (isPressed && !wasPressed)
				{
					was_pressed_map_[key_map_[key]] = true;
					return true;
				}
			}
			if (mouse_button_map_.count(key) && !ImGui::IsMouseHoveringAnyWindow())
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

	Input::KeyMap Input::getKeyMap() const
	{
		return key_map_;
	}

	void Input::setTextInput(bool enabled, bool singleLine)
	{
		text_input_enabled_ = enabled;
		single_line_text_ = singleLine;
		entered_text_ = L"";
	}

	std::wstring Input::getEnteredText() const
	{
		return entered_text_;
	}

	void Input::setEnteredText(std::wstring str)
	{
		entered_text_ = str;
	}

	FSE_V8_REGISTER(Input)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<Input> Input_class(isolate);
		Input_class.auto_wrap_objects(true);
		Input_class.function("isKeyPressed", &Input::isKeyPressed);
		Input_class.function("wasKeyPressed", &Input::wasKeyPressed);
		module.class_("Input", Input_class);

		//isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
		//	v8::String::NewFromUtf8(isolate, "FSEInput").ToLocalChecked(), input_module.new_instance());
		const v8::Local<v8::Object> object = v8pp::class_<Input>::reference_external(isolate, app->getInput());
		isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
			v8::String::NewFromUtf8(isolate, "Input").ToLocalChecked(), object);
		
		//chai.add(chaiscript::user_type<Input>(), "Input");
		//chai.add(chaiscript::fun([](Scene* scene)
		//{
		//		return scene->getApplication()->getInput();
		//}), "Input");
		////chai.add(chaiscript::fun([](std::shared_ptr<FSEObject> object)
		//{
		//		return object->getScene()->getApplication()->getInput();
		//}), "Input");
		////chai.add(chaiscript::fun([](std::shared_ptr<Component> component)
		//{
		//	return component->getAttachedObject()->getScene()->getApplication()->getInput();
		//}), "Input");
		////chai.add(chaiscript::fun(&Input::isKeyPressed), "isKeyPressed");
		////chai.add(chaiscript::fun([](Input* const input, const std::string& key_name)
		//{
		//	return 	input->isKeyPressed(std::wstring(key_name.begin(), key_name.end()));
		//}), "isKeyPressed");
		////chai.add(chaiscript::fun(&Input::wasKeyPressed), "wasKeyPressed");
		////chai.add(chaiscript::fun([](Input* const input, const std::string& key_name)
		//{
		//	return input->wasKeyPressed(std::wstring(key_name.begin(), key_name.end()));
		//}), "wasKeyPressed");
		
	}
	
}
