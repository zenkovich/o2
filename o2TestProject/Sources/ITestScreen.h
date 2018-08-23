#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"

using namespace o2;

class TestApplication;

class Test: public IObject
{
public:
	IOBJECT(Test);
};

class ITestScreen
{
public:
	ITestScreen(TestApplication* application):mApplication(application) {}
	virtual ~ITestScreen() {}

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual String GetId() const = 0;

protected:
	TestApplication* mApplication;
};
typedef Vector<ITestScreen*> TestScreensVec;

CLASS_BASES_META(Test)
{
	BASE_CLASS(IObject);
}
END_META;
CLASS_FIELDS_META(Test)
{
}
END_META;
CLASS_METHODS_META(Test)
{
}
END_META;
