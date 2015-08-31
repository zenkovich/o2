#pragma once

#include "Utils/Containers/IArray.h"
#include "Utils/Assert.h"

#ifndef CONTAINERS_DEBUG
#define CONTAINERS_DEBUG true
#endif 

namespace o2
{
	// --------------------
	// Dynamic linear array
	// --------------------
	template<typename _type>
	class Vector: public IArray<_type>
	{
	public:
		class Iterator
		{
			friend class Vector<_type>;

			_type*  mValuePtr; // Current value pointer
			Vector* mArray;    // Owner array pointer

		public:
			// Constructor
			Iterator(Vector* arr, _type* valuePtr);

			// Returns index of current element
			int Index() const;

			// Returns current value reference
			_type& Value();

			// Returns true if value is valid (in array range)
			bool IsValid() const;

			// Plus operator - moving element index right
			Iterator operator+(int offs) const;

			// Minus operator - moving element index left
			Iterator operator-(int offs) const;

			// Increment operator
			Iterator& operator++();

			// Post increment operator
			Iterator operator++(int);

			// Decrement operator
			Iterator& operator--();

			// Post decrement operator
			Iterator operator--(int);

			// Plus and assign operator
			Iterator& operator+=(int offs);

			// Minus and assign operator
			Iterator& operator-=(int offs);

			// Greater check operator
			bool operator>(const Iterator& itr) const;

			// Fewer check operator
			bool operator<(const Iterator& itr) const;

			// Greater or equal operator
			bool operator>=(const Iterator& itr) const;

			// Fewer or equal operator
			bool operator<=(const Iterator& itr) const;

			// Equal operator
			bool operator==(const Iterator& itr) const;

			// Not equal operator
			bool operator!=(const Iterator& itr) const;

			// Boolean cast operator. Return true if value is valid (in array range)
			operator bool() const;

			// Get pointer operator
			_type* operator->();

			// Get reference operator
			_type& operator*();
		};

		class ConstIterator
		{
			friend class Vector<_type>;

			_type*        mValuePtr; // Current value pointer
			const Vector* mArray;	 // Owner array pointer

		public:
			// Constructor
			ConstIterator(const Vector* arr, _type* valuePtr);

			// Returns index of current element
			int Index() const;

			// Returns current value reference
			const _type& Value() const;

			// Returns true if value is valid (in array range)
			bool IsValid() const;

			// Plus operator - moving element index right
			ConstIterator operator+(int offs) const;

			// Minus operator - moving element index left
			ConstIterator operator-(int offs) const;

			// Increment operator
			ConstIterator& operator++();

			// Post increment operator
			ConstIterator operator++(int);

			// Decrement operator
			ConstIterator& operator--();

			// Post decrement operator
			ConstIterator operator--(int);

			// Plus and assign operator
			ConstIterator& operator+=(int offs);

			// Minus and assign operator
			ConstIterator& operator-=(int offs);

			// Greater check operator
			bool operator>(const ConstIterator& itr) const;

			// Fewer check operator
			bool operator<(const ConstIterator& itr) const;

			// Greater or equal operator
			bool operator>=(const ConstIterator& itr) const;

			// Fewer or equal operator
			bool operator<=(const ConstIterator& itr) const;

			// Equal operator
			bool operator==(const Iterator& itr) const;

			// Not equal operator
			bool operator!=(const Iterator& itr) const;

			// Boolean cast operator. Return true if value is valid (in array range)
			operator bool() const;

			// Get constant pointer operator
			const _type* const operator->();

			// Get constant reference operator
			const _type& operator*();
		};

	protected:
		_type* mValues;   // Array elements
		int    mCount;    // Count of elements
		int    mCapacity; // Size of mValues

	public:
		// Constructor by initial capacity
		Vector(int capacity = 5);

		// Copy-constructor
		Vector(const Vector& arr);

		// Constructor from other array
		Vector(const IArray<_type>* arr);

		// Destructor
		virtual ~Vector();

		// Assign operator
		Vector& operator=(const Vector& arr);

		// Plus operator - returns sum of this array and other elements
		Vector operator+(const Vector& arr) const;

		// Plus and assign operator - adding elements to this from other array
		Vector& operator+=(const Vector& arr);

		// Plus operator - returns vector with this elements and one more
		Vector operator+(const _type& value) const;

		// Plus and assign operator - adds element to this
		Vector& operator+=(const _type& value);

		// Minus operator - returns new vector, where removed elements
		Vector operator-(const Vector& arr) const;

		// Minus and assign operator - removes elements from array
		Vector& operator-=(const Vector& arr);

