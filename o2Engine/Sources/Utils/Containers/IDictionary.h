#pragma once

#include "Utils/Delegates.h"

namespace o2
{
	// -----------------------
	// Template key-value pair
	// -----------------------
	template<typename _key_type, typename _value_type>
	class KeyValuePair
	{
	public:
		_key_type   mKey;  
		_value_type mValue;

	public:
		KeyValuePair();
		KeyValuePair(const _key_type& key, const _value_type& value);
		KeyValuePair(const KeyValuePair& otherPair);

		bool operator==(const KeyValuePair& otherPair);
		bool operator!=(const KeyValuePair& otherPair);

		_key_type& Key();
		_value_type& Value();
	};

	// --------------------------
	// Basic dictionary interface
	// --------------------------
	template<typename _key_type, typename _value_type>
	class IDictionary
	{
	public:
		typedef KeyValuePair<_key_type, _value_type> TKeyValue;

		// Adds key value pair
		virtual void Add(const _key_type& key, const _value_type& value) = 0;

		// Adds key value pair
		virtual void Add(const TKeyValue& keyValue) = 0;

		// Removes element by key
		virtual void Remove(const _key_type& key) = 0;

		// Removes all passing function elements
		virtual void RemoveAll(const Function<bool(const TKeyValue&)>& match) = 0;

		// Removes all elements
		virtual void Clear() = 0;

		// Returns true, if dictionary contains element with specified key
		virtual bool ContainsKey(const _key_type& key) const = 0;

		// Returns true, if dictionary contains element with specified value
		virtual bool ContainsValue(const _value_type& value) const = 0;

		// Returns true, if dictionary contains equal element
		virtual bool Contains(const TKeyValue& keyValue) const = 0;

		// Returns true, if dictionary contains element which pass function
		virtual bool ContainsPred(const Function<bool(const TKeyValue&)>& match) const = 0;

		// Returns element by key
		virtual TKeyValue FindKey(const _key_type& key) const = 0;

		// Returns element by value
		virtual TKeyValue FindValue(const _value_type& value) const = 0;

		// Returns first element that pass function
		virtual TKeyValue Find(const Function<bool(const TKeyValue&)>& match) const = 0;

		// Returns last element that pass function
		virtual TKeyValue FindLast(const Function<bool(const TKeyValue&)>& match) const = 0;

		// Returns first element that pass function
		virtual TKeyValue First(const Function<bool(const TKeyValue&)>& match) const = 0;

		// Returns last element that pass function
		virtual TKeyValue Last(const Function<bool(const TKeyValue&)>& match) const = 0;

		// Sets value by specified key
		virtual void Set(const _key_type& key, const _value_type& value) = 0;

		// Returns reference to value by key
		virtual _value_type& Get(const _key_type& key) = 0;

		// Returns reference to value by key
		virtual const _value_type& Get(const _key_type& key) const = 0;

		// Returns count of elements
		virtual int Count() const = 0;

		// Returns count of elements which pass function
		virtual int Count(const Function<bool(const TKeyValue&)>& match) const = 0;

		// Returns true if dictionary have no elements
		virtual bool IsEmpty() const = 0;

		// Access operator
		_value_type& operator[](const _key_type& key);

		// Access operator
		const _value_type& operator[](const _key_type& key) const;

		// Invokes function fo all elements
		virtual void ForEach(const Function<void(TKeyValue&)>& func) = 0;
	};

#pragma region KeyValuePair Implementation

	template<typename _key_type, typename _value_type>
	KeyValuePair<_key_type, _value_type>::KeyValuePair() 
	{
	}

	template<typename _key_type, typename _value_type>
	KeyValuePair<_key_type, _value_type>::KeyValuePair(const _key_type& key, const _value_type& value):
		mKey(key), mValue(value) 
	{
	}

	template<typename _key_type, typename _value_type>
	KeyValuePair<_key_type, _value_type>::KeyValuePair(const KeyValuePair& otherPair):
		mKey(otherPair.mKey), mValue(otherPair.mValue) 
	{
	}

	template<typename _key_type, typename _value_type>
	bool KeyValuePair<_key_type, _value_type>::operator==(const KeyValuePair& otherPair)
	{
		return mKey == otherPair.mKey && mValue == otherPair.mValue;
	}

	template<typename _key_type, typename _value_type>
	bool KeyValuePair<_key_type, _value_type>::operator!=(const KeyValuePair& otherPair)
	{
		return mKey != otherPair.mKey || mValue != otherPair.mValue;
	}

	template<typename _key_type, typename _value_type>
	_key_type& KeyValuePair<_key_type, _value_type>::Key()
	{
		return mKey;
	}

	template<typename _key_type, typename _value_type>
	_value_type& KeyValuePair<_key_type, _value_type>::Value()
	{
		return mValue;
	}

#pragma endregion KeyValuePair Implementation

#pragma region IDictionary Implementation

	template<typename _key_type, typename _value_type>
	_value_type& IDictionary<_key_type, _value_type>::operator[](const _key_type& key)
	{
		return Get(key);
	}

	template<typename _key_type, typename _value_type>
	const _value_type& o2::IDictionary<_key_type, _value_type>::operator[](const _key_type& key) const
	{
		return Get(key);
	}


#pragma endregion IDictionary Implementation

}