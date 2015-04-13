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

template <typename UnusedType>
class function;


template <typename ReturnType, typename ... ArgumentTypes>
class function <ReturnType(ArgumentTypes ...)>
{
public:

	typedef ReturnType signature_type(ArgumentTypes ...);

	function()
		: mInvoker()
	{
	}


	template <typename FunctionT>

	/// Uncomment this, and add corresponding assignment operator for `check4`
	///explicit
	function(FunctionT f)
		: mInvoker(new free_function_holder<FunctionT>(f))
	{
	}

	template <typename FunctionType, typename ClassType>
	function(FunctionType ClassType::* f)
		: mInvoker(new member_function_holder<FunctionType, ArgumentTypes ...>(f))
	{
	}


	function(const function & other)
		: mInvoker(other.mInvoker->clone())
	{
	}

	function & operator = (const function & other)
	{
		mInvoker = other.mInvoker->clone();
	}

	ReturnType operator ()(ArgumentTypes ... args)
	{
		return mInvoker->invoke(args ...);
	}


private:


	class function_holder_base
	{
	public:

		function_holder_base()
		{
		}

		virtual ~function_holder_base()
		{
		}

		virtual ReturnType invoke(ArgumentTypes ... args) = 0;

		virtual std::auto_ptr<function_holder_base> clone() = 0;

	private:
		function_holder_base(const function_holder_base &);
		void operator = (const function_holder_base &);
	};


	typedef std::auto_ptr<function_holder_base> invoker_t;


	template <typename FunctionT>
	class free_function_holder : public function_holder_base
	{
	public:

		free_function_holder(FunctionT func)
			: function_holder_base(),
			mFunction(func)
		{

		}

		virtual ReturnType invoke(ArgumentTypes ... args)
		{
			return mFunction(args ...);
		}


		virtual invoker_t clone()
		{
			return invoker_t(new free_function_holder(mFunction));
		}

	private:

		FunctionT mFunction;
	};


	template <typename FunctionType, typename ClassType, typename ... RestArgumentTypes>
	class member_function_holder : public function_holder_base
	{
	public:

		typedef FunctionType ClassType::* member_function_signature_t;

		member_function_holder(member_function_signature_t f)
			: mFunction(f)
		{}

		virtual ReturnType invoke(ClassType obj, RestArgumentTypes ... restArgs)
		{
			return (obj.*mFunction)(restArgs ...);
		}

		virtual invoker_t clone()
		{
			return invoker_t(new member_function_holder(mFunction));
		}

	private:
		member_function_signature_t mFunction;
	};

	invoker_t mInvoker;
};

void smFunc(int x)
{
	printf("%i\n", x);
}

int main(char** lpCmdLine, int nCmdShow)
{
	function<void(int)> tht = smFunc;
	tht(5);

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