#include "o2/stdafx.h"
#include "UID.h"

namespace o2
{
	UID& UID::operator=(const String& data)
	{
		FromString(data);
		return *this;
	}

	bool UID::operator!=(const UID& other) const
	{
		return memcmp(data, other.data, 16) != 0;
	}

	bool UID::operator==(const UID& other) const
	{
		return memcmp(data, other.data, 16) == 0;
	}

	UID::UID(const UID& other)
	{
		memcpy(data, other.data, 16);
	}

	UID::UID(int value)
	{
		memset(data, 0, 16);
		memcpy(data, &value, 4);
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
		return memcmp(data, other.data, 16) < 0;
	}

	void UID::Randomize()
	{
		for (int i = 0; i < 16; i += 2)
		{
			auto r = rand();
			memcpy(data + i, &r, 2);
		}
	}

	String UID::ToString() const
	{
		char st[33];
		st[32] = '\0';
		for (int i = 0; i < 16; i += 4)
		{
			std::stringstream stream;
			stream << std::hex << *(int*)(data + i);

			int l = (int)stream.str().length();
			for (int j = 0; j < 8; j++)
			{
				int x = l - 8 + j;
				st[i * 2 + j] = x < 0 ? '0' : stream.str()[x];
			}
		}

		return st;
	}

	void UID::FromString(const String& stringData)
	{
		char* pp;
		char* str = mnew char[stringData.Length() + 1];
		strcpy(str, stringData.Data());
		for (int i = 0; i < 16; i += 4)
		{
			int ii = i * 2;
			int ii1 = ii + 8;
			char t = str[ii1];
			str[ii1] = '\0';
			long x = strtol(str + ii, &pp, 16);
			memcpy(data + i, &x, 4);
			str[ii1] = t;
		}
	}

	UID::operator String() const
	{
		return ToString();
	}

	UID UID::empty = UID(0);

	UID& UID::operator=(const UID& other)
	{
		memcpy(data, other.data, 16);
		return *this;
	}

}
