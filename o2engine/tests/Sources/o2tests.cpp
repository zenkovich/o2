#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

INITIALIZE_O2;

struct B;

struct A
{
	int a = 0;
	Ptr<B> ptr;
};

struct B
{
	int b = 2;
	Ptr<A> ptr;
};

void ttt()
{
	Ptr<A> pa = mnew A();/*
	pa->ptr = mnew B();
	((B*)pa->ptr.Get())->ptr = mnew A();*/

	pa.Release();
}

void* operator new(size_t size, const A&) {
	printf("new x is %i\n", 4);

	return ::operator new(size);
}

void operator delete(void* mem, const A&) {
	printf("delete x is %i\n", 5);

	return ::operator delete(mem);
}

int main(char** lpCmdLine, int nCmdShow)
{
	A a;
	int* xx = new(a) int(5);
	delete xx;

	ttt();

	MemoryManager::CollectGarbage();

	TestMath();

	Application app;
	app.Launch();

	_getch();

	return 0;
}