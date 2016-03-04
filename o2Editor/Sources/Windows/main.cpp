#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"
#include "ReflectionGenerated.h"

#include "Scene/Actor.h"
#include "Utils/Serializer.h"

using namespace o2;

INITIALIZE_O2;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);


int main()
{
	MAIN_O2;

	DataNode data;
	TimeStamp t;
	Vec2F f;
	char y = 'd';
	Actor* ss = nullptr;
	Vector<Actor*> arr;
	Vector<Basis> ass;
	Dictionary<int, Actor*> dc;
	bool xxx = DataNode::IsSupport<Actor*>::value;
	data = ss;
	int h = 5;
	Serializer::Serialize<char>(y, data);
	Serializer::Deserialize(y, data);

	EditorApplication* app = mnew EditorApplication();
	app->Launch();
	delete app;
	delete MemoryManager::instance;

	return 0;
}

