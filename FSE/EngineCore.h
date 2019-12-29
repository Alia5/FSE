#pragma once



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
		Application * application_ = nullptr;

		bool show_window_ = true;
		bool run_ = true;

	};
}

