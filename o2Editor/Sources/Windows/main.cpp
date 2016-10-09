#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"

#include "Scene/Actor.h"
#include "Render/Camera.h"

using namespace o2;
using namespace Editor;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);

int main()
{
	INITIALIZE_O2;

	DataNode data;
	Vector<Actor*> xx = { mnew Actor(), mnew Actor(), mnew Actor() };
	TimeStamp* ts = mnew TimeStamp();
	data = xx[0];

	//ts = data;

	Camera b;
	data = b;
	b = data;

	EditorApplication* app = mnew EditorApplication();
	app->Launch();
	delete app;

	return 0;
}

