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
	Debug::Log("%s", sr.GetTypeName());
}

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	Xml::Document xmlDoc;
	Array<TestSerialize> arr;
	arr.Add(TestSerialize(1, 1.1f, 1, 2, 3));
	arr.Add(TestSerialize(2, 2.1f, 2, 2, 3));
	arr.Add(TestSerialize(3, 3.1f, 3, 2, 3));
	Xml::SaveToFile("xmlTest.xml", xmlDoc);

	auto f = TestSerialize(1, 1.1f, 1, 2, 3);

	Serializer sr(xmlDoc.append_child("ch"));
	sr.Serialize(&f, "f");
	sr.Save("serTest.xml");

	String testStr("String test");
	String str = "fhasjdf123";
	String str2 = (String)-1234567890;
	String str3 = (String)-12.33f;
	String str7 = (String)-12.0f;
	String str4 = (String)Color4(255, 0, 128, 45);
	String str5 = (String)RectI(255, 0, 128, 45);
	String str6 = (String)RectF(255, 0, 128, 45);
	String str8 = (String)Vec2F(255, 0);
	String str9 = (String)Vec2I(255, 0);

	String ss = "0123456789";
	auto xx = ss.SubStr(3, 6);

	int strInt = (int)str2;
	float strFloat = (float)str3;
	Color4 strColor = (Color4)str4;
	RectI strRectI = (RectI)str5;
	RectF strRectF = (RectF)str6;
	Vec2F strVec2F = (Vec2F)str8;
	Vec2I strVec2I = (Vec2I)str9;
	RectF strRectF2 = (RectF)(String)"  255;34 ;  55 ; 55 ";

	auto frmt = String::Format("int %i; float %f; char %c; char* %s; String %ts; Vec2I %vi; Vec2F %vf; RectI %ri; RectF %rf; Color %cl",
		-1234567890, -33.66f, 'k', "cha cha cha", (String)"T STRING", Vec2I(1, 2), Vec2F(3, 4), RectI(1, 2, 3, 4), RectF(5, 6, 7, 8), Color4(0, 128, 256, 44));

	printf("All tests completed!");
	_getch();

	return 0;
}