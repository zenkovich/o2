#include "stdafx.h"
#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"

#include "Scene/Actor.h"
#include "Render/Camera.h"
#include "Utils/System/Time/Timer.h"

using namespace o2;
using namespace Editor;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);

struct MyBase
{
	std::string ass = "ass";
};

struct MySpec : public MyBase
{
	std::string penis = "penis";
};

struct MySpec2 : public MyBase
{
	std::string vagina = "vagina";
};

struct Dummy
{
	std::string asd;
};

struct myTest
{
private:
	template<typename T, typename Enable = void>
	struct testHelper
	{
		static constexpr bool isSupported = false;

		static void Test(const T& val, myTest& tt)
		{
			static_assert(false, "Unsupported");
		}
	};

public:
	template<typename T>
	void Test(const T& val)
	{ 
		testHelper<T>::Test(val, *this);
	}

	template<typename T>
	struct IsSupported
	{
		static constexpr bool value = testHelper<T>::isSupported;
	};

private:
	std::string data;
};

template<typename T>
struct myTest::testHelper<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
{
	static constexpr bool isSupported = true;

	static void Test(const T& val, myTest& tt)
	{
		tt.data = std::to_string(val);
		std::cout << tt.data << std::endl;
	}
};

template<typename T>
struct myTest::testHelper<T, typename std::enable_if<std::is_pointer<T>::value>::type>
{
	static constexpr bool isSupported = myTest::testHelper<std::remove_pointer<T>::type>::isSupported;

	static void Test(const T& val, myTest& tt)
	{
		myTest::testHelper<std::remove_pointer<T>::type>::Test(*val, tt);
	}
};

template<typename T>
struct myTest::testHelper<T, typename std::enable_if<std::is_base_of<MyBase, T>::value>::type>
{
	static constexpr bool isSupported = true;

	static void Test(const T& val, myTest& tt)
	{
		tt.data = val.ass;
		std::cout << tt.data << std::endl;
	}
};

template<>
struct myTest::testHelper<MySpec2>
{
	static constexpr bool isSupported = true;

	static void Test(const MySpec2& val, myTest& tt)
	{
		tt.data = val.vagina;
		std::cout << tt.data << std::endl;
	}
};

int main()
{
	INITIALIZE_O2;

	bool x1 = myTest::IsSupported<int>::value;
	bool x2 = myTest::IsSupported<float>::value;
	bool x3 = myTest::IsSupported<std::string>::value;
	bool x4 = myTest::IsSupported<MySpec>::value;
	bool x5 = myTest::IsSupported<MySpec2>::value;
	bool x6 = myTest::IsSupported<Dummy>::value;
	bool x7 = myTest::IsSupported<Dummy*>::value;
	bool x8 = myTest::IsSupported<MySpec2*>::value;

	myTest xx;
	xx.Test(15);
	xx.Test(MySpec());
	xx.Test(MySpec2());
	xx.Test(new MySpec2());

	EditorApplication* app = mnew EditorApplication();
	app->Initialize();
	app->Launch();
	delete app;

	return 0;
}

