#pragma once

#include "Utils/Delegates.h"
#include "Utils/Math/Math.h"

namespace o2
{
	/** Basic array interface. .*/
	template<typename _type>
	class IArray
	{
	public:
		virtual IArray* Clone() const = 0;

		_type& operator[](int idx);

		virtual _type& Get(int idx) const = 0;
		virtual void   Set(int idx, const _type& value) = 0;

		virtual int Count() const = 0;
		virtual int CountMatch(const Function<bool(const _type&)>& match) const;

		virtual void Resize(int newCount) = 0;

		virtual _type& Add(const _type& value) = 0;
		virtual void   Add(const IArray& arr) = 0;

		virtual _type& Insert(const _type& value, int position) = 0;
		virtual void   Insert(const IArray& arr, int position) = 0;

		virtual _type PopBack() = 0;

		virtual bool RemoveAt(int idx) = 0;
		virtual bool RemoveRange(int begin, int end) = 0;
		virtual bool Remove(const _type& value) = 0;
		virtual void RemoveAll(const Function<bool(const _type&)>& match);

		virtual void Clear() = 0;

		virtual bool Contains(const _type& value) const = 0;
		virtual bool ContainsPred(const Function<bool(const _type&)>& match) const;

		virtual int   Find(const _type& value) const = 0;
		virtual _type FindMatch(const Function<bool(const _type&)>& match) const;
		virtual int   FindIdx(const Function<bool(const _type&)>& match) const;

		virtual void Sort(const Function<bool(const _type&, const _type&)>& pred = Math::Fewer) = 0;

		template<typename _sort_type>
		void SortBy(const Function<_sort_type(const _type&)>& selector);

		virtual _type& First();
		virtual _type  First(const Function<bool(const _type&)>& match) const;
		virtual int    FirstIdx(const Function<bool(const _type&)>& match) const;

		virtual _type& Last();
		virtual _type  Last(const Function<bool(const _type&)>& match) const;
		virtual int    LastIdx(const Function<bool(const _type&)>& match) const;

		virtual bool IsEmpty() const;

		template<typename _sel_type>
		_type Min(const Function<_sel_type(const _type&)>& selector) const;

		template<typename _sel_type>
		int MinIdx(const Function<_sel_type(const _type&)>& selector) const;

		template<typename _sel_type>
		_type Max(const Function<_sel_type(const _type&)>& selector) const;

		template<typename _sel_type>
		int MaxIdx(const Function<_sel_type(const _type&)>& selector) const;

		virtual bool All(const Function<bool(const _type&)>& match) const;

		virtual bool Any(const Function<bool(const _type&)>& match) const;

		template<typename _sel_type>
		_sel_type Sum(const Function<_sel_type(const _type&)>& selector) const;

		virtual void ForEach(const Function<void(_type&)>& func);
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
			_type& val = Get(i);
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
			_type& val = Get(i);
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
			_type& val = Get(i);
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

		_type& res = Get(0);
		_sel_type minSel = selector(res);

		for (int i = 1; i < count; i++)
		{
			_type& itVal = Get(i);
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

		_type& res = Get(0);
		_sel_type minSel = selector(res);

		for (int i = 1; i < count; i++)
		{
			_type& itVal = Get(i);
			_sel_type itSel = selector(itVal);

			if (itSel > minSel)
			{
				minSel = itSel;
				res = itVal;
			}
		}

		return res;
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
			if (!match(Get(i)))
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
		{
			func(Get(i));
		}
	}
}