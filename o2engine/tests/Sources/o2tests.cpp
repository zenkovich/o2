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
};

#define XX(A, B) (String)(#A) + (String)(#B)

struct xx :public IObject
{
	int abc = SERIALIZABLE(abc);
	int def;

	//FieldsRegistrator _regi = FieldsRegistrator(this).Field(abc, "abc");
	SERIALIZABLE_FIELDS
		FIELD(abc)
		FIELD(def);

	//int abc = ClassFieldRegistrator(this, "abc", &abc, 33);
	//int SRLZ(abc);
	// SERIALIZABLE(int, abc, 33);
	//String SRLZ(str);
	Ptr<yy> yx;

	xx() {}

	xx(int aa)
	{
		yx = gcnew yy();
		def = 5;
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

	DataDoc doc = (DataNode)*x;

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