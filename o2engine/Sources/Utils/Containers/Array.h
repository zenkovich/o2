#pragma once

#include "Utils/Containers/IArray.h"
#include "Utils/Assert.h"

#ifndef CONTAINERS_DEBUG
#define CONTAINERS_DEBUG true
#endif 


namespace o2
{
	template<typename _type>
	class Array: public IArray<_type>
	{
	public:
		class Iterator
		{
			friend class Array<_type>;

			_type*       mValuePtr;
			const Array& mArray;

			Iterator(const Array& arr, _type* valuePtr);

		public:
			int       Index() const;
			_type&    Value();
			bool      IsValid() const;

			Iterator  operator+(int offs) const;
			Iterator  operator-(int offs) const;

			Iterator& operator++();
			Iterator  operator++(int);
			Iterator& operator--();
			Iterator  operator--(int);

			Iterator&  operator+=(int offs);
			Iterator&  operator-=(int offs);

			bool operator>(const Iterator& itr) const;
			bool operator<(const Iterator& itr) const;
			bool operator>=(const Iterator& itr) const;
			bool operator<=(const Iterator& itr) const;

			operator bool() const;

			_type* operator->();
			_type& operator*();
		};

		class ConstIterator
		{
			friend class Array<_type>;

			_type*       mValuePtr;
			const Array& mArray;

			ConstIterator(const Array& arr, _type* valuePtr);

		public:
			int          Index() const;
			const _type& Value() const;
			bool         IsValid() const;

			ConstIterator  operator+(int offs) const;
			ConstIterator  operator-(int offs) const;

			ConstIterator& operator++();
			ConstIterator  operator++(int);
			ConstIterator& operator--();
			ConstIterator  operator--(int);

			ConstIterator&  operator+=(int offs);
			ConstIterator&  operator-=(int offs);

			bool operator>(const ConstIterator& itr) const;
			bool operator<(const ConstIterator& itr) const;
			bool operator>=(const ConstIterator& itr) const;
			bool operator<=(const ConstIterator& itr) const;

			operator bool() const;

			const _type* const operator->();
			const _type&       operator*();
		};

	protected:
		(_type*)mValues;
		int     mCount;
		int     mCapacity;

	public:
		Array(int capacity = 5);
		Array(const Array& arr);
		Array(const IArray<_type>* arr);
		virtual ~Array();

		Array& operator=(const Array& arr);

		Array operator+(const Array& arr) const;
		Array operator+=(const Array& arr);

		bool operator==(const Array& arr);
		bool operator!=(const Array& arr);

		IArray* Clone() const;

		int Count() const;
		int Capacity() const;

		void Resize(int newCount);
		void Reserve(int newCapacity);

		_type& Get(int idx) const;
		void   Set(int idx, const _type& value);

		_type& Add(const _type& value);
		void   Add(const IArray& arr);

		_type& Insert(const _type& value, int position);
		void   Insert(const IArray& arr, int position);

		int  Find(const _type& value) const;
		bool Contains(const _type& value) const;

		_type PopBack();

		bool RemoveAt(int idx);
		bool RemoveRange(int begin, int end);
		bool Remove(const _type& value);
		Iterator Remove(const Iterator& it);

		void Clear();

		void Sort(const TFunction<bool(const _type&, const _type&)> pred = Math::Fewer);

		Array<_type> FindAll(const TFunction<bool(const _type&)> match) const;
		Array<_type> Where(const TFunction<bool(const _type&)> match) const;

		template<typename _sel_type>
		Array<_sel_type> Select(const TFunction<_sel_type(const _type&)> selector) const;

		Array<_type> Take(int count) const; 

		Iterator Begin();
		Iterator End();

		ConstIterator Begin() const;
		ConstIterator End() const;

		Iterator begin() { return Begin(); }
		Iterator end() { return End(); }

		ConstIterator begin() const { return Begin(); }
		ConstIterator end() const { return End(); }

	protected:
		int GetReservingSize(int size);

		void QuickSort(const TFunction<bool(const _type&, const _type&)> pred, int left, int right);
	};

