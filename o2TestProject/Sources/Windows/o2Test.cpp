#include "TestApplication.h"

#include "O2.h"
using namespace o2;

int main()
{
	INITIALIZE_O2;

	TestApplication* app = mnew TestApplication();
	app->Launch();

	return 0;
}

