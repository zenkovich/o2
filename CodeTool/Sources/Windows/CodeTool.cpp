#include "O2.h"
#include "ReflectionGenerated.h"
#include "CodeToolApp.h"

using namespace o2;
using namespace CodeTool;

INITIALIZE_O2;

int main(int nargs, char** agrs)
{
	MAIN_O2;

	CodeToolApplication app;
	app.SetArguments(agrs, nargs);
	app.Launch();

	return 0;
}

