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
#include "Dependencies/Json/JsonBox.h"
#include "Dependencies/Gason/gason.h"


FIRST_SERIALIZATION();

struct TestSerialize: public Serializable
{
	int a;
	float b;
	Array<int> c;

	TestSerialize() {}

	TestSerialize(int a, float b, int c1, int c2, int c3):
		a(a), b(b)
	{
		c.Add(c1); c.Add(c2); c.Add(c3);
	}

	bool operator==(const TestSerialize& other) const
	{
		return a == other.a && b == other.b && c == other.c;
	}

	void fnc(int x)
	{
		Debug::Log("object fnc %i", a + x);
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

	DataDoc doc;
	*doc.AddNode("name") = "prived";
	doc.SaveToFile("file.xml");

	printf("All tests completed!");
	_getch();

	return 0;
}