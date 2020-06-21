#pragma once

#include "o2/Utils/Debug/Assert.h"
#include "o2/Utils/Delegates.h"
#include <map>

namespace o2
{
	// ----------
	// Dictionary
	// ----------
	template<typename _key_type, typename _value_type>
	class Map : public std::map<_key_type, _value_type>
	{
	public:
		using KeyValuePair = typename std::map<_key_type, _value_type>::value_type;
		using Iterator = typename std::map<_key_type, _value_type>::iterator;
		using ConstIterator = typename std::map<_key_type, _value_type>::const_iterator;

	public:
		// Default constructor
		Map();

		// Copy-constructor
		Map(const Map& other);

		// Constructor from initializer list
		Map(std::initializer_list<KeyValuePair> init);

		// Destructor
		~Map();

		// Check equals operator
		bool operator==(const Map& other) const;

		// Check not equals operator
		bool operator!=(const Map& other) const;

		// Copy-operator
		Map& operator=(const Map& other);

		// Adds element
		void Add(const _key_type& key, const _value_type& value);

		// Adds element
		void Add(const KeyValuePair& keyValue);

		// Adds elements from other dictionary
		void Add(const Map& other);

		// Removes element by key
		void Remove(const _key_type& key);

		// Removes all which pass function
		void RemoveAll(const Function<bool(const _key_type&, const _value_type&)>& match);

		// Removes all elements
		void Clear();

		// Returns true if contains element with specified key
		bool ContainsKey(const _key_type& key) const;

		// Returns true if contains element with specified value
		bool ContainsValue(const _value_type& value) const;

		// Returns true if contains same element
		bool Contains(const KeyValuePair& keyValue) const;

