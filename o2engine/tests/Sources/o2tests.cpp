#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

FIRST_SERIALIZATION();
CREATE_SINGLETON(MemoryManager);

struct Tester
{
	Tester(IObject* obj, const char* id)
	{
		printf("Field %s on %x\n", id, obj);
	}

	template<typename T>
	operator T() const { return T(); }
};

struct yy:public IObject
{
	float x = 55.66f;
	int y = Tester(this, "idd");

	~yy()
	{
		printf("Destroying yy\n");
	}
};

struct xx:public IObject
{
	int abc = 33;
	String str = "prived";
	Ptr<yy> yx;

	xx() {}

	xx(int aa):abc(aa) 
	{
		yx = gcnew yy();
	}

	~xx()
	{
		printf("Destroying xx\n");
	}
};

void ttt2()
{
	Ptr<xx> x = mnew xx(44);
	x.Release();
}

int main(char** lpCmdLine, int nCmdShow)
{
	ttt2();

	MemoryManager::CollectGarbage();

	TestMath();

	Application app;
	app.Launch();

	_getch();

	return 0;
}