#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"
#include "ReflectionGenerated.h"

using namespace o2;

INITIALIZE_O2;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(ApplicationConfig);
DECLARE_SINGLETON(ToolsPanel);

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

