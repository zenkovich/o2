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
#include "Utils/Xml.h"
#include "Utils/Serialization.h"

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
	Debug::Log("%s", sr.GetTypeName().c_str());
}

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	Xml::Document xmlDoc;
	Array<TestSerialize> arr;
	arr.Add(TestSerialize(1, 1.1f, 1, 2, 3));
	arr.Add(TestSerialize(2, 2.1f, 2, 2, 3));
	arr.Add(TestSerialize(3, 3.1f, 3, 2, 3));
	//Xml::ToXmlNode(arr, xmlDoc.append_child("ch"));
	auto f = TestSerialize(1, 1.1f, 1, 2, 3);
	srTest(f);

	printf("All tests completed!");
	_getch();

	return 0;
}