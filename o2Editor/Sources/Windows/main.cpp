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

	auto t = TypeOf(RectF);

	EditorApplication* app = mnew EditorApplication();
	app->Launch();
	delete app;

	return 0;
}

