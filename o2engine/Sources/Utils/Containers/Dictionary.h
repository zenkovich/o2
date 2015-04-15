#pragma once

#include "Utils/Containers/IDictionary.h"
#include "Utils/Containers/Array.h"

namespace o2
{
	template<typename _key_type, typename _value_type>
	class Dictionary : public IDictionary<_key_type, _value_type>
	{
	public:
		class Iterator
		{
			const Dictionary&                      mDictionary;
			typename Array<KeyValuePair>::Iterator mPairIt;

		public:
			Iterator(const Dictionary& dictionary, int index = 0);

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

			operator bool() const;

			_key_type&   Key() const;
			_value_type& Value() const;
		};

		class ConstIterator
		{
			const Dictionary&                      mDictionary;
			typename Array<KeyValuePair>::Iterator mPairIt;

		public:
			ConstIterator(const Dictionary& dictionary, int index = 0);

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

			operator bool() const;

			const _key_type&   Key() const;
			const _value_type& Value() const;
		};

	protected:
		Array<KeyValuePair> mPairs;

	public:
		Dictionary();
		Dictionary(const Dictionary& other);
		~Dictionary();

		bool operator==(const Dictionary& other) const;
		bool operator!=(const Dictionary& other) const;

		Dictionary& operator=(const Dictionary& other);

		void Add(const _key_type& key, const _value_type& value);
		void Add(const KeyValuePair& keyValue);

		void Remove(const _key_type& key);
		void Clear();

		bool ContainsKey(const _key_type& key) const;
		bool ContainsValue(const _value_type& value) const;
		bool Contains(const KeyValuePair& keyValue) const;

		KeyValuePair FindKey(const _key_type& key) const;
		KeyValuePair FindValue(const _value_type& value) const;

		void Set(const _key_type& key, const _value_type& value);

		_value_type&  Get(const _key_type& key);
		KeyValuePair& Get(int index) const;

		int  Count() const;
		bool IsEmpty() const;

		void Sort(bool(*compareFunc)(KeyValuePair&, KeyValuePair&));
		void Sort();

		Iterator Begin();
		Iterator End();

		ConstIterator Begin() const;
		ConstIterator End() const;
	};

	//implementation dict::iterator

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Iterator::Iterator(const Dictionary& dictionary, int index /*= 0*/) :
		mDictionary(dictionary), mPairIt(dictionary.Begin() + index)
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
	Dictionary<_key_type, _value_type>::Iterator::operator bool() const
	{
		return mPairIt.IsValid();
	}

	template<typename _key_type, typename _value_type>
	_key_type& Dictionary<_key_type, _value_type>::Iterator::Key() const
	{
		return mPairIt->mFirst;
	}

	template<typename _key_type, typename _value_type>
	_value_type& Dictionary<_key_type, _value_type>::Iterator::Value() const
	{
		return mPairIt->mSecond;
	}


	//implementation dict::ConstIterator

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::ConstIterator::ConstIterator(const Dictionary& dictionary, int index /*= 0*/) :
		mDictionary(dictionary), mPairIt(dictionary.Begin() + index)
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
		return ConstIterator(mArray, Index() + offs);
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::ConstIterator Dictionary<_key_type, _value_type>::ConstIterator::operator-(int offs) const
	{
		return ConstIterator(mArray, Index() - offs);
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
	Dictionary<_key_type, _value_type>::ConstIterator::operator bool() const
	{
		return mPairIt.IsValid();
	}

	template<typename _key_type, typename _value_type>
	const _key_type& Dictionary<_key_type, _value_type>::ConstIterator::Key() const
	{
		return mPairIt->mFirst;
	}

	template<typename _key_type, typename _value_type>
	const _value_type& Dictionary<_key_type, _value_type>::ConstIterator::Value() const
	{
		return mPairIt->mSecond;
	}

	//implementation Dictionary
	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Dictionary()
	{
		mPairs = new Array<KeyValuePair>();
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
		if (Count() != other.Count())
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
		}

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
		mPairs.Add(KeyValuePair(key, value));
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Add(const KeyValuePair& keyValue)
	{
		mPairs.Add(keyValue);
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Remove(const _key_type& key)
	{
		int idx = -1;
		for (auto kv = mPairs.Begin(); kv != mPairs.End(); ++kv)
		{
			if (kv->mFirst == key)
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
			if (kv.mFirst == key)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ContainsValue(const _value_type& value) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mSecond == value)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Contains(const KeyValuePair& keyValue) const
	{
		for (auto kv:mPairs)
		{
			if (kv == keyValue)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::KeyValuePair Dictionary<_key_type, _value_type>::FindKey(const _key_type& key) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mFirst == key)
				return kv;
		}

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::KeyValuePair Dictionary<_key_type, _value_type>::FindValue(const _value_type& value) const
	{
		for (auto kv:mPairs)
		{
			if (kv.mSecond == value)
				return kv;
		}

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Set(const _key_type& key, const _value_type& value)
	{
		for (auto kv:mPairs)
		{
			if (kv.mFirst == key)
			{
				kv.mSecond = value;
				break;
			}
		}
	}

	template<typename _key_type, typename _value_type>
	_value_type& Dictionary<_key_type, _value_type>::Get(const _key_type& key)
	{
		for (auto kv:mPairs)
		{
			if (kv.mFirst == key)
				return kv.mSecond;
		}

		Assert(false, "Failed to get value from dictionary: not found key");

		return mPairs.Get(0).mSecond;
	}

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::KeyValuePair& Dictionary<_key_type, _value_type>::Get(int index) const
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
	void Dictionary<_key_type, _value_type>::Sort(bool(*compareFunc)(KeyValuePair&, KeyValuePair&))
	{
		mPairs.Sort(compareFunc);
	}

	template<typename _key_type, typename _value_type>
	void Dictionary<_key_type, _value_type>::Sort()
	{
		mPairs.Sort();
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
}