#include "JSConsole.h"

#include <sstream>
#include "../Application.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "../imgui-colorText.h"
#include <v8pp/module.hpp>

namespace fse
{
	JSConsole::JSConsole() : JSConsole({ 0.f, 0.f })
	{
	}

	JSConsole::JSConsole(const sf::Vector2f& spawnPos) : fse::FSEObject(spawnPos)
	{
		input_history_.emplace_back("");
	}

	JSConsole::~JSConsole()
	{
	}

	void JSConsole::spawned()
	{
		input_data_[0] = 0;

		auto app = Application::get();

		win_size_ = app->getWindow()->getSize();
		on_resize_connection_ = Signal<>::ScopedConnection(
			app->on_window_resized_, 
			    app->on_window_resized_.connect([this]()
		    {
			    win_size_ = Application::get()->getWindow()->getSize();
		    })
		);
		addDefaultFuns();
		old_ = std::cerr.rdbuf(output_data_.rdbuf());
		on_v8_ctx_init_connection_ = Signal<>::ScopedConnection(
			app->on_v8_ctx_init_,
			app->on_v8_ctx_init_.connect(this, &JSConsole::addDefaultFuns)
		);
	}

	void JSConsole::onDespawn()
	{
		std::cerr.rdbuf(old_);
	}

	void JSConsole::update(float deltaTime)
	{
		if (input_->wasKeyPressed(L"ToggleConsole0") && input_->wasKeyPressed(L"ToggleConsole1"))
		{
			shown_ = !shown_;
		}
	}