		// Returns true if contains element which pass function
		bool Contains(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns element by key
		KeyValuePair FindKey(const _key_type& key) const;

		// Returns element by value
		KeyValuePair FindValue(const _value_type& value) const;

		// Returns first element which pass function
		KeyValuePair Find(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns last element which pass function
		KeyValuePair FindLast(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns all elements which pass function
		Map FindAll(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns all elements which pass function
		Map Where(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns first element which pass function
		KeyValuePair First(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns last element which pass function
		KeyValuePair Last(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Sets value by key
		void Set(const _key_type& key, const _value_type& value);

		// Returns value reference by key
		_value_type& Get(const _key_type& key);

		// Returns constant value reference by key
		const _value_type& Get(const _key_type& key) const;

		// Tries to get value by key, returns true if found
		bool TryGetValue(const _key_type& key, _value_type& output) const;

		// Returns value reference by index
		KeyValuePair GetIdx(int index) const;

		// Returns count of elements
		int Count() const;

		// Returns count of elements which pass function
		int Count(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns true when no elements
		bool IsEmpty() const;

		// Invokes function for all elements
		void ForEach(const Function<void(const _key_type&, _value_type&)>& func);

		// Returns minimal element by selector returned values
		template<typename _sel_type>
		KeyValuePair Min(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const;

		// Returns minimal element index by selector returned values
		template<typename _sel_type>
		int MinIdx(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const;

		// Returns maximal element by selector returned values
		template<typename _sel_type>
		KeyValuePair Max(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const;

		// Returns maximal element index by selector returned values
		template<typename _sel_type>
		int MaxIdx(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const;

		// Returns true when all elements pass function
		bool All(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns true when any of elements pass function
		bool Any(const Function<bool(const _key_type&, const _value_type&)>& match) const;

		// Returns sum of selector results for all elements
		template<typename _sel_type>
		_sel_type Sum(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const;

		// Returns begin iterator
		Iterator Begin() { return begin(); }

		// Returns end iterator
		Iterator End() { return end(); }

		// Returns constant begin iterator
		ConstIterator Begin() const { return cbegin(); }

		// Returns constant end iterator
		ConstIterator End() const { return cend(); }
	};

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type>::Map()
	{}

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type>::Map(const Map<_key_type, _value_type>& other):
		std::map<_key_type, _value_type>(other)
	{ }

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type>::Map(std::initializer_list<KeyValuePair> init):
		std::map<_key_type, _value_type>(init)
	{}

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type>::~Map()
	{}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::operator==(const Map& other) const
	{
		return std::operator==((const std::map<_key_type, _value_type>&)*this, (const std::map<_key_type, _value_type>&)other);
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::operator!=(const Map& other) const
	{
		return std::operator!=((const std::map<_key_type, _value_type>&)*this, (const std::map<_key_type, _value_type>&)other);
	}

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type>& Map<_key_type, _value_type>::operator=(const Map& other)
	{
		std::map<_key_type, _value_type>::operator=(other);
		return *this;
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::Add(const _key_type& key, const _value_type& value)
	{
		insert({ key, value });
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::Add(const KeyValuePair& keyValue)
	{
		insert(keyValue);
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::Add(const Map& other)
	{
		for (auto& kv : other)
			Add(kv.first, kv.second);
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::Remove(const _key_type& key)
	{
		erase(key);
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::Clear()
	{
		clear();
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::ContainsKey(const _key_type& key) const
	{
		return find(key) != end();
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::ContainsValue(const _value_type& value) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (it->second == value)
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::Contains(const KeyValuePair& keyValue) const
	{
		auto fnd = find(keyValue.first);
		return fnd != end() && fnd->second == keyValue.second;
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::FindKey(const _key_type& key) const
	{
		auto fnd = find(key);
		if (fnd != end())
			return { fnd->first, fnd->second };

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::FindValue(const _value_type& value) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (it->second == value)
				return { it->first, it->second };
		}

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::Set(const _key_type& key, const _value_type& value)
	{
		auto fnd = find(key);
		if (fnd != end())
			fnd->second = value;
		else
			insert({ key, value });
	}

	template<typename _key_type, typename _value_type>
	_value_type& Map<_key_type, _value_type>::Get(const _key_type& key)
	{
		auto fnd = find(key);
		if (fnd != end())
			return fnd->second;

		Assert(false, "Failed to get value from dictionary: not found key");

		static _value_type fake;
		return fake;
	}

	template<typename _key_type, typename _value_type>
	const _value_type& Map<_key_type, _value_type>::Get(const _key_type& key) const
	{
		auto fnd = find(key);
		if (fnd != end())
			return fnd->second;

		Assert(false, "Failed to get value from dictionary: not found key");

		static _value_type fake;
		return fake;
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::TryGetValue(const _key_type& key, _value_type& output) const
	{
		auto fnd = find(key);
		if (fnd != end())
		{
			output = fnd->second;
			return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::GetIdx(int index) const
	{
		int i = 0; 
		for (auto it = begin(); it != end(); ++it)
		{
			if (i == index)
				return { it->first, it->second };

			i++;
		}

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	int Map<_key_type, _value_type>::Count() const
	{
		return size();
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::IsEmpty() const
	{
		return empty();
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::ForEach(const Function<void(const _key_type&, _value_type&)>& func)
	{
		for (auto it = begin(); it != end(); ++it)
			func(it->first, it->second);
	}

	template<typename _key_type, typename _value_type>
	int Map<_key_type, _value_type>::Count(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		int res = 0; 
		for (auto it = begin(); it != end(); ++it)
		{
			if (match(it->first, it->second))
				res++;
		}

		return res;
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::Last(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		for (auto it = rbegin(); it != rend(); ++it)
		{
			if (match(it->first, it->second))
				return { it->first, it->second };
		}

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::First(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (match(it->first, it->second))
				return { it->first, it->second };
		}

		return KeyValuePair();
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::FindLast(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		return Last(match);
	}

	template<typename _key_type, typename _value_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::Find(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		return Find(match);
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::Contains(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		for (auto it = rbegin(); it != rend(); ++it)
		{
			if (match(it->first, it->second))
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	void Map<_key_type, _value_type>::RemoveAll(const Function<bool(const _key_type&, const _value_type&)>& match)
	{
		for (auto it = begin(); it != end();)
		{
			if (match(it->first, it->second)) 
				it = erase(it);
			else 
				++it;
		}
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	_sel_type Map<_key_type, _value_type>::Sum(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const
	{
		_sel_type res = _sel_type();
		for (auto it = begin(); it != end(); ++it)
			res += res + selector(it->first, it->second);

		return res;
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::Any(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		for (auto it = begin(); it != end(); ++it) 
		{
			if (match(it->first, it->second))
				return true;
		}

		return false;
	}

	template<typename _key_type, typename _value_type>
	bool Map<_key_type, _value_type>::All(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (!match(it->first, it->second))
				return false;
		}

		return true;
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	int Map<_key_type, _value_type>::MaxIdx(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const
	{
		int idx = 0;
		int maxIdx = 0;
		_sel_type maxVal;
		for (auto it = begin(); it != end(); ++it)
		{
			if (it == begin())
				maxVal = selector(it->first, it->second);
			else
			{
				_sel_type val = selector(it->first, it->second);
				if (val > maxVal)
				{
					maxVal = val;
					maxIdx = idx;
				}
			}

			idx++;
		}

		return idx;
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::Max(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const
	{
		_sel_type maxVal;
		KeyValuePair res;
		for (auto it = begin(); it != end(); ++it)
		{
			if (it == begin())
			{
				maxVal = selector(it->first, it->second);
				res = { it->first, it->second };
			}
			else
			{
				_sel_type val = selector(it->first, it->second);
				if (val > maxVal)
				{
					maxVal = val;
					res = { it->first, it->second };
				}
			}
		}

		return res;
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	int Map<_key_type, _value_type>::MinIdx(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const
	{
		int idx = 0;
		int minIdx = 0;
		_sel_type minVal;
		for (auto it = begin(); it != end(); ++it)
		{
			if (it == begin())
				minVal = selector(it->first, it->second);
			else
			{
				_sel_type val = selector(it->first, it->second);
				if (val < minVal)
				{
					minVal = val;
					minIdx = idx;
				}
			}

			idx++;
		}

		return idx;
	}

	template<typename _key_type, typename _value_type>
	template<typename _sel_type>
	typename Map<_key_type, _value_type>::KeyValuePair Map<_key_type, _value_type>::Min(const Function<_sel_type(const _key_type&, const _value_type&)>& selector) const
	{
		_sel_type minVal;
		KeyValuePair res;
		for (auto it = begin(); it != end(); ++it)
		{
			if (it == begin())
			{
				minVal = selector(it->first, it->second);
				res = { it->first, it->second };
			}
			else
			{
				_sel_type val = selector(it->first, it->second);
				if (val < minVal)
				{
					minVal = val;
					res = { it->first, it->second };
				}
			}
		}

		return res;
	}

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type> Map<_key_type, _value_type>::Where(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		Map res;
		for (auto it = begin(); it != end(); ++it)
		{
			if (match(it->first, it->second))
				res.Add(it->first, it->second);
		}

		return res;
	}

	template<typename _key_type, typename _value_type>
	Map<_key_type, _value_type> Map<_key_type, _value_type>::FindAll(const Function<bool(const _key_type&, const _value_type&)>& match) const
	{
		Map res;
		for (auto it = begin(); it != end(); ++it)
		{
			if (match(it->first, it->second))
				res.Add(it->first, it->second);
		}

		return res;
	}
}
