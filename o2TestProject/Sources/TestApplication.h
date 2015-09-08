#pragma once

#include "O2.h"

using namespace o2;

class ITestAttribute: public IAttribute
{
public:
	virtual void Serialize(void* object, DataNode& data) const = 0;
	virtual void Deserialize(void* object, DataNode& data) const = 0;
};

template<typename _type>
class TestAttribute: public ITestAttribute
{
public:
	void Serialize(void* object, DataNode& data) const
	{
		*data.AddNode(mOwnerFieldInfo->Name()) = mOwnerFieldInfo->GetValue<_type>(object);
	}

	void Deserialize(void* object, DataNode& data) const
	{
		mOwnerFieldInfo->SetValue<_type>(object, (_type)(*data.GetNode(mOwnerFieldInfo->Name())));
	}
};

class TestReflection: public ISerializable
{
public:
	int    mA = 0;
	float  mB = 1.0f;
	String mC = "2";
	Vec2F  mD = Vec2F(3, 3);

	Vector<Ptr<TestReflection>> mVec;

public:
	SERIALIZABLE_IMPL(TestReflection);

	IOBJECT(TestReflection)
	{
		SRLZ_FIELD(mA);
		SRLZ_FIELD(mB);
		SRLZ_FIELD(mC);
		SRLZ_FIELD(mD);
		SRLZ_FIELD(mVec);
	}
};

class InhTest: public TestReflection
{
public:
	IOBJECT(InhTest)
	{
		BASE_CLASS(TestReflection);
	}
};

class TestApplication: public BaseApplication
{
protected:
	// Calls when application is starting
	void OnStarted();

	// Called on updating
	void OnUpdate(float dt);

	// Called on drawing
	void OnDraw();
};