		// Minus operator - returns new vector without specified element
		Vector operator-(const _type& value) const;

		// Minus and assign operator - removes element from array
		Vector& operator-=(const _type& value);

		// Equal operator
		bool operator==(const Vector& arr) const;

		// Not equal operator
		bool operator!=(const Vector& arr) const;

		// Returns a copy of this
		IArray* Clone() const;

		// Returns count of elements in vector
		int Count() const;

		// Returns capacity of vector
		int Capacity() const;

		// Changes count of elements in array. If new size less than array size elements will be removed
		// Otherwise empty elements will be added at end
		void Resize(int newCount);

		// Changes capacity of vector. New capacity can't be less than current
		void Reserve(int newCapacity);

		// Returns value at index
		_type& Get(int idx) const;

		// Sets value at index
		void Set(int idx, const _type& value);

		// Adds new element
		_type& Add(const _type& value);

		// Adds elements from other array
		void Add(const IArray& arr);

		// Inserts new value at position
		_type& Insert(const _type& value, int position);

		// Inserts new values from other array at position
		void Insert(const IArray& arr, int position);

		// Returns index of equal element. Returns -1 when array haven't equal element
		int Find(const _type& value) const;

		// Returns true, if array contains the element
		bool Contains(const _type& value) const;

		// Removes element from back and returns him
		_type PopBack();

		// Removes element at position
		bool RemoveAt(int idx);

		// Removes elements in range
		bool RemoveRange(int begin, int end);

		// Removes equal array element
		bool Remove(const _type& value);

		// Removes element by iterator
		Iterator Remove(const Iterator& it);

		// Removes all elements
		void Clear();

		// Sorts elements in array by predicate
		void Sort(const Function<bool(const _type&, const _type&)>& pred = Math::Fewer);

		// Return vector of elements which pass function
		Vector FindAll(const Function<bool(const _type&)>& match) const;

		// Return vector of elements which pass function
		Vector Where(const Function<bool(const _type&)>& match) const;

		// Return vector of function results of all elements
		template<typename _sel_type>
		Vector<_sel_type> Select(const Function<_sel_type(const _type&)>& selector) const;

		// Returns first specified count elements
		Vector Take(int count) const;

		// Returns begin iterator
		Iterator Begin();

		// Returns end iterator
		Iterator End();

		// Returns constant begin iterator
		ConstIterator Begin() const;

		// Returns constant end iterator
		ConstIterator End() const;

		// Returns begin iterator (for range based "for")
		Iterator begin() { return Begin(); }

		// Returns end iterator (for range based "for")
		Iterator end() { return End(); }

		// Returns constant begin iterator (for range based "for")
		ConstIterator begin() const { return Begin(); }

		// Returns constant end iterator (for range based "for")
		ConstIterator end() const { return End(); }

	protected:
		// Calculates new optimal capacity for specified size
		int GetReservingSize(int size);

		// Quick sort algorithm
		void QuickSort(const Function<bool(const _type&, const _type&)>& pred, int left, int right);
	};

#pragma region Array::Iterator implementation

	template<typename _type>
	Vector<_type>::Iterator::Iterator(Vector<_type>* arr, _type* valuePtr):
		mValuePtr(valuePtr), mArray(arr)
	{
	}

	template<typename _type>
	int Vector<_type>::Iterator::Index() const
	{
		return (int)(mValuePtr - mArray->mValues);
	}

	template<typename _type>
	bool Vector<_type>::Iterator::IsValid() const
	{
		return !(mValuePtr < mArray->mValues || mValuePtr >= mArray->mValues + mArray->mCount);
	}

	template<typename _type>
	typename Vector<_type>::Iterator Vector<_type>::Iterator::operator+(int offs) const
	{
		return Iterator(mArray, mValuePtr + offs);
	}

	template<typename _type>
	typename Vector<_type>::Iterator Vector<_type>::Iterator::operator-(int offs) const
	{
		return Iterator(mArray, mValuePtr - offs);
	}

	template<typename _type>
	typename Vector<_type>::Iterator& Vector<_type>::Iterator::operator++()
	{
		mValuePtr++;
		return *this;
	}

