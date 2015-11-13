#include "TestApplication.h"

INITIALIZE_O2;

//o2::Debug::SetupAsRootObject();
//o2::Types::instance.mIsOnTop = IPtr::TreePosition::Root;

// TODO: check this shit
// String ab = "asdasd";
// String str = String::Format("String %s", ab);
// Cant take string as first parameter. Works when string not first parameter
// 

struct A
{
	int x = 5;
};

struct B
{
	int f = 9;

	Vector<Ptr<A>> av;
	Ptr<A> ap = mnew A();
};

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

