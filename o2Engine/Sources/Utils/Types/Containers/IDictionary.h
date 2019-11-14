#pragma once

#include "Utils/Delegates.h"

namespace o2
{
	// --------------------------
	// Basic dictionary interface
	// --------------------------
	template<typename _key_type, typename _value_type>
	class IDictionary
	{
	public:
		using KeyValuePair = std::pair<_key_type, _value_type>;

		// Adds key value pair
		virtual void Add(const _key_type& key, const _value_type& value) = 0;

		// Adds key value pair
		virtual void Add(const KeyValuePair& keyValue) = 0;

		// Removes element by key
		virtual void Remove(const _key_type& key) = 0;

		// Removes all passing function elements
		virtual void RemoveAll(const Function<bool(const _key_type&, const _value_type&)>& match) = 0;

		// Removes all elements
		virtual void Clear() = 0;

		// Returns true, if dictionary contains element with specified key
		virtual bool ContainsKey(const _key_type& key) const = 0;

		// Returns true, if dictionary contains element with specified value
		virtual bool ContainsValue(const _value_type& value) const = 0;

		// Returns true, if dictionary contains equal element
		virtual bool Contains(const KeyValuePair& keyValue) const = 0;

		// Returns true, if dictionary contains element which pass function
		virtual bool ContainsPred(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Returns element by key
		virtual KeyValuePair FindKey(const _key_type& key) const = 0;

		// Returns element by value
		virtual KeyValuePair FindValue(const _value_type& value) const = 0;

		// Returns first element that pass function
		virtual KeyValuePair Find(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Returns last element that pass function
		virtual KeyValuePair FindLast(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Returns first element that pass function
		virtual KeyValuePair First(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Returns last element that pass function
		virtual KeyValuePair Last(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Sets value by specified key
		virtual void Set(const _key_type& key, const _value_type& value) = 0;

		// Returns reference to value by key
		virtual _value_type& Get(const _key_type& key) = 0;

		// Returns reference to value by key
		virtual const _value_type& Get(const _key_type& key) const = 0;

		// Returns count of elements
		virtual int Count() const = 0;

		// Returns count of elements which pass function
		virtual int Count(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Returns true if dictionary have no elements
		virtual bool IsEmpty() const = 0;

		// Invokes function to all elements
		virtual void ForEach(const Function<void(const _key_type&, _value_type&)>& match) = 0;

		// Returns true when all elements pass function
		virtual bool All(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;

		// Returns true when any of elements pass function
		virtual bool Any(const Function<bool(const _key_type&, const _value_type&)>& match) const = 0;
	};
}
