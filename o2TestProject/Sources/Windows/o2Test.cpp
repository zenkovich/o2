#include "TestApplication.h"

INITIALIZE_O2;

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
	Ptr<B> pb = mnew B();
	for (int i = 0; i < 5; i++)
		pb->av.Add(mnew A());

	Types::InitializeTypes();

	TestApplication app;
	app.Launch();

	o2Memory.CollectGarbage();

	return 0;
}

