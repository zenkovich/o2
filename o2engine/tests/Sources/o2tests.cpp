#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

#include "Assets/Asset.h"

INITIALIZE_O2;

struct TestSerializable: ISerializable
{
	float  mA = 1.0f;
	int    mB = 2;
	String mC = "hello kitty";
	Vec2F  mD = Vec2F::Up();

	SERIALIZABLE_IMPL(TestSerializable);

	FIELDS()
	{
		SERIALIZABLE_FIELD(mA);
		SERIALIZABLE_FIELD(mB);
		SERIALIZABLE_FIELD(mC);
		SERIALIZABLE_FIELD(mD);
	}
};

SERIALIZABLE_REG(TestSerializable);

struct InheritedTestSerializable: public TestSerializable
{
	int mE = 6;
	Vector<Ptr<TestSerializable>> mVector;

	SERIALIZABLE_IMPL(InheritedTestSerializable);

	FIELDS()
	{
		BASE_CLASS_FIELDS(TestSerializable);
		SERIALIZABLE_FIELD(mE);
		SERIALIZABLE_FIELD(mVector) ANIMATABLE(nullptr);
	}
};
SERIALIZABLE_REG(InheritedTestSerializable);

int main(char** lpCmdLine, int nCmdShow)
{
	TestSerializable test;
	DataNode node = test;

	TestSerializable it;
	it.mA = -1.0f;
	it = node;

	InheritedTestSerializable iit;
	iit.mVector.Add(mnew TestSerializable());
	iit.mVector.Add(mnew TestSerializable());
	iit.mVector.Add(mnew TestSerializable());
	node = iit;

	InheritedTestSerializable iit2;
	iit2.mE = 99;
	iit2 = node;

	DataDoc doc = node;
	doc.SaveToFile("test.xml");

	auto fields = iit.GetFields();

	TestMath();

	Application app;
	app.Launch();

	MemoryManager::CollectGarbage();

	_getch();

	return 0;
}