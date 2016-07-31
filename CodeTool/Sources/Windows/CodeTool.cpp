#include "O2.h"
#include "ReflectionGenerated.h"
#include "CodeToolApp.h"

using namespace o2;
using namespace CodeTool;

INITIALIZE_O2;

int main()
{
	MAIN_O2;

	CodeToolApplication app;
	app.Launch();

	return 0;
}

