#include "TestApplication.h"
#include "ReflectionGenerated.h"

#include "O2.h"
using namespace o2;


INITIALIZE_O2;

//o2::Debug::SetupAsRootObject();
//o2::Types::instance.mIsOnTop = IPtr::TreePosition::Root;

// TODO: check this shit
// String ab = "asdasd";
// String str = String::Format("String %s", ab);
// Cant take string as first parameter. Works when string not first parameter
// 

int main()
{
	MAIN_O2;

	Ptr<TestApplication> app = mnew TestApplication();
	app.SetupAsRoot();
	app->Launch();
	app.Release();

	o2Memory.CollectGarbage();

	return 0;
}

