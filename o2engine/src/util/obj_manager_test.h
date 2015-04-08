#pragma once

#include "public.h"
#include "singleton.h"

OPEN_O2_NAMESPACE

class IObjectHandle
{
	friend class Object;

protected:
	bool mValid;
};

class Object
{
	friend class ObjectsManager;

protected:
	typedef Array<IObjectHandle*> HandlesArr;

	int        mHandleIdx;
	HandlesArr mHandles;

	virtual ~Object();

	void RegHandle(IObjectHandle* handle);
	void UnregHandle(IObjectHandle* handle);
	void ForceDestroy();
};

template<typename _type>
class ObjectHandle: public IObjectHandle
{
protected:
	_type* mObject;

	ObjectHandle(_type* object);
	ObjectHandle(const ObjectHandle& objHandle);
	virtual ~ObjectHandle();

	ObjectHandle& operator=(const ObjectHandle& objHandle);

public:
	void ForceDestroy();
	bool IsValid();
};

class ObjectsManager: public Singleton<ObjectsManager>
{
	friend class Object;

	typedef Array<Object*> ObjectsArr;
	typedef Array<int> IntArr;

	ObjectsArr mObjects;
	IntArr     mFreeHandles;
	ObjectsArr mReleasedObjects;

public:
	ObjectsManager();
	~ObjectsManager();

private:
	int  AddObject(Object* object);
	void RemoveObject(int handleIdx);
	void ProcessObjectsReleasing();
	void CreateFreeHandles(int count);

public:
	Object* GetObjectByHandleIdx(int handleIdx);
};

CLOSE_O2_NAMESPACE