	template<typename _type>
	typename Vector<_type>::Iterator Vector<_type>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		mValuePtr++;
		return temp;
	}

	template<typename _type>
	typename Vector<_type>::Iterator& Vector<_type>::Iterator::operator--()
	{
		mValuePtr--;
		return *this;
	}

	template<typename _type>
	typename Vector<_type>::Iterator Vector<_type>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		mValuePtr--;
		return temp;
	}

	template<typename _type>
	typename Vector<_type>::Iterator& Vector<_type>::Iterator::operator+=(int offs)
	{
		mValuePtr += offs;
		return *this;
	}

	template<typename _type>
	typename Vector<_type>::Iterator& Vector<_type>::Iterator::operator-=(int offs)
	{
		mValuePtr -= offs;
		return *this;
	}

	template<typename _type>
	bool Vector<_type>::Iterator::operator>(const Iterator& itr) const
	{
		return mValuePtr > itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::Iterator::operator<(const Iterator& itr) const
	{
		return mValuePtr < itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::Iterator::operator>=(const Iterator& itr) const
	{
		return mValuePtr >= itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::Iterator::operator<=(const Iterator& itr) const
	{
		return mValuePtr <= itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::Iterator::operator==(const Iterator& itr) const
	{
		return mValuePtr == itr.mValuePtr && mArray == itr.mArray;
	}

	template<typename _type>
	bool Vector<_type>::Iterator::operator!=(const Iterator& itr) const
	{
		return mValuePtr != itr.mValuePtr || mArray != itr.mArray;
	}

	template<typename _type>
	Vector<_type>::Iterator::operator bool() const
	{
		return IsValid();
	}

	template<typename _type>
	_type* Vector<_type>::Iterator::operator->()
	{
		return &Value();
	}

	template<typename _type>
	_type& Vector<_type>::Iterator::operator*()
	{
		return Value();
	}

	template<typename _type>
	_type& Vector<_type>::Iterator::Value()
	{
		if (CONTAINERS_DEBUG)
			Assert(mValuePtr >= mArray->mValues && mValuePtr <= mArray->mValues + mArray->mCount, "Array iterator is out of range");

		return *mValuePtr;
	}

#pragma endregion Array::Iterator implementation

#pragma region Array::ConstIterator implementation

	template<typename _type>
	Vector<_type>::ConstIterator::ConstIterator(const Vector<_type>* arr, _type* valuePtr):
		mArray(arr), mValuePtr(valuePtr)
	{
	}

	template<typename _type>
	int Vector<_type>::ConstIterator::Index() const
	{
		return mValuePtr - mArray->mValues;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::IsValid() const
	{
		return !(mValuePtr < mArray->mValues || mValuePtr >= mArray->mValues + mArray->mCount);
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator Vector<_type>::ConstIterator::operator+(int offs) const
	{
		return ConstIterator(mArray, mValuePtr + offs);
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator Vector<_type>::ConstIterator::operator-(int offs) const
	{
		return ConstIterator(mArray, mValuePtr - offs);
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator& Vector<_type>::ConstIterator::operator++()
	{
		mValuePtr++;
		return *this;
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator Vector<_type>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		mValuePtr++;
		return temp;
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator& Vector<_type>::ConstIterator::operator--()
	{
		mValuePtr--;
		return *this;
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator Vector<_type>::ConstIterator::operator--(int)
	{
		ConstIterator temp = *this;
		mValuePtr--;
		return temp;
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator& Vector<_type>::ConstIterator::operator+=(int offs)
	{
		mValuePtr += offs;
		return *this;
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator& Vector<_type>::ConstIterator::operator-=(int offs)
	{
		mValuePtr -= offs;
		return *this;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::operator>(const ConstIterator& itr) const
	{
		return mValuePtr > itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::operator<(const ConstIterator& itr) const
	{
		return mValuePtr < itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::operator>=(const ConstIterator& itr) const
	{
		return mValuePtr >= itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::operator<=(const ConstIterator& itr) const
	{
		return mValuePtr <= itr->mValuePtr;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::operator==(const Iterator& itr) const
	{
		return mValuePtr == itr.mValuePtr && mArray == itr.mArray;
	}

	template<typename _type>
	bool Vector<_type>::ConstIterator::operator!=(const Iterator& itr) const
	{
		return mValuePtr != itr.mValuePtr || mArray != itr.mArray;
	}

	template<typename _type>
	Vector<_type>::ConstIterator::operator bool() const
	{
		return IsValid();
	}

	template<typename _type>
	const _type* const Vector<_type>::ConstIterator::operator->()
	{
		return &Value();
	}

	template<typename _type>
	const _type& Vector<_type>::ConstIterator::operator*()
	{
		return Value();
	}

	template<typename _type>
	const _type& Vector<_type>::ConstIterator::Value() const
	{
		if (CONTAINERS_DEBUG)
			Assert(mValuePtr >= mArray->mValues && mValuePtr <= mArray->mValues + mArray->mCount, "Array iterator is out of range");

		return *mValuePtr;
	}

#pragma endregion Array::ConstIterator implementation

#pragma region Array implementation

	template<typename _type>
	Vector<_type>::Vector(int capacity /*= 5*/)
	{
		if (CONTAINERS_DEBUG)
			Assert(capacity > 0, "Can't initialize array with empty capacity");

		mValues = (_type*)malloc(sizeof(_type)*capacity);
		mCapacity = capacity;
		mCount = 0;
	}

	template<typename _type>
	Vector<_type>::Vector(const Vector& arr)
	{
		mValues = (_type*)malloc(sizeof(_type)*arr.mCapacity);
		mCapacity = arr.mCapacity;
		mCount = arr.mCount;

		for (int i = 0; i < mCount; i++)
			new (mValues + i) _type(arr.mValues[i]);
	}

	template<typename _type>
	Vector<_type>::Vector(const IArray<_type>* arr)
	{
		mCount = arr->Count();
		mCapacity = GetReservingSize(mCount);
		mValues = (_type*)malloc(sizeof(_type)*mCapacity);

		for (int i = 0; i < mCount; i++)
			new (mValues + i) _type(arr->Get(i));
	}

	template<typename _type>
	Vector<_type>::~Vector()
	{
		Clear();
		free(mValues);
	}

	template<typename _type>
	IArray<_type>* Vector<_type>::Clone() const
	{
		return new Vector<_type>(this);
	}

	template<typename _type>
	Vector<_type>& Vector<_type>::operator=(const Vector<_type>& arr)
	{
		Reserve(arr.mCapacity);

		for (int i = 0; i < mCount; i++)
			mValues[i].~_type();

		mCount = arr.mCount;

		for (int i = 0; i < arr.mCount; i++)
			new (mValues + i) _type(arr.mValues[i]);

		return *this;
	}

	template<typename _type>
	Vector<_type> Vector<_type>::operator+(const Vector<_type>& arr) const
	{
		Vector<_type> res(*this);
		res.Add(arr);
		return res;
	}

	template<typename _type>
	Vector<_type>& Vector<_type>::operator+=(const Vector<_type>& arr)
	{
		Add(arr);
		return *this;
	}

	template<typename _type>
	Vector<_type> Vector<_type>::operator+(const _type& value) const
	{
		Vector<_type> res(*this);
		res.Add(value);
		return res;
	}

	template<typename _type>
	Vector<_type>& Vector<_type>::operator+=(const _type& value)
	{
		Add(value);
		return *this;
	}


	template<typename _type>
	Vector<_type> Vector<_type>::operator-(const Vector<_type>& arr) const
	{
		Vector<_type> res(*this);
		res.Remove(arr);
		return res;
	}

	template<typename _type>
	Vector<_type>& Vector<_type>::operator-=(const Vector<_type>& arr)
	{
		Remove(arr);
		return *this;
	}

	template<typename _type>
	Vector<_type> Vector<_type>::operator-(const _type& value) const
	{
		Vector<_type> res(*this);
		res.Remove(value);
		return res;
	}

	template<typename _type>
	Vector<_type>& Vector<_type>::operator-=(const _type& value)
	{
		Remove(value);
		return *this;
	}

	template<typename _type>
	bool Vector<_type>::operator==(const Vector<_type>& arr) const
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
	bool Vector<_type>::operator!=(const Vector<_type>& arr) const
	{
		return !(*this == arr);
	}

	template<typename _type>
	int Vector<_type>::Count() const
	{
		return  mCount;
	}

	template<typename _type>
	int Vector<_type>::Capacity() const
	{
		return mCapacity;
	}

	template<typename _type>
	void Vector<_type>::Resize(int newCount)
	{
		if (CONTAINERS_DEBUG)
			Assert(newCount > 0, "Can't resize array to zero size");

		Reserve(GetReservingSize(newCount));

		if (mCount < newCount)
		{
			for (int i = newCount; i < mCount; i++)
				mValues[i].~_type();
		}
		else
		{
			for (int i = mCount; i < newCount; i++)
				new (mValues + i) _type();
		}

		mCount = newCount;
	}

	template<typename _type>
	void Vector<_type>::Reserve(int newCapacity)
	{
		if (newCapacity < mCapacity)
			return;

		if (newCapacity < 5)
			newCapacity = 5;

		mCapacity = newCapacity;

		_type* tmpValues = (_type*)malloc(mCount*sizeof(_type));
		for (int i = 0; i < mCount; i++)
		{
			new (tmpValues + i) _type(mValues[i]);
			mValues[i].~_type();
		}

		free(mValues);
		mValues = (_type*)malloc(mCapacity*sizeof(_type));

		for (int i = 0; i < mCount; i++)
		{
			new (mValues + i) _type(tmpValues[i]);
			tmpValues[i].~_type();
		}

		free(tmpValues);
	}

	template<typename _type>
	_type& Vector<_type>::Get(int idx) const
	{
		if (CONTAINERS_DEBUG)
			Assert(idx >= 0 || idx < mCount, "Can't get array element: index out of range");

		return mValues[idx];
	}

	template<typename _type>
	void Vector<_type>::Set(int idx, const _type& value)
	{
		if (CONTAINERS_DEBUG)
			Assert(idx >= 0 || idx < mCount, "Can't set array element: index out of range");

		mValues[idx] = value;
	}

	template<typename _type>
	_type& Vector<_type>::Add(const _type& value)
	{
		if (mCount == mCapacity)
			Reserve(GetReservingSize(mCount + 1));

		new (mValues + mCount) _type(value);
		mCount++;

		return mValues[mCount - 1];
	}

	template<typename _type>
	void Vector<_type>::Add(const IArray<_type>& arr)
	{
		int arrCount = arr.Count();
		if (mCount + arrCount >= mCapacity)
			Reserve(GetReservingSize(mCount + arrCount));

		for (int i = 0; i < arrCount; i++)
			new (mValues + mCount + i) _type(arr.Get(i));

		mCount += arrCount;
	}

	template<typename _type>
	_type Vector<_type>::PopBack()
	{
		if (CONTAINERS_DEBUG)
			Assert(mCount > 0, "Can't pop value from array: no values");

		mCount--;
		_type res = mValues[mCount];
		mValues[mCount].~_type();
		return res;
	}

	template<typename _type>
	_type& Vector<_type>::Insert(const _type& value, int position)
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
	void Vector<_type>::Insert(const IArray<_type>& arr, int position)
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
	int Vector<_type>::Find(const _type& value) const
	{
		for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return i;

		return -1;
	}

	template<typename _type>
	bool Vector<_type>::Contains(const _type& value) const
	{
		for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return true;

		return false;
	}

	template<typename _type>
	bool Vector<_type>::RemoveAt(int idx)
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
	bool Vector<_type>::RemoveRange(int begin, int end)
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
	bool Vector<_type>::Remove(const _type& value)
	{
		int idx = Find(value);
		if (idx < 0)
			return false;

		RemoveAt(idx);
		return true;
	}

	template<typename _type>
	typename Vector<_type>::Iterator Vector<_type>::Remove(const Iterator& it)
	{
		RemoveAt(it.Index());
		return it;
	}

	template<typename _type>
	void Vector<_type>::Clear()
	{
		for (int i = 0; i < mCount; i++)
			mValues[i].~_type();

		mCount = 0;
	}

	template<typename _type>
	void Vector<_type>::Sort(const Function<bool(const _type&, const _type&)>& pred /*= Math::Fewer*/)
	{
		QuickSort(pred, 0, mCount - 1);
	}

	template<typename _type>
	int Vector<_type>::GetReservingSize(int size)
	{
		return (int)((float)size*1.5f);
	}

	template<typename _type>
	void Vector<_type>::QuickSort(const Function<bool(const _type&, const _type&)>& pred, int left, int right)
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
	typename Vector<_type>::Iterator Vector<_type>::Begin()
	{
		return Iterator(this, mValues);
	}

	template<typename _type>
	typename Vector<_type>::Iterator Vector<_type>::End()
	{
		return Iterator(this, mValues + mCount);
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator Vector<_type>::Begin() const
	{
		return ConstIterator(this, mValues);
	}

	template<typename _type>
	typename Vector<_type>::ConstIterator Vector<_type>::End() const
	{
		return ConstIterator(this, mValues + mCount);
	}

	template<typename _type>
	Vector<_type> Vector<_type>::FindAll(const Function<bool(const _type&)>& match) const
	{
		Vector<_type> res;
		for (auto x : *this)
		{
			if (match(x))
				res.Add(x);
		}
		return res;
	}

	template<typename _type>
	Vector<_type> Vector<_type>::Where(const Function<bool(const _type&)>& match) const
	{
		Vector<_type> res;
		for (auto x : *this)
		{
			if (match(x))
				res.Add(x);
		}
		return res;
	}

	template<typename _type>
	template<typename _sel_type>
	Vector<_sel_type> Vector<_type>::Select(const Function<_sel_type(const _type&)>& selector) const
	{
		Vector<_sel_type> res;
		for (auto x : *this)
			res.Add(selector(x));
		return res;
	}

	template<typename _type>
	Vector<_type> Vector<_type>::Take(int count) const
	{
		Vector<_type> res;
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

#pragma endregion Array implementation

}