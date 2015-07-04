#pragma once

#include "Utils/Delegates.h"

namespace o2
{
	template<typename _key_type, typename _value_type>
	class KeyValuePair
	{
	public:
		_key_type  mKey;
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

	template<typename _key_type, typename _value_type>
	class IDictionary
	{
	public:
		typedef KeyValuePair<_key_type, _value_type> TKeyValue;

		virtual void Add(const _key_type& key, const _value_type& value) = 0;
		virtual void Add(const TKeyValue& keyValue) = 0;

		virtual void Remove(const _key_type& key) = 0;
		virtual void RemoveAll(const Function<bool(const TKeyValue&)>& match) = 0;

		virtual void Clear() = 0;

		virtual bool ContainsKey(const _key_type& key) const = 0;
		virtual bool ContainsValue(const _value_type& value) const = 0;
		virtual bool Contains(const TKeyValue& keyValue) const = 0;
		virtual bool ContainsPred(const Function<bool(const TKeyValue&)>& match) const = 0;

		virtual TKeyValue FindKey(const _key_type& key) const = 0;
		virtual TKeyValue FindValue(const _value_type& value) const = 0;
		virtual TKeyValue Find(const Function<bool(const TKeyValue&)>& match) const = 0;
		virtual TKeyValue FindLast(const Function<bool(const TKeyValue&)>& match) const = 0;

		virtual TKeyValue First(const Function<bool(const TKeyValue&)>& match) const = 0;
		virtual TKeyValue Last(const Function<bool(const TKeyValue&)>& match) const = 0;

		virtual void Set(const _key_type& key, const _value_type& value) = 0;

		virtual _value_type& Get(const _key_type& key) = 0;

		virtual int Count() const = 0;
		virtual int Count(const Function<bool(const TKeyValue&)>& match) const = 0;

		virtual bool IsEmpty() const = 0;

		_value_type& operator[](const _key_type& key);

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

#pragma endregion IDictionary Implementation

}