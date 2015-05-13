#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

FIRST_SERIALIZATION();

class IPtr;
class IObject;

class ObjectsManager: public Singleton<ObjectsManager>
{
public:
	typedef Array<IObject*> ObjectsArr;

	ObjectsArr mObjects;

	static void OnObjectCreating(IObject* object)
	{
		mInstance->mObjects.Add(object);
	}

	static void OnObjectRemoving(IObject* object)
	{
		mInstance->mObjects.Remove(object);
	}

	static void OnPtrCreating(IPtr* ptr);
};

CREATE_SINGLETON(ObjectsManager);

class IObject
{
public:
	typedef Array<IPtr*> PointersArr;

	PointersArr mPointers;
	PointersArr mChildPointers;
	UInt        mSize;

	IObject()
	{
		ObjectsManager::OnObjectCreating(this);
	}

	virtual ~IObject()
	{
		ObjectsManager::OnObjectRemoving(this);
	}
};

void ObjectsManager::OnPtrCreating(IPtr* ptr)
{
	char* cptr = (char*)ptr;
	for (auto obj : mInstance->mObjects)
	{
		char* beg = (char*)obj;
		char* end = beg + obj->mSize;
		if (cptr >= beg && cptr < end)
		{
			obj->mChildPointers.Add(ptr);
			return;
		}
	}
}

class IPtr
{
protected:
public:
	IObject* mObject;

	IPtr(IObject* object = nullptr):
		mObject(object)
	{
		ObjectsManager::OnPtrCreating(this);

		if (mObject)
			mObject->mPointers.Add(this);
	}

	IPtr(const IPtr& other):
		IPtr(other.mObject)
	{}

	virtual ~IPtr()
	{
		if (mObject)
			mObject->mPointers.Remove(this);
	}

	IPtr& operator=(const IPtr& other)
	{
		if (mObject)
			mObject->mPointers.Remove(this);

		mObject = other.mObject;

		if (mObject)
			mObject->mPointers.Add(this);

		return *this;
	}

	bool IsValid() const
	{
		return mObject != nullptr;
	}

	void Release()
	{
		if (mObject)
		{
			for (auto ptr : mObject->mPointers)
				ptr->mObject = nullptr;

			delete mObject;
		}
	}
};

template<typename _type>
class XPtr: public IPtr
{
public:
	// 	XPtr(_type* object = nullptr):
	// 		IPtr(object)
	// 	{}

	XPtr():IPtr(nullptr) {}

	XPtr(const XPtr& other):
		IPtr(other)
	{}

	~XPtr()
	{}

	XPtr& operator=(const XPtr& other)
	{
		IPtr::operator=(other);
		return *this;
	}

	bool operator==(const XPtr& other) const
	{
		return mObject == other.mObject;
	}

	bool operator!=(const XPtr& other) const
	{
		return mObject != other.mObject;
	}

	operator bool() const
	{
		return mObject != nullptr;
	}

	operator _type*()
	{
		return (_type*)mObject;
	}

	operator _type* const() const
	{
		return (_type*)mObject;
	}

	_type* operator->() const
	{
		return (_type*)mObject;
	}

	_type& operator*()
	{
		return *(_type*)mObject;
	}

	_type* Get()
	{
		return (_type*)mObject;
	}
};

template<typename _type, typename ... _args>
XPtr<_type> CreateObject(_args ... args)
{
	XPtr<_type> res;
	_type* obj = new _type(args ...);
	obj->mSize = sizeof(*obj);
	res.mObject = obj;
	res.mObject->mPointers.Add(&res);
	return res;
}

struct yy:public IObject
{
	float x = 55.66f;
};

struct xx:public IObject
{
	int abc = 33;
	String str = "prived";
	XPtr<yy> yx;

	xx() {}

	xx(int aa):abc(aa) {}
};

int main(char** lpCmdLine, int nCmdShow)
{
	XPtr<xx> x = CreateObject<xx>(44);

	TestMath();

	Application app;
	app.Launch();

	_getch();

	return 0;
}