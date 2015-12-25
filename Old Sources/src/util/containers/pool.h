#pragma once

#include "assert.h"
#include "util/containers/array.h"

OPEN_O2_NAMESPACE

template<typename _type>
class Pool
{
	Array<_type*> mObjects;
	int           mChunkSize;

public:
	Pool(int initialCount = 5, int chunkSize = 5);
	~Pool();

	void SetChunkSize(int chunkSize);
	int  GetChunkSize() const;

	_type* Take();
	void   Free(_type* obj);

	void CreateObjects(int count);
};

//implementation
template<typename _type>
Pool<_type>::Pool(int initialCount = 5, int chunkSize = 5):
mChunkSize(chunkSize)
{
	CreateObjects(initialCount);
}

template<typename _type>
Pool<_type>::~Pool()
{
	for (int i = 0; i < mObjects.Count(); i++)
		SafeRelease(mObjects[i]);

	mObjects.Clear();
}

template<typename _type>
void Pool<_type>::SetChunkSize(int chunkSize)
{
	mChunkSize = chunkSize;
}

template<typename _type>
int Pool<_type>::GetChunkSize() const
{
	return mChunkSize;
}

template<typename _type>
_type* Pool<_type>::Take()
{
	if (mObjects.Count() == 0)
		CreateObjects(mChunkSize);

	return mObjects.PopBack();
}

template<typename _type>
void Pool<_type>::Free(_type* obj)
{
	mObjects.Add(obj);
}

template<typename _type>
void Pool<_type>::CreateObjects(int count)
{
	for (int i = 0; i < count; i++)
		mObjects.Add(mnew _type());
}

CLOSE_O2_NAMESPACE
