#pragma once

namespace o2
{
	// -------------
	// Template pair
	// -------------
	template<typename _key_type, typename _value_type>
	class Pair
	{
	public:
		_key_type   first;
		_value_type second;

	public:
		Pair() {}

		Pair(const _key_type& first, const _value_type& second) :
			first(first), second(second) {}

		Pair(const Pair& otherPair) :
			first(otherPair.first), second(otherPair.second) {}

		bool operator==(const Pair& otherPair) const
		{
			return first == otherPair.first && second == otherPair.second;
		}

		bool operator!=(const Pair& otherPair) const
		{
			return first != otherPair.first || second != otherPair.second;
		}
	};
}
