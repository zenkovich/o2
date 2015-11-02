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
	o2::Debug::SetupAsRootObject();
	o2::Types::instance.SetupAsRoot();
	o2::FileSystem::SetupAsRootObject();

	Ptr<B> pb = mnew B();
	pb.SetupAsRoot();
	for (int i = 0; i < 10; i++)
		pb->av.Add(mnew A());

	pb.Release();

	Types::InitializeTypes();

	o2Memory.CollectGarbage();

	Ptr<TestApplication> app = mnew TestApplication();
	app.SetupAsRoot();
	app->Launch();

	app.Release();

	o2Memory.CollectGarbage();

	return 0;
}

