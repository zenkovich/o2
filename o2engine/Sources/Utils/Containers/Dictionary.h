#pragma once

#include "Utils/Containers/IDictionary.h"
#include "Utils/Containers/Vector.h"

namespace o2
{
	template<typename _key_type, typename _value_type>
	class Dictionary : public IDictionary<_key_type, _value_type>
	{
	public:
		class Iterator
		{
			typedef typename Vector<TKeyValue>::Iterator PairIterator;

			Dictionary*  mDictionary;
			PairIterator mPairIt;

		public:
			Iterator(Dictionary* dictionary, int index = 0);

			int Index() const;

			Iterator  operator+(int offs) const;
			Iterator& operator+=(int offs);

			Iterator  operator-(int offs) const;
			Iterator& operator-=(int offs);

			Iterator& operator++();    
			Iterator  operator++(int); 

			Iterator& operator--();    
			Iterator  operator--(int); 

			bool operator>(const Iterator& itr) const;
			bool operator<(const Iterator& itr) const;

			bool operator>=(const Iterator& itr) const;
			bool operator<=(const Iterator& itr) const;

			Iterator& operator*();

			operator bool() const;

			_key_type&   Key();
			_value_type& Value();
		};

		class ConstIterator
		{
			typedef typename Vector<TKeyValue>::ConstIterator PairIterator;

			const Dictionary* mDictionary;
			PairIterator      mPairIt;

		public:
			ConstIterator(const Dictionary* dictionary, int index = 0);

			int Index() const;

			ConstIterator  operator+(int offs) const;
			ConstIterator& operator+=(int offs);

			ConstIterator  operator-(int offs) const;
			ConstIterator& operator-=(int offs);

			ConstIterator& operator++();    
			ConstIterator  operator++(int); 

			ConstIterator& operator--();    
			ConstIterator  operator--(int); 

			bool operator>(const ConstIterator& itr) const;
			bool operator<(const ConstIterator& itr) const;

			bool operator>=(const ConstIterator& itr) const;
			bool operator<=(const ConstIterator& itr) const;

			const ConstIterator& operator*();

			operator bool() const;

			const _key_type&   Key() const;
			const _value_type& Value() const;
		};

	protected:
		Vector<TKeyValue> mPairs;

	public:
		Dictionary();
		Dictionary(const Dictionary& other);
		~Dictionary();

		bool operator==(const Dictionary& other) const;
		bool operator!=(const Dictionary& other) const;

		Dictionary& operator=(const Dictionary& other);

		void Add(const _key_type& key, const _value_type& value);
		void Add(const TKeyValue& keyValue);

		void Remove(const _key_type& key);
		void RemoveAll(const Function<bool(const TKeyValue&)>& match);
		void Clear();

		bool ContainsKey(const _key_type& key) const;
		bool ContainsValue(const _value_type& value) const;
		bool Contains(const TKeyValue& keyValue) const;
		bool ContainsPred(const Function<bool(const TKeyValue&)>& match) const;

		TKeyValue FindKey(const _key_type& key) const;
		TKeyValue FindValue(const _value_type& value) const;
		TKeyValue Find(const Function<bool(const TKeyValue&)>& match) const;
		TKeyValue FindLast(const Function<bool(const TKeyValue&)>& match) const;

		Dictionary FindAll(const Function<bool(const TKeyValue&)>& match) const;
		Dictionary Where(const Function<bool(const TKeyValue&)>& match) const;

		TKeyValue First(const Function<bool(const TKeyValue&)>& match) const;
		TKeyValue Last(const Function<bool(const TKeyValue&)>& match) const;

		void Set(const _key_type& key, const _value_type& value);

		_value_type&  Get(const _key_type& key);
		TKeyValue&    Get(int index) const;

		int Count() const;
		int Count(const Function<bool(const TKeyValue&)>& match) const;

		bool IsEmpty() const;

		void Sort(const Function<bool(const TKeyValue&, const TKeyValue&)>& pred);
		
		void ForEach(const Function<void(TKeyValue&)>& func);

		template<typename _sel_type>
		TKeyValue Min(const Function<_sel_type(const TKeyValue&)>& selector) const;

		template<typename _sel_type>
		int MinIdx(const Function<_sel_type(const TKeyValue&)>& selector) const;

		template<typename _sel_type>
		TKeyValue Max(const Function<_sel_type(const TKeyValue&)>& selector) const;

		template<typename _sel_type>
		int MaxIdx(const Function<_sel_type(const TKeyValue&)>& selector) const;

		virtual bool All(const Function<bool(const TKeyValue&)>& match) const;

		virtual bool Any(const Function<bool(const TKeyValue&)>& match) const;

		template<typename _sel_type>
		_sel_type Sum(const Function<_sel_type(const TKeyValue&)>& selector) const;

