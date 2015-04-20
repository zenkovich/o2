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

struct TestStruct
{
	int a;

	TestStruct(int _a = 10):a(_a) {}

	bool operator==(const TestStruct& other) const
	{
		return a == other.a;
	}

	bool sum(int x) { printf("sum = %i\n", a + x); return a > x; }
	int mul(int x) { printf("mul = %i\n", a*x); return a*x; }
};

void smFunc(int x)
{
	printf("out %i\n", x);
}

void printDictionary(Dictionary<String, TestStruct>& testDictionary)
{
	testDictionary.ForEach(&[](KeyValuePair<String, TestStruct>& t) { Debug::Log("key: %s value:%i", t.Key().c_str(), t.Value().a); });
}

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	Vec2F vv(5, 4);
	
	Dictionary<String, TestStruct> testDictionary;
	testDictionary.Add("atata", TestStruct(1));
	testDictionary.Add("atata2", TestStruct(2));
	testDictionary.Add("atata3", TestStruct(3));
	testDictionary.Add("atata4", TestStruct(4));
	testDictionary.Add(KeyValuePair<String, TestStruct>("atatss", TestStruct(5)));

	for (auto kv:testDictionary)
	{
		Debug::Log("key: %s value:%i", kv.Key().c_str(), kv.Value().a);
	}

	if (testDictionary.Any(&[](const KeyValuePair<String, TestStruct>& t) { return t.mValue.a > 3; }))
		Debug::Log("True");
	else
		Debug::Log("False");
	
	Debug::Log("x = %i", testDictionary.Count(&[](const KeyValuePair<String, TestStruct>& t) { return t.mValue.a > 3; }));

	auto kvf = testDictionary.FindAll(&[](const KeyValuePair<String, TestStruct>& t) { return t.mValue.a > 3; });

	TFunction<void(Dictionary<String, TestStruct>&)> fnc = &printDictionary;
	fnc(kvf);

	printf("All tests completed!");
	_getch();

	return 0;
}