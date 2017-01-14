#pragma once

#include <iostream>
#include <memory>


#include <SFML/Graphics.hpp>


namespace fse
{
	class Game;
	class EngineCore
	{
	public:
		EngineCore();
		~EngineCore();

		int exec(Game* game);


	

	private:
		Game * m_game = nullptr;

		bool showWindow = true;
		bool run = true;

	};
}

