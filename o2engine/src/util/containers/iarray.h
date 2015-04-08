#pragma once

#include "engine_settings.h"
#include "util/public_namespace.h"
#include "util/assert.h"
#include "util/compare.h"

OPEN_O2_NAMESPACE

/** Array interface. */
template<typename _type>
class IArray
{
public:
	virtual IArray* Clone() const = 0;

	virtual _type& Get(int idx) const = 0;
	virtual void   Set(int idx, const _type& value) = 0;

	virtual int Count() const = 0;

	virtual void Resize(int newCount) = 0;

	virtual _type& Add(const _type& value) = 0;
	virtual void   Add(const IArray& arr) = 0;

	virtual _type& Insert(const _type& value, int position) = 0;
	virtual void   Insert(const IArray& arr, int position) = 0;

	virtual _type PopBack() = 0;

	virtual bool RemoveAt(int idx) = 0;
	virtual bool RemoveRange(int begin, int end) = 0;
	virtual bool Remove(const _type& value) = 0;

	virtual void Clear() = 0;

	virtual bool Contains(const _type& value) const = 0;
	virtual int  Find(const _type& value) const = 0;

	virtual void Sort(bool(*compareFunc)(_type&, _type&) = &CompareLess) = 0;

	_type& operator[](int idx);

	virtual _type& First();
	virtual _type& Last();

	virtual bool IsEmpty() const;
};


template<typename _type>
_type& IArray<_type>::First()
{
	return Get(0);
}

template<typename _type>
_type& IArray<_type>::Last()
{
	return Get(Count() - 1);
}

template<typename _type>
_type& IArray<_type>::operator[](int idx)
{
	return Get(idx);
}

template<typename _type>
bool IArray<_type>::IsEmpty() const
{
	return Count() == 0;
}

CLOSE_O2_NAMESPACE
