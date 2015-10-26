#pragma once

#include "O2.h"

using namespace o2;

class TestApplication;

class ITestScreen
{
public:
	ITestScreen(Ptr<TestApplication> application):mApplication(application) {}
	virtual ~ITestScreen() {}

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual String GetId() const = 0;

protected:
	Ptr<TestApplication> mApplication;
};
typedef Vector<Ptr<ITestScreen>> TestScreensVec;