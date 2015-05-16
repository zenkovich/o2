#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

CREATE_SINGLETON(MemoryManager);

struct Tester
{
	Tester(IObject* obj, const char* id, void* ptr)
	{
		printf("Field %s on %x: %x\n", id, obj, ptr);
	}

	template<typename T>
	operator T() const { return T(); }
};

struct yy :public IObject
{
	float x = 55.66f;
	int y = Tester(this, "idd", &y);

	~yy()
	{
		printf("Destroying yy\n");
	}

	SERIALIZABLE(yy)
		FIELD(x)
		FIELD(y);
};

#define XX(A, B) (String)(#A) + (String)(#B)

struct xx :public IObject
{
	int abc = 33;
	int def = 55;
	yy vy;

	SERIALIZABLE(xx)
		FIELD(abc)
		FIELD(def)
		FIELD(vy);

	Ptr<yy> yx;

	xx(int aa)
	{
		yx = gcnew yy();
		def = 5;
	}

	DataNode Serialize()
	{
		printf("Serializing xx");

		return IObject::Serialize();
	}

	~xx()
	{
		printf("Destroying xx\n");
		auto x = XX(ss, DataDoc);
	}

	void DoSome(int x)
	{
		printf("do dome: %i\n", x);
	}


	void DoSome(const String& x)
	{
		printf("do dome: %s\n", x.Data());
	}

	template<typename _type>
	void DoSome(_type& x)
	{
		printf("do dome: %s\n", x.GetStr().Data());
	}
};

void ttt2()
{
	Ptr<xx> x = mnew xx(44);
	x->abc = 0;
	x->def = 1;
	x->vy.x = 2;
	x->vy.y = 3;

	DataDoc doc = *x;
	*doc.AddNode("nd") = 55;
	int nd = *doc["nd"];

	Ptr<xx> y = mnew xx(44);
	*y = doc;

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