#pragma once

#include <utility>

namespace o2
{
	template<typename _key_type, typename _value_type>
	using Pair = std::pair<_key_type, _value_type>;
}
