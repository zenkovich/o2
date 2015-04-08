#include "obj_manager_test.h"

#include "assert.h"

OPEN_O2_NAMESPACE

CREATE_SINGLETON(ObjectsManager);

Object::~Object()
{
	foreach(mHandles, handleIt)
		(*handleIt)->mValid = false;
}

void Object::RegHandle(IObjectHandle* handle)
{
	mHandles.Add(handle);
}

void Object::UnregHandle(IObjectHandle* handle)
{
	mHandles.Remove(handle);
	if (mHandles.Count() == 0)
		ObjectsManager::Instance().RemoveObject(mHandleIdx);
}

void Object::ForceDestroy()
{
	foreach(mHandles, handleIt)
		(*handleIt)->mValid = false;

	ObjectsManager::Instance().RemoveObject(mHandleIdx);
}


ObjectsManager::ObjectsManager():
mObjects(1024), mFreeHandles(1024), mReleasedObjects(512)
{
	CreateFreeHandles(512);
}

ObjectsManager::~ObjectsManager()
{
	ProcessObjectsReleasing();

	int unremovedObjectsCount = 0;
	foreach(mObjects, objIt)
	{
		if (*objIt == NULL)
			continue;

		unremovedObjectsCount++;
		SafeRelease(*objIt);
	}

	HightLog("Found %i unremoved objects", unremovedObjectsCount);
}

void ObjectsManager::CreateFreeHandles(int count)
{
	int lastObjCount = mObjects.Count();
	int lastHandlCount = mFreeHandles.Count();

	mObjects.Resize(lastObjCount + count);
	mFreeHandles.Resize(lastHandlCount + count);

	for (int i = 0; i < count; i++)
	{
		int hdx = lastObjCount + i;
		mObjects[hdx] = NULL;
		mFreeHandles[lastHandlCount + i] = hdx;
	}
}

int ObjectsManager::AddObject(Object* object)
{
	if (mFreeHandles.Count() == 0)
		CreateFreeHandles(512);

	int handleIdx = mFreeHandles.PopBack();
	mObjects[handleIdx] = object;
	object->mHandleIdx = handleIdx;

	return handleIdx;
}

void ObjectsManager::RemoveObject(int handleIdx)
{
	mReleasedObjects.Add(mObjects[handleIdx]);
	mObjects[handleIdx] = NULL;
	mFreeHandles.Add(handleIdx);
}

Object* ObjectsManager::GetObjectByHandleIdx(int handleIdx)
{
	return mObjects[handleIdx];
}

void ObjectsManager::ProcessObjectsReleasing()
{
	foreach(mReleasedObjects, objIt)
		SafeRelease(*objIt);

	mReleasedObjects.Clear();
}

CLOSE_O2_NAMESPACE