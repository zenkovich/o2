#pragma once

#include "o2/Utils/Types/String.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

namespace o2
{
	class UID
	{
	public:
		long long data[2];

	public:
		UID();
		UID(long long value);
		UID(const UID& other);
		UID(const String& stringData);

		UID& operator=(const UID& other);

		bool operator==(const UID& other) const;
		bool operator!=(const UID& other) const;

		bool operator<(const UID& other) const;

		void Randomize();

		String ToString() const;
		void FromString(const String& stringData);

		operator String() const;
		UID& operator=(const String& data);
	};
}
