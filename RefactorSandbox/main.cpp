#include <FSE/EngineCore.h>
#include "RefactorBox.h"

int main()
{
	fse::EngineCore core;
	RefactorBox app;
	return core.exec(&app);
}