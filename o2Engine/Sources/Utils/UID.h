#pragma once

#include "Utils/String.h"

namespace o2
{
	class UID
	{
	public:
		char data[16];

		UID()
		{ 
			Randomize(); 
		}

		UID(int value)
		{
			memset(data, 0, 16);
			memcpy(data, &value, 4);
		}

		UID(const UID& other) 
		{
			memcpy(data, other.data, 16); 
		}

		UID& operator=(const UID& other)
		{
			memcpy(data, other.data, 16);
			return *this;
		}

		bool operator==(const UID& other) const
		{
			return memcmp(data, other.data, 16) == 0;
		}

		bool operator!=(const UID& other) const
		{
			return memcmp(data, other.data, 16) != 0;
		}

		void Randomize()
		{
			for (int i = 0; i < 16; i += 2)
			{
				auto r = rand();
				memcpy(data + i, &r, 2);
			}
		}

		String ToString() const
		{
			char st[33];
			st[32] = '\0';
			for (int i = 0; i < 16; i += 4)
			{
				char buf[9];
				_itoa(*(int*)(data + i), buf, 16);

				int l = strlen(buf);
				for (int j = 0; j < 8; j++)
				{
					int x = l - 8 + j;
					st[i*2 + j] = x < 0 ? '0' : buf[x];
				}
			}

			return st;
		}

		void FromString(const String& stringData)
		{
			char* pp;
			char* str = stringData.Data();
			for (int i = 0; i < 16; i += 4)
			{
				int ii = i*2;
				int ii1 = ii + 8;
				char t = str[ii1];
				str[ii1] = '\0';
				long x = strtol(str + ii, &pp, 16);
				*(long*)(data + i) = x;
				str[ii1] = t;
			}
		}

		operator String() const
		{
			return ToString();
		}

		UID& operator=(const String& data)
		{
			FromString(data);
			return *this;
		}
	};
}