	void JSConsole::draw(sf::RenderTarget& target)
	{
		if (shown_)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(win_size_.x, win_size_.y * 0.40));
			ImGui::Begin("ScriptConsole", 0,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
			int newlines = 0;
			for (const auto& cha : input_data_)
			{
				if (cha == 0)
					break;
				if (cha == '\n')
					newlines++;
			}

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, 1.f, 1.f, 0.1f));
			if (copy_mode_ || copy_mode_from_ctrl_)
			{
				const std::string out = std::regex_replace(output_data_.str(), std::regex("\33\\[.*?m"), "");
				ImGui::InputTextMultiline("##ScriptOutput", const_cast<char*>(out.data()),
					output_data_.str().size(),
					ImVec2(ImGui::GetCurrentWindow()->Size.x - 15, ImGui::GetCurrentWindow()->Size.y - (13 * newlines + 75)),
					ImGuiInputTextFlags_ReadOnly);
				if (output_to_bottom_)
				{
					ImGui::BeginChild("##ScriptOutput",
						ImVec2(ImGui::GetCurrentWindow()->Size.x - 15, ImGui::GetCurrentWindow()->Size.y - (13 * newlines + 75))
					);
					ImGui::SetScrollY(ImGui::GetScrollMaxY());
					ImGui::EndChild();
				}
			} else {
				ImGui::BeginChild("##ScriptOutput",
					ImVec2(ImGui::GetCurrentWindow()->Size.x - 15, ImGui::GetCurrentWindow()->Size.y - (13 * newlines + 75))
				);
				ImGui::BeginChildFrame(ImGui::GetID("##ScriptOutput"), ImVec2(0, 0));
				ImGui::TextAnsiColored(sf::Color::White, output_data_.str().data());
				if (output_to_bottom_)
					ImGui::SetScrollY(ImGui::GetScrollMaxY());
				ImGui::EndChildFrame();
				ImGui::EndChild();
			}
			ImGui::PopStyleColor(1);

			int winHeight = 13 * newlines + 25;
			if (winHeight >= ImGui::GetCurrentWindow()->Size.y - 110)
				winHeight = ImGui::GetCurrentWindow()->Size.y - 110;
			if (ImGui::InputTextMultiline("##ScriptInput", input_data_.data(), input_data_.size(),
				ImVec2(ImGui::GetCurrentWindow()->Size.x - 15, winHeight),
				ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_AllowTabInput
				| ImGuiInputTextFlags_EnterReturnsTrue))
			{

				output_data_ << "> " << input_data_.data() << '\n';
				
				consoleEval(input_data_.data());
				
				input_history_[input_history_.size() - 1] = (input_data_.data());
				input_history_.reserve(1);
				input_history_.emplace_back("");
				history_pos_ = input_history_.size() - 1;
				input_data_[0] = 0;
				input_should_gain_focus_ = true;
				output_to_bottom_ = true;
			}

			if (input_should_gain_focus_)
			{
				ImGui::SetKeyboardFocusHere(-1);
				input_should_gain_focus_ = false;
			}

			ImGuiIO& io = ImGui::GetIO();
			if (io.KeyCtrl)
			{
				copy_mode_from_ctrl_ = true;
				if ((ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_UpArrow]) || ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_DownArrow])))
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
			} else {
				copy_mode_from_ctrl_ = false;

			}

			ImGui::Checkbox("Auto Scroll##JSConsole", &output_to_bottom_);
			ImGui::SameLine();
			ImGui::Checkbox("Copy Mode##JSConsole", &copy_mode_);
			ImGui::End();
		}
	}

	
	void JSConsole::addDefaultFuns()
	{
		v8::Isolate* iso = v8::Isolate::GetCurrent();
		v8::HandleScope handle_scope(iso);
		v8::Local<v8::Context> ctx = iso->GetEnteredContext();

		v8pp::module console(iso);
		console
			.function("log", [this](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
					v8::HandleScope handle_scope(args.GetIsolate());
					for (int i = 0; i < args.Length(); ++i)
					{
						if (i > 0) output_data_ << ", ";
						v8::String::Utf8Value utf8(args.GetIsolate(), args[i]);
						output_data_  << *utf8;	
					}
					output_data_ << '\n';
			})
			.function("error", [this](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::HandleScope handle_scope(args.GetIsolate());
				output_data_ << "\33[31m";
				for (int i = 0; i < args.Length(); ++i)
				{
					if (i > 0) output_data_ << ", ";
					v8::String::Utf8Value utf8(args.GetIsolate(), args[i]);
					output_data_ << *utf8;
				}
				output_data_ << "\33[0m \n";
				})
			.function("exit", [this]() { shown_ = false;  })
			.function("clear", [this]() { output_data_.str(std::string());  });
		v8::Local<v8::Object> object = console.new_instance();
		ctx->Global()->Set(ctx, v8::String::NewFromUtf8(iso, "console").ToLocalChecked(), object);
		ctx->Global()->Set(ctx, v8::String::NewFromUtf8(iso, "print").ToLocalChecked(),
			object->Get(ctx, v8::String::NewFromUtf8(iso, "log").ToLocalChecked()).ToLocalChecked());		
	}
	void JSConsole::consoleEval(const std::string& js)
	{
		v8::Isolate* iso = v8::Isolate::GetCurrent();
		v8::HandleScope handle_scope(iso);
		const v8::Local<v8::Context> ctx = iso->GetEnteredContext();

		v8::TryCatch try_catch(iso);
		try_catch.SetCaptureMessage(true);
		try_catch.SetVerbose(true);

		v8::MaybeLocal<v8::String> maybe_source =
			v8::String::NewFromUtf8(iso, input_data_.data(),
				v8::NewStringType::kNormal);

		if (maybe_source.IsEmpty())
		{
			v8EvalCatch(try_catch);
			return;
		}
		const v8::Local<v8::String> source = maybe_source.ToLocalChecked();

		v8::ScriptOrigin origin(v8pp::to_v8(iso, "JSConsole"),      // specifier
			v8::Integer::New(iso, 0),             // line offset
			v8::Integer::New(iso, 0),             // column offset
			v8::False(iso),                       // is cross origin
			v8::Local<v8::Integer>(),                     // script id
			v8::Local<v8::Value>(),                       // source map URL
			v8::False(iso),                       // is opaque
			v8::False(iso),                       // is WASM
			v8::False(iso));                       // is ES6 module
		v8::MaybeLocal<v8::Script> maybe_script = v8::Script::Compile(ctx, source, &origin);
		if (maybe_script.IsEmpty())
		{
			v8EvalCatch(try_catch);
			return;
		}
		v8::Local<v8::Script> script = maybe_script.ToLocalChecked();

		
		// Run the script to get the result.
		v8::MaybeLocal<v8::Value> maybe_result = script->Run(ctx);
		if (maybe_result.IsEmpty())
		{
			v8EvalCatch(try_catch);
			return;
		}
		const v8::Local<v8::Value> result = maybe_result.ToLocalChecked();
		
		// Convert the result to an UTF8 string and print it.
		v8::String::Utf8Value utf8(iso, result);
		output_data_ << "\33[32m" << std::string(*utf8) << "\33[0m \n";
		try_catch.Reset();

	}
	
	void JSConsole::v8EvalCatch(v8::TryCatch& try_catch)
	{
		v8::Isolate* iso = v8::Isolate::GetCurrent();
		if (try_catch.HasCaught())
		{
			if (!try_catch.CanContinue())
			{
				throw std::exception(std::string(*v8::String::Utf8Value(iso, try_catch.Message()->Get())).c_str());
			}
			output_data_ <<  "\33[31m" << std::string(*v8::String::Utf8Value(iso, try_catch.Message()->Get())) << "\33[0m \n";
		}
		try_catch.Reset();
	}
}
