#pragma once

namespace o2
{
	template<typename _keysArrayType>
	void SearchKey(const _keysArrayType& keys, int keysCount, float position, int& begin, int& end, bool direction, 
				   int& cacheKey)
	{
		begin = 0;
		end = 1;

		bool found = false;
		if (direction)
		{
			for (int i = cacheKey + 1; i < keysCount; i++)
			{
				if (keys[i].position >= position)
				{
					begin = i - 1;
					end = i;

					found = true;
					break;
				}
			}

			if (!found && keys[keysCount - 1].position < position)
			{
				begin = keysCount - 2;
				end = keysCount - 1;
				found = true;
			}

			if (!found)
			{
				for (int i = 1; i < keysCount; i++)
				{
					if (keys[i].position >= position || i == keysCount - 1)
					{
						begin = i - 1;
						end = i;
						break;
					}
				}
			}
		}
		else
		{
			for (int i = cacheKey; i >= 0; i--)
			{
				if (keys[i].position < position)
				{
					begin = i;
					end = i + 1;
					found = true;
					break;
				}
			}

			if (!found && keys[0].position > position)
			{
				begin = 0;
				end = 1;
				found = true;
			}

			if (!found)
			{
				for (int i = keysCount - 1; i >= 0; i--)
				{
					if (keys[i].position < position || i == 0)
					{
						begin = i;
						end = i + 1;
						break;
					}
				}
			}
		}

		cacheKey = begin;
	}
}
