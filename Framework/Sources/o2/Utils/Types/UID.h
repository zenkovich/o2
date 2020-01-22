#pragma once

#include "o2/Utils/Types/String.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace o2
{
	class UID
	{
	public:
		char data[16];

	public:
		UID();
		UID(const UID& other);
		UID(int value);
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

	public:
		static UID empty;
	};
}
