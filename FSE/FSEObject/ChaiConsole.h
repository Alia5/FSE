#pragma once
#include "FSEObject.h"

namespace fse
{
	class ChaiConsole : public FSEObject
	{
	public:

		explicit ChaiConsole(Scene* scene);
		ChaiConsole(Scene* scene, const sf::Vector2f& spawnPos);
		~ChaiConsole() override;

		void spawned() override;


		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;

		int setHistoryString(bool up);

		chaiscript::ChaiScript::State replaceDefaultPrints(const chaiscript::ChaiScript::State& state);

	private:

		void addDefaultFuns();

		bool shown_ = false;
		bool input_should_gain_focus_ = true;

		bool output_to_bottom_ = true;

		std::array<char, 512> input_data_;
		std::stringstream output_data_;

		std::vector<std::string> input_history_;
		int history_pos_;

		Signal<>::Connection on_resize_connection_;
		sf::Vector2u win_size_;


	};
}
