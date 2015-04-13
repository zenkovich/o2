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
	int a, b;
	float d;
	String c;

	TestStruct(){}
	TestStruct(const String& f) :c(f), a(10) {}

	bool operator==(const TestStruct& other) const
	{
		return a == other.a && b == other.b && d == other.d && c == other.c;
	}
};

struct TestStructArr
{
	Array<int> arr;

	int FindMax() const
	{
		int res = 0;
		for (auto n : arr)
		{
			if (n > res)
				res = n;
		}

		return res;
	}

	void print()
	{
		printf("==========\n");
		for (auto n : arr)
			printf("%i\n", n);
	}

	template<typename T>
	void TemplTest(const Function<T(int)> fn)
	{
		int rr = (int)(fn(10));
		printf("rr = %i\n", rr);
	}
};

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	Debug::Log("Some log");
	Debug::LogWarning("Some log warning");
	Debug::LogError("Some log error");

	auto xx = FormatStr("%f", 5.4f);
	Debug::Log(xx);

	Array<TestStruct> arrTest;
	arrTest.Add(TestStruct("a"));
	arrTest.Add(TestStruct("b"));
	arrTest.Add(TestStruct("c"));
	arrTest.Add(TestStruct("d"));
	for (auto x : arrTest)
	{
		Debug::Log("%s", x.c.c_str());
	}

	Debug::Log("=============");

	FunctionsList<void(int)> tt([](int x){ printf("%i\n", x + 1); });
	auto tt2 = tt + [](int x){ printf("%i\n", x + 2); };
	auto lamb = [](int x){ printf("%i\n", x + 3); };
	tt2 += lamb;

	tt2(10);

	Debug::Log("=============");

	//tt2 -= lamb;
	tt2(10);

	TestStructArr bbc;
	bbc.arr.Add(0);
	bbc.arr.Add(5);
	bbc.arr.Add(2);
	bbc.arr.Add(8);
	bbc.arr.Add(11);
	bbc.print();
	printf("max is %i\n", bbc.FindMax());

	bbc.print();

	bbc.TemplTest<float>([](int x) { return (float)x; });


	Debug::Log("=============");
	Array<float> farr;
	farr.Add(0.1f);
	farr.Add(0.01f);
	farr.Add(0.6f);
	farr.Add(0.4f);
	farr.Add(0.1f);
	farr.Add(0.18f);
	farr.Add(2.1f);
	farr.Add(3.1f);

	farr.ForEach([](float x){ Debug::Log("%f", x); });
	Debug::Log("=============");
	farr.RemoveAll([](float x){ return x > 0.5f; });
	farr.SortBy<int>([](float x){ return (int)(x*100.0f); });
	farr.Take(3).ForEach([](float x){ Debug::Log("%f", x); });

	Debug::Log("Sum: %f", farr.Sum<float>([](float ff){ return ff; }));

	printf("All tests completed!");
	_getch();

	return 0;
}