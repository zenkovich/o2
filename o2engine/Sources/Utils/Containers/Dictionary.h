#pragma once

#include "Utils/Containers/IDictionary.h"
#include "Utils/Containers/Vector.h"

namespace o2
{
	// ----------
	// Dictionary
	// ----------
	template<typename _key_type, typename _value_type>
	class Dictionary : public IDictionary<_key_type, _value_type>
	{
	public:
		// --------
		// Iterator
		// --------
		class Iterator
		{
			typedef typename Vector<TKeyValue>::Iterator PairIterator;

			Dictionary*  mDictionary; // Owner dictionary
			PairIterator mPairIt;     // Pair iterator

		public:
			// Constructor
			Iterator(Dictionary* dictionary, int index = 0);

			// Returns index of current element
			int Index() const;

			// Plus operator - moving element index right
			Iterator  operator+(int offs) const;

			// Plus and assign operator
			Iterator& operator+=(int offs);

			// Minus operator - moving element index left
			Iterator  operator-(int offs) const;

			// Minus and assign operator
			Iterator& operator-=(int offs);

			// Increment operator
			Iterator& operator++();    

			// Post increment operator
			Iterator  operator++(int); 

			// Decrement operator
			Iterator& operator--();    

			// Post decrement operator
			Iterator  operator--(int); 

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

			// Key access
			_key_type&   Key();

			// Value access
			_value_type& Value();

			// Pointer access operator (for range-based-for)
			Iterator& operator*();
		};

		// -----------------
		// Constant iterator
		// -----------------
		class ConstIterator
		{
			typedef typename Vector<TKeyValue>::ConstIterator PairIterator;

			const Dictionary* mDictionary; // Owner dictionary
			PairIterator      mPairIt;     // Pair iterator

		public:
			// Constructor
			ConstIterator(const Dictionary* dictionary, int index = 0);

			// Returns index of current element
			int Index() const;

			// Plus operator - moving element index right
			ConstIterator  operator+(int offs) const;

			// Plus and assign operator
			ConstIterator& operator+=(int offs);

			// Minus operator - moving element index left
			ConstIterator  operator-(int offs) const;

			// Minus and assign operator
			ConstIterator& operator-=(int offs);

			// Increment operator
			ConstIterator& operator++();   

			// Post increment operator
			ConstIterator  operator++(int); 

			// Decrement operator
			ConstIterator& operator--();    

			// Post decrement operator
			ConstIterator  operator--(int); 

			// Greater check operator
			bool operator>(const ConstIterator& itr) const;

			// Fewer check operator
			bool operator<(const ConstIterator& itr) const;

			// Greater or equal operator
			bool operator>=(const ConstIterator& itr) const;

			// Fewer or equal operator
			bool operator<=(const ConstIterator& itr) const;

			// Equal operator
			bool operator==(const ConstIterator& itr) const;

			// Not equal operator
			bool operator!=(const ConstIterator& itr) const;

			// Boolean cast operator. Return true if value is valid (in array range)
			operator bool() const;

			// Key access
			const _key_type&   Key() const;

			// Value access
			const _value_type& Value() const;

			// Pointer access operator (for range-based-for)
			const ConstIterator& operator*();
		};

	protected:
		Vector<TKeyValue> mPairs; // Key-value pairs vector

	public:
		// Default constructor
		Dictionary();

		// Copy-constructor
		Dictionary(const Dictionary& other);

		// Constructor from initializer list
		Dictionary(std::initializer_list<TKeyValue> init);

		// Destructor
		~Dictionary();

		// Check equals operator
		bool operator==(const Dictionary& other) const;

		// Check not equals operator
		bool operator!=(const Dictionary& other) const;

		// Copy-operator
		Dictionary& operator=(const Dictionary& other);

		// Adds element
		void Add(const _key_type& key, const _value_type& value);

		// Adds element
		void Add(const TKeyValue& keyValue);

		// Removes element by key
		void Remove(const _key_type& key);

		// Removes all which pass function
		void RemoveAll(const Function<bool(const TKeyValue&)>& match);

		// Removes all elements
		void Clear();

		// Returns true if contains element with specified key
		bool ContainsKey(const _key_type& key) const;

		// Returns true if contains element with specified value
		bool ContainsValue(const _value_type& value) const;

		// Returns true if contains same element
		bool Contains(const TKeyValue& keyValue) const;

		// Returns true if contains element which pass function
		bool ContainsPred(const Function<bool(const TKeyValue&)>& match) const;

		// Returns element by key
		TKeyValue FindKey(const _key_type& key) const;

		// Returns element by value
		TKeyValue FindValue(const _value_type& value) const;

		// Returns first element which pass function
		TKeyValue Find(const Function<bool(const TKeyValue&)>& match) const;

		// Returns last element which pass function
		TKeyValue FindLast(const Function<bool(const TKeyValue&)>& match) const;

		// Returns all elements which pass function
		Dictionary FindAll(const Function<bool(const TKeyValue&)>& match) const;

		// Returns all elements which pass function
		Dictionary Where(const Function<bool(const TKeyValue&)>& match) const;

		// Returns first element which pass function
		TKeyValue First(const Function<bool(const TKeyValue&)>& match) const;

		// Returns last element which pass function
		TKeyValue Last(const Function<bool(const TKeyValue&)>& match) const;

