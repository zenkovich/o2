#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include <functional>
#include <xfunctional>

#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Debug.h"
#include "Utils/Containers/Array.h"
#include "Utils/Function.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Serialization.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/SmartPointers.h"
#include "Utils/Property.h"
#include "Dependencies/Json/JsonBox.h"
#include "Dependencies/Gason/gason.h"


FIRST_SERIALIZATION();

struct TestSerialize: public Serializable, public PtrBase<TestSerialize>
{
	int a;
	float b;
	Array<int> c;
	Vec2F d;

	Property<TestSerialize, int> ap;
	Property<TestSerialize, Vec2F> dp;

	TestSerialize() 
	{
		InitProps();
	}

	TestSerialize(int a, float b, int c1, int c2, int c3):
		a(a), b(b)
	{
		c.Add(c1); c.Add(c2); c.Add(c3);

		InitProps();
	}

	bool operator==(const TestSerialize& other) const
	{
		return a == other.a && b == other.b && c == other.c;
	}

	void SetA(int v)
	{
		a = v;
	}

	int GetA() const
	{
		return a;
	}

	void SetD(const Vec2F& v)
	{
		d = v;
	}

	Vec2F GetD() const
	{
		return d;
	}

	void fnc(int x)
	{
		Debug::Log("object fnc %i", a + x);
	}

	void InitProps()
	{
		INITIALIZE_PROPERTY(TestSerialize, ap, SetA, GetA);
		INITIALIZE_PROPERTY(TestSerialize, dp, SetD, GetD);
	}

	SERIALIZE_METHODS(TestSerialize);
};

SERIALIZE_METHOD_IMPL(TestSerialize)
{
	SERIALIZE(a);
	SERIALIZE(b);
	SERIALIZE(c);

	return true;
}

void srTest(Serializable& sr)
{
	Debug::Log("%s", sr.GetTypeName());
}

void tst(int x)
{
	Debug::Log("static int:%i", x);
}

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	TestSerialize tt(1, 2, 3, 4, 5);

	Serializer srl;
	srl.Serialize(&tt, "tt");
	srl.Save("testSerialize.xml");

	Debug::LogWarning("integer %f", 45.3f);

// 	DataDoc doc;
// 	*doc.AddNode("name") = L"prived";
// 	doc.SaveToFile("file.xml");

	DataDoc readDoc("file.xml");

	Ptr<TestSerialize> ptest(new TestSerialize(1, 2, 3, 4, 5));
	Ptr<TestSerialize> p2test(ptest, true);

	int f = tt.ap;
	tt.ap = f;
	tt.dp = Vec2F(1, 3);

	printf("All tests completed!");
	_getch();

	return 0;
}