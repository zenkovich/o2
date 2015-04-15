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

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();
	
	Dictionary<String, TestStruct> testDictionary;
	testDictionary.Add("atata", TestStruct(1));
	testDictionary.Add("atata2", TestStruct(2));
	testDictionary.Add("atata3", TestStruct(3));
	testDictionary.Add("atata4", TestStruct(4));

	printf("All tests completed!");
	_getch();

	return 0;
}