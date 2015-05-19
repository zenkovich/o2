#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

INITIALIZE_O2;

struct Tester
{
	Tester(IObject* obj, const char* id, void* ptr)
	{
		printf("Field %s on %x: %x\n", id, obj, ptr);
	}

	template<typename T>
	operator T() const { return T(); }
};

struct yy :public IObject, public Serializable
{
	float x = 55.66f;

	~yy()
	{
		printf("Destroying yy\n");
	}

	bool operator==(const yy& other)
	{
		return Math::Equals(x, other.x);
	}

	SERIALIZABLE_FIELDS(yy)
		FIELD(x);
	END_SERIALIZABLE_FIELDS;
};

#define XX(A, B) (String)(#A) + (String)(#B)

struct xx :public IObject, public Serializable
{
	int abc = 33;
	int def = 55;
	yy vy;
	Ptr<yy> yx;
	Array<yy> yarr;

	SERIALIZABLE_FIELDS(xx)
		FIELD(abc)
		FIELD(def)
		FIELD(vy)
		FIELD(yx)
		FIELD(yarr);
	END_SERIALIZABLE_FIELDS;


	xx(int aa = 0)
	{
		yx = gcnew yy();
		def = 5;

		yarr.Add(yy());
		yarr.Add(yy());
		yarr.Add(yy());
		yarr.Add(yy());
	}

	DataNode Serialize()
	{
		printf("Serializing xx");

		return Serializable::Serialize();
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

SERIALIZABLE_REG(xx);
SERIALIZABLE_REG(yy);

void ttt2()
{
	Ptr<xx> x = mnew xx(44);
	x->abc = 0;
	x->def = 1;
	x->vy.x = 2;

	DataDoc doc = *x;
// 	*doc.AddNode("nd") = 55;
// 	int nd = *doc["nd"];

	Ptr<xx> y = mnew xx(44);
	y->yarr.Clear();
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