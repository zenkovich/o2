#pragma once

#include "o2/Utils/Delegates.h"
#include "o2/Utils/Math/Math.h"

namespace o2
{
	// ---------------------
	// Basic array interface
	// ---------------------
	template<typename _type>
	class IArray
	{
	public:
		// Returns a copy of this
		virtual IArray* Clone() const = 0;

		// Returns value at index
		virtual const _type& Get(int idx) const = 0;

		// Returns value at index
		virtual _type& Get(int idx) = 0;

		// Sets value at index
		virtual void Set(int idx, const _type& value) = 0;

		// Returns count of elements in array
		virtual int Count() const = 0;

		// Returns count of elements in array by lambda
		virtual int CountMatch(const Function<bool(const _type&)>& match) const;

		// Changes count of elements in array. If new size less than array size elements will be removed
		// Otherwise empty elements will be added at end
		virtual void Resize(int newCount) = 0;

		// Adds new element
		virtual _type& Add(const _type& value) = 0;

		// Adds elements from other array
		virtual void Add(const IArray& arr) = 0;

		// Inserts new value at position
		virtual _type& Insert(const _type& value, int position) = 0;

		// Inserts new values from other array at position
		virtual void Insert(const IArray& arr, int position) = 0;

		// Removes element from back and returns him
		virtual _type PopBack() = 0;

		// Removes element at position
		virtual void RemoveAt(int idx) = 0;

		// Removes elements in range
		virtual void RemoveRange(int begin, int end) = 0;

		// Removes equal array element
		virtual void Remove(const _type& value) = 0;

		// Removes matched array element
		virtual void Remove(const Function<bool(const _type&)>& match) = 0;

		// Removes all elements that pass function
		virtual void RemoveAll(const Function<bool(const _type&)>& match);

		// Removes all elements
		virtual void Clear() = 0;

		// Returns true, if array contains the element
		virtual bool Contains(const _type& value) const = 0;

		// Returns true, if array contains a element that pass function
		virtual bool ContainsPred(const Function<bool(const _type&)>& match) const;

		// Returns index of equal element. Returns -1 when array haven't equal element
		virtual int Find(const _type& value) const = 0;

		// Returns elements of array that pass function
		virtual _type FindMatch(const Function<bool(const _type&)>& match) const;

		// Returns index of element that pass function
		virtual int FindIdx(const Function<bool(const _type&)>& match) const;

		// Sorts elements in array by predicate
		virtual void Sort(const Function<bool(const _type&, const _type&)>& pred = Math::Fewer) = 0;

		// Sorts elements in array by sorting value, that gets from function
		template<typename _sort_type>
		void SortBy(const Function<_sort_type(const _type&)>& selector);

		// Returns first element
		virtual _type& First();

		// Returns first element
		virtual const _type& First() const;

		// Returns first element that pass function
		virtual _type First(const Function<bool(const _type&)>& match) const;

		// Returns index of first element that pass function
		virtual int FirstIdx(const Function<bool(const _type&)>& match) const;

		// Returns last element
		virtual _type& Last();

		// Returns constant last element
		virtual const _type& Last() const;

		// Returns last element that pass function
		virtual _type Last(const Function<bool(const _type&)>& match) const;

		// Returns index of last element that pass function
		virtual int LastIdx(const Function<bool(const _type&)>& match) const;

		// Returns true if array is empty
		virtual bool IsEmpty() const;

		// Returns element by minimal result of function
		template<typename _sel_type>
		_type Min(const Function<_sel_type(const _type&)>& selector) const;

		// Returns element index by minimal result of function
		template<typename _sel_type>
		int MinIdx(const Function<_sel_type(const _type&)>& selector) const;

		// Returns element by maximal result of function
		template<typename _sel_type>
		_type Max(const Function<_sel_type(const _type&)>& selector) const;

		// Returns element index by maximal result of function
		template<typename _sel_type>
		int MaxIdx(const Function<_sel_type(const _type&)>& selector) const;

		// Returns all elements that pass function
		virtual bool All(const Function<bool(const _type&)>& match) const;

		// Returns true if any of elements pass function
		virtual bool Any(const Function<bool(const _type&)>& match) const;

		// Returns sum of function results for all elements
		template<typename _sel_type>
		_sel_type Sum(const Function<_sel_type(const _type&)>& selector) const;

		// Invokes function for all elements in array
		virtual void ForEach(const Function<void(_type&)>& func);

		// Reversing array
		void Reverse();
	};

	template<typename _type>
	_type& IArray<_type>::First()
	{
		return Get(0);
	}

	template<typename _type>
	const _type& IArray<_type>::First() const
	{
		return Get(0);
	}

	template<typename _type>
	_type& IArray<_type>::Last()
	{
		return Get(Count() - 1);
	}

	template<typename _type>
	const _type& IArray<_type>::Last() const
	{
		return Get(Count() - 1);
	}

	template<typename _type>
	bool IArray<_type>::IsEmpty() const
	{
		return Count() == 0;
	}

