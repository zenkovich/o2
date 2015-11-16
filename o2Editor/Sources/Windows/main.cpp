#include "EditorApplication.h"

INITIALIZE_O2;

int main()
{
	MAIN_O2;

	Ptr<EditorApplication> app = mnew EditorApplication();
	app.SetupAsRoot();
	app->Launch();
	app.Release();

	o2Memory.CollectGarbage();

	return 0;
}

