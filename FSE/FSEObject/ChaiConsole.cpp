#include "ChaiConsole.h"

#include <imgui.h>
#include <sstream>
#include "imgui_internal.h"
#include "../Application.h"

namespace fse
{
	ChaiConsole::ChaiConsole(Scene* scene) : ChaiConsole(scene, { 0.f, 0.f })
	{
	}

	ChaiConsole::ChaiConsole(Scene* scene, const sf::Vector2f& spawnPos) : fse::FSEObject(scene, spawnPos)
	{
		input_history_.emplace_back("");
	}

	ChaiConsole::~ChaiConsole()
	{
		scene_->getApplication()->on_window_resized_.disconnect(on_resize_connection_);
	}

	void ChaiConsole::spawned()
	{
		input_data_[0] = 0;

		win_size_ = scene_->getApplication()->getWindow()->getSize();
		on_resize_connection_ = scene_->getApplication()->on_window_resized_.connect([this]()
		{
			win_size_ = scene_->getApplication()->getWindow()->getSize();
		});
		getChai()->add(chaiscript::fun([this](const std::string& string)
		{
			
			output_data_ << string;

		}), "ig_puts");

		getChai()->add(chaiscript::fun([this](const std::string& string)
		{

			output_data_ << string << '\n';

		}), "ig_print");
	}

	void ChaiConsole::update(float deltaTime)
	{
		if (input_->wasKeyPressed(L"ToggleConsole0") && input_->wasKeyPressed(L"ToggleConsole1"))
		{
			shown_ = !shown_;
		}
	}

	void ChaiConsole::draw(sf::RenderTarget& target)
	{
		if (shown_)
		{
			ImGui::Begin("ScriptConsole", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(win_size_.x, win_size_.y * 0.40));
			int newlines = 0;
			for (const auto& cha : input_data_)
			{
				if (cha == 0)
					break;
				if (cha == '\n')
					newlines++;
			}
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, 1.f, 1.f, 0.1f));
			ImGui::InputTextMultiline("##ScriptOutput", const_cast<char*>(output_data_.str().data()),
				output_data_.str().size(),
				ImVec2(ImGui::GetCurrentWindow()->Size.x - 15, ImGui::GetCurrentWindow()->Size.y - (13 * newlines + 75)),
				ImGuiInputTextFlags_ReadOnly);
			ImGui::PopStyleColor(1);

			ImGui::BeginChild(ImGui::GetID("##ScriptOutput"));
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			ImGui::EndChild();


			if (ImGui::InputTextMultiline("##ScriptInput", input_data_.data(), input_data_.size(),
				ImVec2(ImGui::GetCurrentWindow()->Size.x - 15, 13 * newlines + 25),
				ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_AllowTabInput
				| ImGuiInputTextFlags_EnterReturnsTrue))
			{
				try {
					getChai()->eval(input_data_.data());
				}
				catch (const chaiscript::exception::eval_error& error) {
					output_data_ << error.pretty_print();
				}
				input_history_[input_history_.size() - 1] = (input_data_.data());
				input_history_.reserve(1);
				input_history_.emplace_back("");
				history_pos_ = input_history_.size() - 1;
				input_data_[0] = 0;
				input_should_gain_focus_ = true;
			}

			if (input_should_gain_focus_)
			{
				ImGui::SetKeyboardFocusHere(-1);
				input_should_gain_focus_ = false;
			}

			ImGuiIO& io = ImGui::GetIO();
			if (io.KeyShift && (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_UpArrow]) || ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_DownArrow])))
			{
				if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_UpArrow]))
				{
					history_pos_--;
					if (history_pos_ < 0)
						history_pos_ = 0;
				} 
				else
				{
					history_pos_++;
					if (history_pos_ > input_history_.size() - 1)
						history_pos_ = input_history_.size() - 1;
				}

				std::copy(std::begin(input_history_[history_pos_]),
					std::end(input_history_[history_pos_]), std::begin(input_data_));
				input_data_[input_history_[history_pos_].size()] = 0;
				ImGui::SetKeyboardFocusHere(0);
				input_should_gain_focus_ = true;

			} 

			ImGui::End();
		}
	}

	/**
	 * \brief Replaces chaiscripts print and puts function with ones from this console.
	 * \param state Base application state before replacing the print functions
	 * \return Base application state after replacing print functions
	 */
	chaiscript::ChaiScript::State ChaiConsole::replaceDefaultPrints(const chaiscript::ChaiScript::State& state)
	{
		getChai()->set_state(state);

		getChai()->add(chaiscript::fun([this](const std::string& string)
		{

			output_data_ << string;

		}), "ig_puts");

		getChai()->add(chaiscript::fun([this](const std::string& string)
		{

			output_data_ << string << '\n';

		}), "ig_print");

		getChai()->eval("GLOBAL print = fun[print](s) { ig_print(\"${s}\"); }");
		getChai()->eval("GLOBAL puts = fun[print](s) { ig_puts(\"${s}\"); }");
		return getChai()->get_state();
	}

}
