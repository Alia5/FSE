#pragma once
#include "FSEObject.h"
#include <sstream>

namespace v8 {
	class TryCatch;
}

namespace fse
{
	class JSConsole : public FSEObject
	{
	public:

		JSConsole();
		explicit JSConsole(const sf::Vector2f& spawnPos);
		~JSConsole() override;

		void spawned() override;
		void onDespawn() override;


		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;

	private:

		void addDefaultFuns();

		void consoleEval(const std::string& js);
		void v8EvalCatch(v8::TryCatch& try_catch);
		bool v8CheckSyntax();

		bool shown_ = false;
		bool input_should_gain_focus_ = true;

		bool output_to_bottom_ = true;
		bool copy_mode_ = false;
		bool copy_mode_from_ctrl_ = false;

		unsigned int lastInputSize = 0;
		bool inputHasErrors = false;

		std::array<char, 4096> input_data_;
		std::stringstream output_data_;

		std::vector<std::string> input_history_;
		int history_pos_;

		Signal<>::ScopedConnection on_resize_connection_;
		sf::Vector2u win_size_;

		std::streambuf* old_;

		Signal<>::ScopedConnection on_v8_ctx_init_connection_;

		Input* input_;

	};
}