	//implementation array::iterator
	template<typename _type>
	Array<_type>::Iterator::Iterator(const Array<_type>& arr, _type* valuePtr):
		mValuePtr(valuePtr), mArray(arr)
	{
	}

	template<typename _type>
	int Array<_type>::Iterator::Index() const
	{
		return mValuePtr - mArray.mValues;
	}

	template<typename _type>
	bool Array<_type>::Iterator::IsValid() const
	{
		return !(mValuePtr < mArray.mValues || mValuePtr >= mArray.mValues + mArray.mCount);
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::Iterator::operator+(int offs) const
	{
		return iterator(mArray, mValuePtr + offs);
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::Iterator::operator-(int offs) const
	{
		return iterator(mArray, mValuePtr - offs);
	}

	template<typename _type>
	typename Array<_type>::Iterator& Array<_type>::Iterator::operator++()
	{
		mValuePtr++;
		return *this;
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		mValuePtr++;
		return temp;
	}

	template<typename _type>
	typename Array<_type>::Iterator& Array<_type>::Iterator::operator--()
	{
		mValuePtr--;
		return *this;
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		mValuePtr--;
		return temp;
	}

	template<typename _type>
	typename Array<_type>::Iterator& Array<_type>::Iterator::operator+=(int offs)
	{
		mValuePtr += offs;
		return *this;
	}

	template<typename _type>
	typename Array<_type>::Iterator& Array<_type>::Iterator::operator-=(int offs)
	{
		mValuePtr -= offs;
		return *this;
	}

	template<typename _type>
	bool Array<_type>::Iterator::operator>(const Iterator& itr) const
	{
		return mValuePtr > itr->mValuePtr;
	}

	template<typename _type>
	bool Array<_type>::Iterator::operator<(const Iterator& itr) const
	{
		return mValuePtr < itr->mValuePtr;
	}

	template<typename _type>
	bool Array<_type>::Iterator::operator>=(const Iterator& itr) const
	{
		return mValuePtr >= itr->mValuePtr;
	}

	template<typename _type>
	bool Array<_type>::Iterator::operator<=(const Iterator& itr) const
	{
		return mValuePtr <= itr->mValuePtr;
	}

	template<typename _type>
	Array<_type>::Iterator::operator bool() const
	{
		return IsValid();
	}

	template<typename _type>
	_type* Array<_type>::Iterator::operator->()
	{
		return &Value();
	}

	template<typename _type>
	_type& Array<_type>::Iterator::operator*()
	{
		return Value();
	}

	template<typename _type>
	_type& Array<_type>::Iterator::Value()
	{
		if (CONTAINERS_DEBUG)
			Assert(mValuePtr >= mArray.mValues && mValuePtr <= mArray.mValues + mArray.mCount, "Array iterator is out of range");

		return *mValuePtr;
	}


	//implementation Array::ConstIterator

	template<typename _type>
	Array<_type>::ConstIterator::ConstIterator(const Array<_type>& arr, _type* valuePtr):
		mArray(arr), mValuePtr(valuePtr)
	{
	}

	template<typename _type>
	int Array<_type>::ConstIterator::Index() const
	{
		return mValuePtr - mArray.mValues;
	}

	template<typename _type>
	bool Array<_type>::ConstIterator::IsValid() const
	{
		return !(mValuePtr < mArray.mValues || mValuePtr >= mArray.mValues + mArray.mCount);
	}

	template<typename _type>
	typename Array<_type>::ConstIterator Array<_type>::ConstIterator::operator+(int offs) const
	{
		return ConstIterator(mArray, mValuePtr + offs);
	}

	template<typename _type>
	typename Array<_type>::ConstIterator Array<_type>::ConstIterator::operator-(int offs) const
	{
		return ConstIterator(mArray, mValuePtr - offs);
	}

	template<typename _type>
	typename Array<_type>::ConstIterator& Array<_type>::ConstIterator::operator++()
	{
		mValuePtr++;
		return *this;
	}

	template<typename _type>
	typename Array<_type>::ConstIterator Array<_type>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		mValuePtr++;
		return temp;
	}

	template<typename _type>
	typename Array<_type>::ConstIterator& Array<_type>::ConstIterator::operator--()
	{
		mValuePtr--;
		return *this;
	}

	template<typename _type>
	typename Array<_type>::ConstIterator Array<_type>::ConstIterator::operator--(int)
	{
		ConstIterator temp = *this;
		mValuePtr--;
		return temp;
	}

	template<typename _type>
	typename Array<_type>::ConstIterator& Array<_type>::ConstIterator::operator+=(int offs)
	{
		mValuePtr += offs;
		return *this;
	}

	template<typename _type>
	typename Array<_type>::ConstIterator& Array<_type>::ConstIterator::operator-=(int offs)
	{
		mValuePtr -= offs;
		return *this;
	}

	template<typename _type>
	bool Array<_type>::ConstIterator::operator>(const ConstIterator& itr) const
	{
		return mValuePtr > itr->mValuePtr;
	}

	template<typename _type>
	bool Array<_type>::ConstIterator::operator<(const ConstIterator& itr) const
	{
		return mValuePtr < itr->mValuePtr;
	}

	template<typename _type>
	bool Array<_type>::ConstIterator::operator>=(const ConstIterator& itr) const
	{
		return mValuePtr >= itr->mValuePtr;
	}

	template<typename _type>
	bool Array<_type>::ConstIterator::operator<=(const ConstIterator& itr) const
	{
		return mValuePtr <= itr->mValuePtr;
	}

	template<typename _type>
	Array<_type>::ConstIterator::operator bool() const
	{
		return IsValid();
	}

	template<typename _type>
	const _type* const Array<_type>::ConstIterator::operator->()
	{
		return &Value();
	}

	template<typename _type>
	const _type& Array<_type>::ConstIterator::operator*()
	{
		return Value();
	}

	template<typename _type>
	const _type& Array<_type>::ConstIterator::Value() const
	{
		if (CONTAINERS_DEBUG)
			Assert(mValuePtr >= mArray.mValues && mValuePtr <= mArray.mValues + mArray.mCount, "Array iterator is out of range");

		return *mValuePtr;
	}


	//implementation Array

	//TODO check all functions

	template<typename _type>
	Array<_type>::Array(int capacity /*= 5*/)
	{
		if (CONTAINERS_DEBUG)
			Assert(capacity > 0, "Can't initialize array with empty capacity");

		mValues = (_type*)malloc(sizeof(_type)*capacity);
		mCapacity = capacity;
		mCount = 0;
	}

	template<typename _type>
	Array<_type>::Array(const Array& arr)
	{
		mValues = (_type*)malloc(sizeof(_type)*arr.mCapacity);
		mCapacity = arr.mCapacity;
		mCount = arr.mCount;

		for (int i = 0; i < mCount; i++)
			new (mValues + i) _type(arr.mValues[i]);
	}

	template<typename _type>
	Array<_type>::Array(const IArray<_type>* arr)
	{
		mCount = arr->Count();
		mCapacity = GetReservingSize(mCount);
		mValues = (_type*)malloc(sizeof(_type)*mCapacity);

		for (int i = 0; i < mCount; i++)
			new (mValues + i) _type(arr->Get(i));
	}

	template<typename _type>
	Array<_type>::~Array()
	{
		Clear();
		free(mValues);
	}

	template<typename _type>
	IArray<_type>* Array<_type>::Clone() const
	{
		return new Array<_type>(this);
	}

	template<typename _type>
	Array<_type>& Array<_type>::operator=(const Array<_type>& arr)
	{
		Reserve(arr.mCapacity);
		mCount = arr.mCount;

		for (int i = 0; i < arr.mCount; i++)
		{
			if (i < mCount)
				mValues[i] = arr.mValues[i]; //TODO: check desctuctors calling
			else
				new (mValues + i) _type(arr.mValues[i]);
		}

		return *this;
	}

	template<typename _type>
	Array<_type> Array<_type>::operator+(const Array<_type>& arr) const
	{
		Array<_type> res(*this);
		res.Add(arr);
		return res;
	}

	template<typename _type>
	Array<_type> Array<_type>::operator+=(const Array<_type>& arr)
	{
		Add(arr);
		return *this;
	}

	template<typename _type>
	bool Array<_type>::operator==(const Array<_type>& arr)
	{
		if (arr.mCount != mCount)
			return false;

		for (int i = 0; i < mCount; i++)
		{
			if (mValues[i] != arr.mValues[i])
				return false;
		}

		return true;
	}

	template<typename _type>
	bool Array<_type>::operator!=(const Array<_type>& arr)
	{
		return !(*this == arr);
	}

	template<typename _type>
	int Array<_type>::Count() const
	{
		return  mCount;
	}

	template<typename _type>
	int Array<_type>::Capacity() const
	{
		return mCapacity;
	}

	template<typename _type>
	void Array<_type>::Resize(int newCount)
	{
		if (CONTAINERS_DEBUG)
			Assert(newCount > 0, "Can't resize array to zero size");

		Reserve(GetReservingSize(newCount));

		if (mCount < newCount)
		for (int i = newCount; i < mCount; i++)
			mValues[i].~_type();
		else
		for (int i = mCount; i < newCount; i++)
			new (mValues + i) _type();

		mCount = newCount;
	}

	template<typename _type>
	void Array<_type>::Reserve(int newCapacity)
	{
		if (CONTAINERS_DEBUG)
			Assert(newCapacity > 0, "Can't reserve array to zero size");

		if (newCapacity < mCapacity)
			newCapacity = mCapacity;

		if (newCapacity < 5)
			newCapacity = 5;

		mCapacity = newCapacity;

		mValues = (_type*)realloc(mValues, mCapacity*sizeof(_type));
	}

	template<typename _type>
	_type& Array<_type>::Get(int idx) const
	{
		if (CONTAINERS_DEBUG)
			Assert(idx >= 0 || idx < mCount, "Can't get array element: index out of range");

		return mValues[idx];
	}

	template<typename _type>
	void Array<_type>::Set(int idx, const _type& value)
	{
		if (CONTAINERS_DEBUG)
			Assert(idx >= 0 || idx < mCount, "Can't set array element: index out of range");

		mValues[idx] = value;
	}

	template<typename _type>
	_type& Array<_type>::Add(const _type& value)
	{
		if (mCount == mCapacity)
			Reserve(GetReservingSize(mCount + 1));

		new (mValues + mCount) _type(value);
		mCount++;

		return mValues[mCount - 1];
	}

	template<typename _type>
	void Array<_type>::Add(const IArray<_type>& arr)
	{
		int arrCount = arr.Count();
		if (mCount + arrCount >= mCapacity)
			Reserve(GetReservingSize(mCount + arrCount));

		for (int i = 0; i < arrCount; i++)
			new (mValues + mCount + i) _type(arr.Get(i));

		mCount += arrCount;
	}

	template<typename _type>
	_type Array<_type>::PopBack()
	{
		if (CONTAINERS_DEBUG)
			Assert(mCount > 0, "Can't pop value from array: no values");

		mCount--;
		_type res = mValues[mCount];
		mValues[mCount].~_type();
		return res;
	}

	template<typename _type>
	_type& Array<_type>::Insert(const _type& value, int position)
	{
		if (CONTAINERS_DEBUG)
			Assert(position >= 0 || position < mCount, "Can't insert element: index out of range");

		if (mCount == mCapacity)
			Reserve(GetReservingSize(mCount + 1));

		new (mValues + mCount) _type();
		mCount++;

		_type tmp = value;
		for (int i = position; i < mCount; i++)
		{
			_type curValue = mValues[i];
			mValues[i] = tmp;
			tmp = curValue;
		}

		return mValues[position];
	}

	template<typename _type>
	void Array<_type>::Insert(const IArray<_type>& arr, int position)
	{
		int arrCount = arr.Count();
		if (mCount + arrCount >= mCapacity)
			Reserve(GetReservingSize(mCount + arrCount));

		for (int i = position; i < mCount; i++)
			new (mValues + i + arrCount) _type(mValues[i]);

		for (int i = 0; i < arrCount; i++)
		{
			if (i < mCount)
				mValues[i + position] = arr.Get(i);
			else
				new (mValues + i + position) _type(arr.Get(i));
		}

		mCount += arrCount;
	}

	template<typename _type>
	int Array<_type>::Find(const _type& value) const
	{
		for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return i;

		return -1;
	}

	template<typename _type>
	bool Array<_type>::Contains(const _type& value) const
	{
		for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return true;

		return false;
	}

	template<typename _type>
	bool Array<_type>::RemoveAt(int idx)
	{
		if (idx < 0 || idx >= mCount)
			return false;

		mValues[idx].~_type();

		for (int i = idx; i < mCount - 1; i++)
			mValues[i] = mValues[i + 1];

		mCount--;

		return true;
	}

	template<typename _type>
	bool Array<_type>::RemoveRange(int begin, int end)
	{
		if (begin < 0 || end < 0 || begin >= mCount || end >= mCount || begin > end)
			return false;

		int diff = end - begin;

		for (int i = begin; i < end; i++)
			mValues[i].~_type();

		for (int i = begin; i < mCount - diff; i++)
			mValues[i] = mValues[i + diff];

		mCount -= diff;

		return true;
	}

	template<typename _type>
	bool Array<_type>::Remove(const _type& value)
	{
		int idx = Find(value);
		if (idx < 0)
			return false;

		RemoveAt(idx);
		return true;
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::Remove(const Iterator& it)
	{
		RemoveAt(it.Index());
		return it;
	}

	template<typename _type>
	void Array<_type>::Clear()
	{
		mCount = 0;
	}

	template<typename _type>
	void Array<_type>::Sort(const TFunction<bool(const _type&, const _type&)> pred /*= Math::Fewer*/)
	{
		QuickSort(pred, 0, mCount - 1);
	}

	template<typename _type>
	int Array<_type>::GetReservingSize(int size)
	{
		return (int)((float)size*1.5f);
	}

	template<typename _type>
	void Array<_type>::QuickSort(const TFunction<bool(const _type&, const _type&)> pred, int left, int right)
	{
		int i = left, j = right;
		_type tmp;
		_type pivot = mValues[(left + right)/2];

		/* partition */
		while (i <= j)
		{
			while (pred(mValues[i], pivot))
				i++;

			while (pred(pivot, mValues[j]))
				j--;

			if (i <= j)
			{
				tmp = mValues[i];
				mValues[i] = mValues[j];
				mValues[j] = tmp;
				i++;
				j--;
			}
		};

		/* recursion */
		if (left < j)
			QuickSort(pred, left, j);

		if (i < right)
			QuickSort(pred, i, right);
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::Begin()
	{
		return Iterator(*this, mValues);
	}

	template<typename _type>
	typename Array<_type>::Iterator Array<_type>::End()
	{
		return Iterator(*this, mValues + mCount);
	}

	template<typename _type>
	typename Array<_type>::ConstIterator Array<_type>::Begin() const
	{
		return ConstIterator(*this, mValues);
	}

	template<typename _type>
	typename Array<_type>::ConstIterator Array<_type>::End() const
	{
		return ConstIterator(*this, mValues + mCount);
	}

	template<typename _type>
	Array<_type> Array<_type>::FindAll(const TFunction<bool(const _type&)> match) const
	{
		Array<_type> res;
		for (auto x : *this)
		{
			if (match(x))
				res.Add(x);
		}
		return res;
	}

	template<typename _type>
	Array<_type> Array<_type>::Where(const TFunction<bool(const _type&)> match) const
	{
		Array<_type> res;
		for (auto x : *this)
		{
			if (match(x))
				res.Add(x);
		}
		return res;
	}

	template<typename _type>
	template<typename _sel_type>
	Array<_sel_type> Array<_type>::Select(const TFunction<_sel_type(const _type&)> selector) const
	{
		Array<_sel_type> res;
		for (auto x : *this)
			res.Add(selector(x));
		return res;
	}

	template<typename _type>
	Array<_type> Array<_type>::Take(int count) const
	{
		Array<_type> res;
		int i = 0;
		for (auto x : *this)
		{
			res.Add(x);
			i++;
			if (i == count)
				break;
		}
		return res;
	}
}