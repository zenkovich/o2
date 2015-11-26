#include "EditorApplication.h"
#include "ReflectionGenerated.h"

INITIALIZE_O2;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(ApplicationConfig);

int main()
{
	MAIN_O2;
	ReflectionRegistTypes();

	Ptr<EditorApplication> app = mnew EditorApplication();
	app.SetupAsRoot();
	app->Launch();
	app.Release();

	o2Memory.CollectGarbage();

	return 0;
}