		Iterator Begin();
		Iterator End();

		ConstIterator Begin() const;
		ConstIterator End() const;

		Iterator begin() { return Begin(); }
		Iterator end() { return End(); }

		ConstIterator begin() const { return Begin(); }
		ConstIterator end() const { return End(); }
	};

#pragma region Dictionary::Iterator implementation

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Iterator::Iterator(Dictionary* dictionary, int index /*= 0*/) :
		mDictionary(dictionary), mPairIt(mDictionary->mPairs.Begin() + index)
	{
	}

	template<typename _key_type, typename _value_type>
	int Dictionary<_key_type, _value_type>::Iterator::Index() const
	{
		return mPairIt.Index();
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator+(int offs) const
	{
		return Iterator(mDictionary, Index() + offs);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator-(int offs) const
	{
		return Iterator(mDictionary, Index() - offs);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator++() // ++A;
	{
		mPairIt++;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator++(int) // A++;
	{
		Iterator temp = *this;
		mPairIt++;
		return temp;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator--() // --A;
	{
		mPairIt--;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Iterator::operator--(int) // A--;		
	{
		Iterator temp = *this;
		mPairIt--;
		return temp;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator+=(int offs)
	{
		mPairIt += offs;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator-=(int offs)
	{
		mPairIt -= offs;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator>(const Iterator& itr) const
	{
		return mPairIt > itr->mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator<(const Iterator& itr) const
	{
		return mPairIt < itr->mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator>=(const Iterator& itr) const
	{
		return mPairIt >= itr->mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator<=(const Iterator& itr) const
	{
		return mPairIt <= itr->mPairIt;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator*()
	{
		return *this;
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Iterator::operator bool() const
	{
		return mPairIt.IsValid();
	}

	template<typename _key_type, typename _value_type>
	_key_type& Dictionary<_key_type, _value_type>::Iterator::Key()
	{
		return mPairIt.Value().mKey;
	}

	template<typename _key_type, typename _value_type>
	_value_type& Dictionary<_key_type, _value_type>::Iterator::Value()
	{
		return mPairIt.Value().mValue;
	}

#pragma endregion Dictionary::Iterator implementation

#pragma region Dictionary::ConstIterator implementation

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::ConstIterator::ConstIterator(const Dictionary* dictionary, int index /*= 0*/) :
		mDictionary(dictionary), mPairIt(mDictionary->mPairs.Begin() + index)
	{
	}

	template<typename _key_type, typename _value_type>
	int Dictionary<_key_type, _value_type>::ConstIterator::Index() const
	{
		return mPairIt.Index();
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator+(int offs) const
	{
		return ConstIterator(mDictionary, Index() + offs);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator-(int offs) const
	{
		return ConstIterator(mDictionary, Index() - offs);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator++() // ++A;
	{
		mPairIt++;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator++(int) // A++;
	{
		ConstIterator temp = *this;
		mPairIt++;
		return temp;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator--() // --A;
	{
		mPairIt--;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator--(int) // A--;		
	{
		ConstIterator temp = *this;
		mPairIt--;
		return temp;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator+=(int offs)
	{
		mPairIt += offs;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator-=(int offs)
	{
		mPairIt -= offs;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ConstIterator::operator>(const ConstIterator& itr) const
	{
		return mPairIt > itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ConstIterator::operator<(const ConstIterator& itr) const
	{
		return mPairIt < itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ConstIterator::operator>=(const ConstIterator& itr) const
	{
		return mPairIt >= itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ConstIterator::operator<=(const ConstIterator& itr) const
	{
		return mPairIt <= itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	typename const Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator*()
	{
		return *this;
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::ConstIterator::operator bool() const
	{
		return mPairIt.IsValid();
	}

	template<typename _key_type, typename _value_type>
	const _key_type& Dictionary<_key_type, _value_type>::ConstIterator::Key() const
	{
		return mPairIt.Value().mKey;
	}

	template<typename _key_type, typename _value_type>
	const _value_type& Dictionary<_key_type, _value_type>::ConstIterator::Value() const
	{
		return mPairIt.Value().mValue;
	}

#pragma endregion Dictionary::ConstIterator implementation
	
#pragma region Dictionary implementation

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Dictionary()
	{
		mPairs = new Vector<TKeyValue>();
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Dictionary(const Dictionary<_key_type, _value_type>& other)
	{
		mPairs = other.mPairs.Clone();
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::~Dictionary()
	{
		Clear();
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::operator==(const Dictionary& other) const
	{
		return mPairs == other.mPairs;
		/*if (Count() != other.Count())
			return false;

		for (auto kv : mPairs)
		{
			bool found = false;
			for (auto otherKv : other.mPairs)
			{
				if (kv == otherKv)
				{
					found = true;
					break;
				}
			}

			if (!found)
				return false;
		}*/

		return true;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::operator!=(const Dictionary& other) const
	{
		return !(*this == other);
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>& Dictionary<_key_type, _value_type>::operator=(const Dictionary& other)
	{
		mPairs = other.mPairs;
		return *this;
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Add(const _key_type& key, const _value_type& value)
	{
		mPairs.Add(TKeyValue(key, value));
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Add(const TKeyValue& keyValue)
	{
		mPairs.Add(keyValue);
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Remove(const _key_type& key)
	{
		int idx = -1;
		for (auto kv = mPairs.Begin(); kv != mPairs.End(); ++kv)
		{
			if (kv->mKey == key)
			{
				idx = kv.Index();
				break;
			}
		}

		if (idx >= 0)
			mPairs.RemoveAt(idx);
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Clear()
	{
		mPairs.Clear();
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ContainsKey(const _key_type& key) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mKey == key)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ContainsValue(const _value_type& value) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mValue == value)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Contains(const TKeyValue& keyValue) const
	{
		for (auto kv:mPairs)
		{
			if (kv == keyValue)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::FindKey(const _key_type& key) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mKey == key)
				return kv;
		}

		return TKeyValue();
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::FindValue(const _value_type& value) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mValue == value)
				return kv;
		}

		return TKeyValue();
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Set(const _key_type& key, const _value_type& value)
	{
		for (auto kv:mPairs)
		{
			if (kv.mKey == key)
			{
				kv.mValue = value;
				break;
			}
		}
	}

	template<typename _key_type, typename _value_type>
	_value_type& Dictionary<_key_type, _value_type>::Get(const _key_type& key)
	{
		for (auto kv:mPairs)
		{
			if (kv.mKey == key)
				return kv.mValue;
		}

		Assert(false, "Failed to get value from dictionary: not found key");

		return mPairs.Get(0).mValue;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue& Dictionary<_key_type, _value_type>::Get(int index) const
	{
		return mPairs.Get(index);
	}

	template<typename _key_type, typename _value_type>
	int Dictionary<_key_type, _value_type>::Count() const
	{
		return mPairs.Count();
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::IsEmpty() const
	{
		return Count() == 0;
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Sort(const Function<bool(const TKeyValue&, const TKeyValue&)>& pred)
	{
		mPairs.Sort(pred);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::Begin()
	{
		return Iterator(this, 0);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator Dictionary<_key_type, _value_type>::End()
	{
		return Iterator(this, Count());
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::Begin() const
	{
		return ConstIterator(this, 0);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::End() const
	{
		return ConstIterator(this, Count());
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::ForEach(const Function<void(TKeyValue&)>& func)
	{
		for (auto kv:mPairs)
			func(kv);
	}

	template<typename _key_type, typename _value_type>
	int Dictionary<_key_type, _value_type>::Count(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.CountMatch(match);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::Last(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.Last(match);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::First(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.First(match);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::FindLast(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.Last(match);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::Find(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.FindMatch(match);
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ContainsPred(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.ContainsPred(match);
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::RemoveAll(const Function<bool(const TKeyValue&)>& match)
	{
		return mPairs.RemoveAll(match);
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	_sel_type Dictionary<_key_type, _value_type>::Sum(const Function<_sel_type(const TKeyValue&)>& selector) const
	{
		return mPairs.Sum(selector);
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Any(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.Any(match);
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::All(const Function<bool(const TKeyValue&)>& match) const
	{
		return mPairs.All(match);
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	int Dictionary<_key_type, _value_type>::MaxIdx(const Function<_sel_type(const TKeyValue&)>& selector) const
	{
		return mPairs.MaxIdx(selector);
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::Max(const Function<_sel_type(const TKeyValue&)>& selector) const
	{
		return mPairs.Max(selector);
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	int Dictionary<_key_type, _value_type>::MinIdx(const Function<_sel_type(const TKeyValue&)>& selector) const
	{
		return mPairs.MinIdx(selector);
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue Dictionary<_key_type, _value_type>::Min(const Function<_sel_type(const TKeyValue&)>& selector) const
	{
		return mPairs.Min(selector);
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type> Dictionary<_key_type, _value_type>::Where(const Function<bool(const TKeyValue&)>& match) const
	{
		Dictionary res;
		for (auto kv:mPairs)
		{
			if (match(kv))
				res.Add(kv);
		}

		return res;
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type> Dictionary<_key_type, _value_type>::FindAll(const Function<bool(const TKeyValue&)>& match) const
	{
		Dictionary res;
		for (auto kv:mPairs)
		{
			if (match(kv))
				res.Add(kv);
		}

		return res;
	}

#pragma endregion Dictionary implementation

}