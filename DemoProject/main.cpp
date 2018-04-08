#include <FSE/EngineCore.h>
#include "DemoApp.h"

int main()
{
	fse::EngineCore core;
	DemoApp app;
	return core.exec(&app);
}