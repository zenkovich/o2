#include "stdafx.h"
#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"

#include "Scene/Actor.h"
#include "Render/Camera.h"
#include "Utils/System/Time/Timer.h"

using namespace o2;
using namespace Editor;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);

int main()
{
	INITIALIZE_O2;

	auto x1 = Color4::SomeColor(0);
	auto x2 = Color4::SomeColor(1);
	auto x3 = Color4::SomeColor(2);
	auto x4 = Color4::SomeColor(3);
	auto x5 = Color4::SomeColor(4);
	auto x6 = Color4::SomeColor(5);
	
	EditorApplication* app = mnew EditorApplication();
	app->Initialize();
	app->Launch();
	delete app;

	return 0;
}

