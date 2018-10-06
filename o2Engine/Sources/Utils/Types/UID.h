#pragma once

#include "Utils/Types/String.h"
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
				std::stringstream stream;
				stream << std::hex << *(int*)(data + i);

				int l = (int)stream.str().length();
				for (int j = 0; j < 8; j++)
				{
					int x = l - 8 + j;
					st[i*2 + j] = x < 0 ? '0' : stream.str()[x];
				}
			}

			return st;
		}

		void FromString(const String& stringData)
		{
			char* pp;
			char* str = mnew char[stringData.Length() + 1];
			strcpy(str, stringData.Data());
			for (int i = 0; i < 16; i += 4)
			{
				int ii = i*2;
				int ii1 = ii + 8;
				char t = str[ii1];
				str[ii1] = '\0';
				long x = strtol(str + ii, &pp, 16);
				memcpy(data + i, &x, 4);
				//*(long*)(data + i) = x;
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