	template<typename _type>
	int IArray<_type>::CountMatch(const Function<bool(const _type&)>& match) const
	{
		int res = 0;
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			if (match(Get(i)))
				res++;
		}

		return res;
	}

	template<typename _type>
	void IArray<_type>::RemoveAll(const Function<bool(const _type&)>& match)
	{
		for (int i = 0; i < Count(); i++)
		{
			if (match(Get(i)))
			{
				RemoveAt(i);
				i--;
			}
		}
	}

	template<typename _type>
	bool IArray<_type>::ContainsPred(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			if (match(Get(i)))
				return true;
		}

		return false;
	}

	template<typename _type>
	_type IArray<_type>::FindMatch(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			const _type& val = Get(i);
			if (match(val))
				return val;
		}

		return _type();
	}

	template<typename _type>
	int IArray<_type>::FindIdx(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			if (match(Get(i)))
				return i;
		}

		return -1;
	}

	template<typename _type>
	template<typename _sort_type>
	void IArray<_type>::SortBy(const Function<_sort_type(const _type&)>& selector)
	{
		Sort([&](const _type& l, const _type& r){ return selector(l) < selector(r); });
	}

	template<typename _type>
	_type IArray<_type>::First(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			const _type& val = Get(i);
			if (match(val))
				return val;
		}

		return _type();
	}

	template<typename _type>
	int IArray<_type>::FirstIdx(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			if (match(Get(i)))
				return i;
		}

		return -1;
	}

	template<typename _type>
	_type IArray<_type>::Last(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = count - 1; i >= 0; i--)
		{
			const _type& val = Get(i);
			if (match(val))
				return val;
		}

		return _type();
	}

	template<typename _type>
	int IArray<_type>::LastIdx(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = count - 1; i >= 0; i++)
		{
			if (match(Get(i)))
				return i;
		}

		return -1;
	}

	template<typename _type>
	template<typename _sel_type>
	_type IArray<_type>::Min(const Function<_sel_type(const _type&)>& selector) const
	{
		int count = Count();
		if (count == 0)
			return _type();

		const _type& res = Get(0);
		_sel_type minSel = selector(res);

		for (int i = 1; i < count; i++)
		{
			const _type& itVal = Get(i);
			_sel_type itSel = selector(itVal);

			if (itSel < minSel)
			{
				minSel = itSel;
				res = itVal;
			}
		}

		return res;
	}

	template<typename _type>
	template<typename _sel_type>
	int IArray<_type>::MinIdx(const Function<_sel_type(const _type&)>& selector) const
	{
		int count = Count();
		if (count == 0)
			return -1;

		int res = 0;
		_sel_type minSel = selector(Get(0));

		for (int i = 1; i < count; i++)
		{
			_sel_type itSel = selector(Get(i));

			if (itSel < minSel)
			{
				res = i;
				minSel = itSel;
			}
		}

		return res;
	}

	template<typename _type>
	template<typename _sel_type>
	_type IArray<_type>::Max(const Function<_sel_type(const _type&)>& selector) const
	{
		int count = Count();
		if (count == 0)
			return _type();

		const _type* res = &Get(0);
		_sel_type minSel = selector(*res);

		for (int i = 1; i < count; i++)
		{
			const _type* itVal = &Get(i);
			_sel_type itSel = selector(*itVal);

			if (itSel > minSel)
			{
				minSel = itSel;
				res = itVal;
			}
		}

		return *res;
	}

	template<typename _type>
	template<typename _sel_type>
	int IArray<_type>::MaxIdx(const Function<_sel_type(const _type&)>& selector) const
	{
		int count = Count();
		if (count == 0)
			return -1;

		int res = 0;
		_sel_type minSel = selector(Get(0));

		for (int i = 1; i < count; i++)
		{
			_sel_type itSel = selector(Get(i));

			if (itSel > minSel)
			{
				res = i;
				minSel = itSel;
			}
		}

		return res;
	}

	template<typename _type>
	bool IArray<_type>::All(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			if (!match(Get(i)))
				return false;
		}

		return true;
	}

	template<typename _type>
	bool IArray<_type>::Any(const Function<bool(const _type&)>& match) const
	{
		int count = Count();
		for (int i = 0; i < count; i++)
		{
			if (match(Get(i)))
				return true;
		}

		return false;
	}

	template<typename _type>
	template<typename _sel_type>
	_sel_type IArray<_type>::Sum(const Function<_sel_type(const _type&)>& selector) const
	{
		int count = Count();
		if (count == 0)
			return _sel_type();

		_sel_type res = selector(Get(0));
		for (int i = 1; i < count; i++)
		{
			res = res + selector(Get(i));
		}

		return res;
	}

	template<typename _type>
	void IArray<_type>::ForEach(const Function<void(_type&)>& func)
	{
		int count = Count();
		for (int i = 0; i < count; i++)
			func(Get(i));
	}

	template<typename _type>
	void IArray<_type>::Reverse()
	{
		int c = Count();
		for (int i = 0; i < c/2; i++)
		{
			_type a = Get(i);
			_type b = Get(c - i - 1);

			Set(i, b);
			Set(c - i - 1, a);
		}
	}
}
