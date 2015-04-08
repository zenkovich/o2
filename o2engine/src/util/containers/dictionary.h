#pragma once

#include "util/assert.h"
#include "util/containers/array.h"
#include "util/containers/idictionary.h"
#include "util/containers/pair.h"
#include "util/mem_utils/alloc_operators.h"
#include "util/mem_utils/memory_manager.h"

OPEN_O2_NAMESPACE

template<typename _key_type, typename _value_type>
class Dictionary: public IDictionary<_key_type, _value_type>
{
public:
	class Iterator
	{
		Dictionary* mDictionary;
		int         mIndex;

	public:
		Iterator(Dictionary* dictionary = NULL, int index = 0);

		int Index() const;

		Iterator operator+(int offs) const;
		Iterator operator+=(int offs);

		Iterator operator-(int offs) const;
		Iterator operator-=(int offs);

		Iterator& operator++();    // ++A;
		Iterator  operator++(int); // A++;

		Iterator& operator--();    // --A;
		Iterator  operator--(int); // A--;

		bool operator>(const Iterator& itr) const;
		bool operator<(const Iterator& itr) const;

		bool operator>=(const Iterator& itr) const;
		bool operator<=(const Iterator& itr) const;

		operator bool() const;

		_key_type&   Key() const;
		_value_type& Value() const;
	};

	class ConstIterator
	{
		const Dictionary* const mDictionary;
		int               mIndex;

	public:
		ConstIterator(const Dictionary* const dictionary = NULL, int index = 0);

		int Index() const;

		ConstIterator operator+(int offs) const;
		ConstIterator operator+=(int offs);

		ConstIterator operator-(int offs) const;
		ConstIterator operator-=(int offs);

		ConstIterator& operator++();    // ++A;
		ConstIterator  operator++(int); // A++;

		ConstIterator& operator--();    // --A;
		ConstIterator  operator--(int); // A--;

		bool operator>(const ConstIterator& itr) const;
		bool operator<(const ConstIterator& itr) const;

		bool operator>=(const ConstIterator& itr) const;
		bool operator<=(const ConstIterator& itr) const;

		operator bool() const;

		const _key_type&   Key() const;
		const _value_type& Value() const;
	};

protected:
	IArray< keyValuePair >* mPairs;

public:
	Dictionary();
	Dictionary(const Dictionary& other);
	~Dictionary();

	bool operator==(const Dictionary& other) const;
	bool operator!=(const Dictionary& other) const;

	Dictionary& operator=(const Dictionary& other);

	template<typename _type>
	void SetContainerType();

	void Add(const _key_type& key, const _value_type& value);
	void Add(const keyValuePair& keyValue);

	void Remove(const _key_type& key);
	void Clear();

	bool ContainsKey(const _key_type& key) const;
	bool ContainsValue(const _value_type& value) const;
	bool Contains(const keyValuePair& keyValue) const;

	keyValuePair FindKey(const _key_type& key) const;
	keyValuePair FindValue(const _value_type& value) const;

	void Set(const _key_type& key, const _value_type& value);

	_value_type&  Get(const _key_type& key);
	keyValuePair& Get(int index) const;

	int  Count() const;
	bool IsEmpty() const;

	void Sort(bool(*compareFunc)(keyValuePair&, keyValuePair&));
	void Sort();

	Iterator Begin();
	Iterator End();

	ConstIterator Begin() const;
	ConstIterator End() const;
};

//implementation dict::iterator

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::Iterator::Iterator(Dictionary* dictionary /*= NULL*/, int index /*= 0*/):
mDictionary(dictionary), mIndex(index)
{
}

