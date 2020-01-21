#include "o2/stdafx.h"

namespace o2
{
	UID& UID::operator=(const String& data)
	{
		FromString(data);
		return *this;
	}

	bool UID::operator!=(const UID& other) const
	{
		return data[0] != other.data[0] || data[1] != other.data[1];
	}

	bool UID::operator==(const UID& other) const
	{
		return data[0] == other.data[0] && data[1] == other.data[1];
	}

	UID::UID(const UID& other)
	{
		data[0] = other.data[0];
		data[1] = other.data[1];
	}

	UID::UID(long long value)
	{
		data[0] = 0;
		data[1] = value;
	}

	UID::UID()
	{
		Randomize();
	}

	UID::UID(const String& stringData)
	{
		FromString(stringData);
	}

	bool UID::operator<(const UID& other) const
	{
		if (data[1] < other.data[1])
			return true;
		
		if (data[1] == other.data[1])
			return data[0] < other.data[0];

		return false;
	}

	void UID::Randomize()
	{
		static std::default_random_engine generator;
		static std::uniform_int_distribution<long long> distribution;
		data[0] = distribution(generator);
		data[1] = distribution(generator);
	}

	String UID::ToString() const
	{
		std::stringstream stream;
		stream << std::hex << data[0] << ' ' << data[1];
		return stream.str();
	}

	void UID::FromString(const String& stringData)
	{
		auto s = stringData.find(' ');
		if (s == std::string::npos)
			Randomize();
		else
		{
			char* f;
			data[0] = strtoll(stringData.c_str(), &f, 16);
			data[1] = strtoll(stringData.c_str() + s, &f, 16);
		}
	}

	UID::operator String() const
	{
		return ToString();
	}

	UID& UID::operator=(const UID& other)
	{
		data[0] = other.data[0];
		data[1] = other.data[1];
		return *this;
	}
}
