#pragma once

namespace o2
{
	template<typename _key_type, typename _value_type>
	class Pair
	{
	public:
		_key_type  mFirst;
		_value_type mSecond;

	public:
		Pair() {}

		Pair(const _key_type& first, const _value_type& second) :
			mFirst(first), mSecond(second) {}

		Pair(const Pair& otherPair) :
			mFirst(otherPair.mFirst), mSecond(otherPair.mSecond) {}

		bool operator==(const Pair& otherPair)
		{
			return mFirst == otherPair.mFirst && mSecond == otherPair.mSecond;
		}

		bool operator!=(const Pair& otherPair)
		{
			return mFirst != otherPair.mFirst || mSecond != otherPair.mSecond;
		}
	};
}