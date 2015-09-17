#include "TestApplication.h"

INITIALIZE_O2;

// TODO: check this shit
// String ab = "asdasd";
// String str = String::Format("String %s", ab);
// Cant take string as first parameter. Works when string not first parameter

int main()
{
	Types::InitializeTypes();

	TestApplication app;
	app.Launch();

	return 0;
}

