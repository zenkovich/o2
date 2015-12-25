#pragma once

#include "engine_settings.h"
#include "pair.h"
#include "util/public_namespace.h"
#include "util/assert.h"

OPEN_O2_NAMESPACE

template<typename _key_type, typename _value_type>
class IDictionary
{
public:
	typedef Pair< _key_type, _value_type > keyValuePair;

	virtual void Add(const _key_type& key, const _value_type& value) = 0;
	virtual void Add(const keyValuePair& keyValue) = 0;

	virtual void Remove(const _key_type& key) = 0;
	virtual void Clear() = 0;

	virtual bool ContainsKey(const _key_type& key) const = 0;
	virtual bool ContainsValue(const _value_type& value) const = 0;
	virtual bool Contains(const keyValuePair& keyValue) const = 0;

	virtual keyValuePair FindKey(const _key_type& key) const = 0;
	virtual keyValuePair FindValue(const _value_type& value) const = 0;

	virtual void Set(const _key_type& key, const _value_type& value) = 0;

	virtual _value_type& Get(const _key_type& key) = 0;

	virtual int Count() const = 0;

	virtual bool IsEmpty() const = 0;

	_value_type& operator[](const _key_type& key);
};

//implementation IDictionary

template<typename _key_type, typename _value_type>
_value_type& IDictionary<_key_type, _value_type>::operator[](const _key_type& key)
{
	return Get(key);
}

CLOSE_O2_NAMESPACE