template<typename _key_type, typename _value_type>
int Dictionary<_key_type, _value_type>::Iterator::Index() const
{
	return mIndex;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator+(int offs) const
{
	return iterator(mArray, mIndex + offs);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator-(int offs) const
{
	return iterator(mArray, mIndex - offs);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator++() // ++A;
{
	mIndex++;
	return *this;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator++(int) // A++;
{
	Iterator temp = *this;
	mIndex++;
	return temp;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator--() // --A;
{
	mIndex--;
	return *this;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator--(int) // A--;		
{
	Iterator temp = *this;
	mIndex--;
	return temp;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator+=(int offs)
{
	*this = *this + offs;
	return *this;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator-=(int offs)
{
	*this = *this - offs;
	return *this;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::Iterator::operator>(const Iterator& itr) const
{
	return mIndex > itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::Iterator::operator<(const Iterator& itr) const
{
	return mIndex < itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::Iterator::operator>=(const Iterator& itr) const
{
	return mIndex >= itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::Iterator::operator<=(const Iterator& itr) const
{
	return mIndex <= itr->mIndex;
}

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::Iterator::operator bool() const
{
	return mIndex >= 0 && mIndex < mDictionary->Count();
}

template<typename _key_type, typename _value_type>
_key_type& Dictionary<_key_type, _value_type>::Iterator::Key() const
{
	return mDictionary->Get(mIndex).mFirst;
}

template<typename _key_type, typename _value_type>
_value_type& Dictionary<_key_type, _value_type>::Iterator::Value() const
{
	return mDictionary->Get(mIndex).mSecond;
}


//implementation dict::ConstIterator

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::ConstIterator::ConstIterator(const Dictionary* const dictionary /*= NULL*/, int index /*= 0*/):
mDictionary(dictionary), mIndex(index)
{
}

template<typename _key_type, typename _value_type>
int Dictionary<_key_type, _value_type>::ConstIterator::Index() const
{
	return mIndex;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator+(int offs) const
{
	return ConstIterator(mArray, mIndex + offs);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator-(int offs) const
{
	return ConstIterator(mArray, mIndex - offs);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator++() // ++A;
{
	mIndex++;
	return *this;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator++(int) // A++;
{
	ConstIterator temp = *this;
	mIndex++;
	return temp;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator--() // --A;
{
	mIndex--;
	return *this;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator--(int) // A--;		
{
	ConstIterator temp = *this;
	mIndex--;
	return temp;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator+=(int offs)
{
	*this = *this + offs;
	return *this;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator-=(int offs)
{
	*this = *this - offs;
	return *this;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::ConstIterator::operator>(const ConstIterator& itr) const
{
	return mIndex > itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::ConstIterator::operator<(const ConstIterator& itr) const
{
	return mIndex < itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::ConstIterator::operator>=(const ConstIterator& itr) const
{
	return mIndex >= itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::ConstIterator::operator<=(const ConstIterator& itr) const
{
	return mIndex <= itr->mIndex;
}

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::ConstIterator::operator bool() const
{
	return mIndex >= 0 && mIndex < mDictionary->Count();
}

template<typename _key_type, typename _value_type>
const _key_type& Dictionary<_key_type, _value_type>::ConstIterator::Key() const
{
	return mDictionary->Get(mIndex).mFirst;
}

template<typename _key_type, typename _value_type>
const _value_type& Dictionary<_key_type, _value_type>::ConstIterator::Value() const
{
	return mDictionary->Get(mIndex).mSecond;
}

//implementation Dictionary
template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::Dictionary()
{
	mPairs = mnew Array< keyValuePair >();
}

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::Dictionary(const Dictionary<_key_type, _value_type>& other)
{
	mPairs = other.mPairs->Clone();
}

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>::~Dictionary()
{
	SafeRelease(mPairs);
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::operator==(const Dictionary& other) const
{
	if (Count() != other.Count())
		return false;

	foreach(*mPairs, kv)
	if (*kv != other.mPairs[kv.Index()])
		return false;

	return true;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::operator!=(const Dictionary& other) const
{
	if (Count() == other.Count())
		return false;

	foreach(*mPairs, kv)
	if (*kv != other.mPairs[kv.Index()])
		return true;

	return false;
}

template<typename _key_type, typename _value_type>
Dictionary<_key_type, _value_type>& Dictionary<_key_type, _value_type>::operator=(const Dictionary& other)
{
	SafeRelease(mPairs);
	mPairs = other.mPairs->Clone();
	return *this;
}

template<typename _key_type, typename _value_type>
template<typename _type>
void Dictionary<_key_type, _value_type>::SetContainerType()
{
	IArray* tmp = mPairs->Clone();
	SafeRelease(mPairs);
	mPairs = mnew _type(tmp);
	SafeRelease(tmp);
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Add(const _key_type& key, const _value_type& value)
{
	mPairs->Add(keyValuePair(key, value));
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Add(const keyValuePair& keyValue)
{
	mPairs->Add(keyValue);
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Remove(const _key_type& key)
{
	int idx = -1;
	foreach(*mPairs, kv)
	{
		if (kv->mFirst == key)
		{
			idx = kv.Index();
			break;
		}
	}

	if (idx >= 0)
		mPairs->Remove(idx);
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Clear()
{
	mPairs->Clear();
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::ContainsKey(const _key_type& key) const
{
	foreach(*mPairs, kv)
	if (kv->mFirst == key)
		return true;

	return false;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::ContainsValue(const _value_type& value) const
{
	foreach(*mPairs, kv)
	if (kv->mSecond == value)
		return true;

	return false;
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::Contains(const keyValuePair& keyValue) const
{
	foreach(*mPairs, kv)
	if (*kv == keyValue)
		return true;

	return false;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::keyValuePair Dictionary<_key_type, _value_type>::FindKey(const _key_type& key) const
{
	foreach(*mPairs, kv)
	if (kv->mFirst == key)
		return *kv;

	return keyValuePair();
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::keyValuePair Dictionary<_key_type, _value_type>::FindValue(const _value_type& value) const
{
	foreach(*mPairs, kv)
	if (kv->mSecond == value)
		return *kv;

	return keyValuePair();
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Set(const _key_type& key, const _value_type& value)
{
	foreach(*mPairs, kv)
	{
		if (kv->mFirst == key)
		{
			kv->mSecond = value;
			break;
		}
	}
}

template<typename _key_type, typename _value_type>
_value_type& Dictionary<_key_type, _value_type>::Get(const _key_type& key)
{
	foreach(*mPairs, kv)
	if (kv->mFirst == key)
		return kv->mSecond;

	assert(false, "Failed to get value from dictionary: not found key");

	return mPairs->Get(0).mSecond;
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::keyValuePair& Dictionary<_key_type, _value_type>::Get(int index) const
{
	return mPairs->Get(index);
}

template<typename _key_type, typename _value_type>
int Dictionary<_key_type, _value_type>::Count() const
{
	return mPairs->Count();
}

template<typename _key_type, typename _value_type>
bool Dictionary<_key_type, _value_type>::IsEmpty() const
{
	return Count() == 0;
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Sort(bool(*compareFunc)(keyValuePair&, keyValuePair&))
{
	mPairs->Sort(compareFunc);
}

template<typename _key_type, typename _value_type>
void Dictionary<_key_type, _value_type>::Sort()
{
	mPairs->Sort();
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Begin()
{
	return Iterator(this, 0);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::End()
{
	return Iterator(this, Count() - 1);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::Begin() const
{
	return ConstIterator(this, 0);
}

template<typename _key_type, typename _value_type>
typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::End() const
{
	return ConstIterator(this, Count() - 1);
}

CLOSE_O2_NAMESPACE
