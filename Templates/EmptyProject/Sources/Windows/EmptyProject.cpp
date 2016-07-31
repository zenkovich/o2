#include "O2.h"
#include "ReflectionGenerated.h"
#include "MyApplication.h"

using namespace o2;
using namespace MyApp;

INITIALIZE_O2;

int main()
{
	MAIN_O2;

	MyApplication app;
	app.Launch();

	return 0;
}

