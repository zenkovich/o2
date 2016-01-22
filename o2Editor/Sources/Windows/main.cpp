#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"
#include "ReflectionGenerated.h"

using namespace o2;

INITIALIZE_O2;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);

int main()
{
	MAIN_O2;
	
	EditorApplication* app = mnew EditorApplication();
	app->Launch();
	delete app;
	delete MemoryManager::instance;

	return 0;
}

