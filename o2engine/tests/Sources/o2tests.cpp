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

	TestStruct():a(10) {}

	void sum(int x) { printf("sum = %i\n", a + x); }
	void mul(int x) { printf("mul = %i\n", a*x); }
};

void smFunc(int x)
{
	printf("out %i\n", x);
}

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	TestStruct testStruct;
	auto lambda = [](int x) { printf("lambda %i\n", x); };
	TFunction<void(int)> func(smFunc);
	func.Add(&TestStruct::mul, testStruct);
	func.Add(&TestStruct::sum, testStruct);
	func.Add(&lambda);
	func(3);

	printf("==========\n");
	TFunction<void(int)> func2(smFunc);
	func2.Add(&TestStruct::mul, testStruct);
	func2.Remove(smFunc);
	func.Remove(func2);
	func(3);

	printf("==========\n");
	func = TFunction<void(int)>(&TestStruct::mul, testStruct);
	func(3);

	printf("==========\n");
	TFunction<void(int)> func3(func);
	func3 += &lambda;
	func3(3);

	printf("==========\n");
	func3 -= &lambda;
	func3(3);

	func3 = smFunc;

	if (func3 == smFunc)
		printf("Equals\n");

	printf("==========\n");
	TFunction<void(int)> func4(smFunc);
	func4.Add(&TestStruct::mul, testStruct);
	func4.Add(&TestStruct::sum, testStruct);
	func4.Add(&lambda);
	func4(3);

	if (func4.Contains(smFunc))
		printf("Contains smFunc\n");

	if (func4.Contains(&TestStruct::mul, testStruct))
		printf("Contains &TestStruct::mul, testStruct\n");

	if (func4.Contains(&lambda))
		printf("Contains lambda\n");

	func4 -= smFunc;

	if (func4.Contains(func3 + &lambda))
		printf("Contains &lambda + smFunc\n");


	printf("All tests completed!");
	_getch();

	return 0;
}