#pragma once

#include "util/public_namespace.h"

OPEN_O2_NAMESPACE

template<typename _first_type, typename _second_type>
class Pair
{
public:
	_first_type  mFirst;
	_second_type mSecond;

public:
	Pair() {}

	Pair(const _first_type& first, const _second_type& second):
		mFirst(first), mSecond(second) {}

	Pair(const Pair& otherPair):
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

CLOSE_O2_NAMESPACE
