#include "EditorApplication.h"
#include "ReflectionGenerated.h"

#include "O2.h"
using namespace o2;

#include "Animation/AnimatedValue.h"

INITIALIZE_O2;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(ApplicationConfig);

int main()
{
	MAIN_O2;

	AnimatedValue<RectF> rr;
	
	Ptr<EditorApplication> app = mnew EditorApplication();
	app.SetupAsRoot();
	app->Launch();
	app.Release();

	o2Memory.CollectGarbage();

	return 0;
}

