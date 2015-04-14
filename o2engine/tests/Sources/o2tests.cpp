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

	TestStruct testStruct;
	TFunction<bool(int)> func(&TestStruct::sum, testStruct);
	printf("func = %i\n", (int)func(15));

	Array<TestStruct> arr;
	arr.Add(TestStruct(1));
	arr.Add(TestStruct(2));
	arr.Add(TestStruct(4));
	arr.Add(TestStruct(6));
	arr.Add(TestStruct(10));
	arr.Add(TestStruct(-31));

	arr.RemoveAll(&[](const TestStruct& str){ return str.a < 5; });
	arr.ForEach(&[](const TestStruct& str){ printf("%i\n", str.a); });


	printf("All tests completed!");
	_getch();

	return 0;
}