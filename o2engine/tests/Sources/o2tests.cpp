#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

INITIALIZE_O2;

struct A: public IObject
{
	int a = 0;
	Ptr<IObject> ptr;
};

struct B: public IObject
{
	int b = 2;
	Ptr<IObject> ptr;
};

void ttt()
{
	Ptr<A> pa = mnew A();
	pa->ptr = mnew B();
	((B*)pa->ptr.Get())->ptr = mnew A();

	pa.Release();
}

int main(char** lpCmdLine, int nCmdShow)
{
	ttt();

	MemoryManager::CollectGarbage();

	TestMath();

	Application app;
	app.Launch();

	_getch();

	return 0;
}