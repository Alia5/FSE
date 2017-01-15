#pragma once

#include <iostream>
#include <memory>


#include <SFML/Graphics.hpp>


namespace fse
{
	class Application;
	class EngineCore
	{
	public:
		EngineCore();
		~EngineCore();

		int exec(Application* application);


	

	private:
		Application * m_Application = nullptr;

		bool showWindow = true;
		bool run = true;

	};
}

