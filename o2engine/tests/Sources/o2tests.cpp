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
#include "Utils/Data/DataDoc.h"
#include "Dependencies/Json/JsonBox.h"
#include "Dependencies/Json2/json.hpp"

using json = nlohmann::json;

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
	Debug::Log("%s", sr.GetTypeName());
}

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	DataDoc doc;
	auto nd1 = doc.AddNode("Boroda3");
	*doc.AddNode("Boroda") = "Sosiska";
	*doc.AddNode("Boroda2") = Vec2F(3, 4);

	*(nd1->AddNode("ff0")) = 10;
	*(nd1->AddNode("ff2")) = 11;
	*(nd1->AddNode("ff3")) = 12;
	*(nd1->AddNode("ff4")) = 13;

	doc.SaveToFile("testDoc.xml");


	DataDoc doc2;
	doc2.LoadFromFile("testDoc.xml");

	std::string atat = "asdasd";

	JsonBox::Value js;
	js["atat"] = "vasd";
	js.writeToFile("testJson.json");

	json j;

	printf("All tests completed!");
	_getch();

	return 0;
}