		// Sets value by key
		void Set(const _key_type& key, const _value_type& value);

		// Returns value reference by key
		_value_type& Get(const _key_type& key);

		// Returns constant value reference by key
		const _value_type& Get(const _key_type& key) const;

		// Tries to get value by key, returns true if found
		bool TryGetValue(const _key_type& key, _value_type& output) const;

		// Returns value reference by index
		TKeyValue& GetIdx(int index) const;

		// Returns count of elements
		int Count() const;

		// Returns count of elements which pass function
		int Count(const Function<bool(const TKeyValue&)>& match) const;

		// Returns true when no elements
		bool IsEmpty() const;

		// Sorts element by predicate
		void Sort(const Function<bool(const TKeyValue&, const TKeyValue&)>& pred);
		
		// Invokes function for all elements
		void ForEach(const Function<void(TKeyValue&)>& func);

		// Returns minimal element by selector returned values
		template<typename _sel_type>
		TKeyValue Min(const Function<_sel_type(const TKeyValue&)>& selector) const;

		// Returns minimal element index by selector returned values
		template<typename _sel_type>
		int MinIdx(const Function<_sel_type(const TKeyValue&)>& selector) const;

		// Returns maximal element by selector returned values
		template<typename _sel_type>
		TKeyValue Max(const Function<_sel_type(const TKeyValue&)>& selector) const;

		// Returns maximal element index by selector returned values
		template<typename _sel_type>
		int MaxIdx(const Function<_sel_type(const TKeyValue&)>& selector) const;

		// Returns true when all elements pass function
		virtual bool All(const Function<bool(const TKeyValue&)>& match) const;

		// Returns true when any of elements pass function
		virtual bool Any(const Function<bool(const TKeyValue&)>& match) const;

		// Returns sum of selector results for all elements
		template<typename _sel_type>
		_sel_type Sum(const Function<_sel_type(const TKeyValue&)>& selector) const;

		// Returns begin iterator
		Iterator Begin();

		// Returns end iterator
		Iterator End();

		// Returns constant begin iterator
		ConstIterator Begin() const;

		// Returns constant end iterator
		ConstIterator End() const;

		// Returns begin iterator (for range-based-for)
		Iterator begin() { return Begin(); }

		// Returns end iterator (for range-based-for)
		Iterator end() { return End(); }

		// Returns constant begin iterator (for range-based-for)
		ConstIterator begin() const { return Begin(); }

		// Returns constant end iterator (for range-based-for)
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
		return mPairIt > itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator<(const Iterator& itr) const
	{
		return mPairIt < itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator>=(const Iterator& itr) const
	{
		return mPairIt >= itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator<=(const Iterator& itr) const
	{
		return mPairIt <= itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator!=(const Iterator& itr) const
	{
		return mPairIt != itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::Iterator::operator==(const Iterator& itr) const
	{
		return mPairIt == itr.mPairIt;
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

	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::Iterator& Dictionary<_key_type, _value_type>::Iterator::operator*()
	{
		return *this;
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
	bool Dictionary<_key_type, _value_type>::ConstIterator::operator!=(const ConstIterator& itr) const
	{
		return mPairIt != itr.mPairIt;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::ConstIterator::operator==(const ConstIterator& itr) const
	{
		return mPairIt == itr.mPairIt;
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

	template<typename _key_type, typename _value_type>
	typename const Dictionary<_key_type, _value_type>::ConstIterator& Dictionary<_key_type, _value_type>::ConstIterator::operator*()
	{
		return *this;
	}

#pragma endregion Dictionary::ConstIterator implementation
	
#pragma region Dictionary implementation

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Dictionary()
	{
		mPairs = Vector<TKeyValue>();
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Dictionary(const Dictionary<_key_type, _value_type>& other)
	{
		mPairs = other.mPairs;
	}

	template<typename _key_type, typename _value_type>
	Dictionary<_key_type, _value_type>::Dictionary(std::initializer_list<TKeyValue> init)
	{
		for (auto elem : init)
			Add(elem);
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
		for (auto it = mPairs.Begin(); it != mPairs.End(); ++it)
		{
			TKeyValue& kv = *it;
			if (kv.mKey == key)
				return kv.mValue;
		}

		Assert(false, "Failed to get value from dictionary: not found key");

		return mPairs.Get(0).mValue;
	}

	template<typename _key_type, typename _value_type>
	const _value_type& Dictionary<_key_type, _value_type>::Get(const _key_type& key) const
	{
		for (auto it = mPairs.Begin(); it != mPairs.End(); ++it)
		{
			const TKeyValue& kv = *it;
			if (kv.mKey == key)
				return kv.mValue;
		}

		Assert(false, "Failed to get value from dictionary: not found key");

		return mPairs.Get(0).mValue;
	}

	template<typename _key_type, typename _value_type>
	bool Dictionary<_key_type, _value_type>::TryGetValue(const _key_type& key, _value_type& output) const
	{
		for (auto it = mPairs.Begin(); it != mPairs.End(); ++it)
		{
			const TKeyValue& kv = *it;
			if (kv.mKey == key)
			{
				output = kv.mValue;
				return true;
			}
		}

		return false;
	}


	template<typename _key_type, typename _value_type>
	typename Dictionary<_key_type, _value_type>::TKeyValue& Dictionary<_key_type, _value_type>::GetIdx(int index) const
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