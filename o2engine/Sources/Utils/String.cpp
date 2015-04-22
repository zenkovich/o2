#include "String.h"

namespace o2
{
	char charBuf[4096];

	o2::String FormatStr(const char* format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		vsprintf(charBuf, format, vlist);

		va_end(vlist);

		return (String)charBuf;
	}

	o2::String FormatStr(const char* format, va_list vlist)
	{
		vsprintf(charBuf, format, vlist);
		return (String)charBuf;
	}


	TString::TString():
		mData((wchar_t*)malloc(20*sizeof(wchar_t))), mCapacity(20)
	{
	}

	TString::TString(wchar_t* data):
		mCapacity(0)
	{
		while (data[mCapacity] != '\0') mCapacity++;
		mData = (wchar_t*)malloc(mCapacity*sizeof(wchar_t));
		memcpy(mData, data, mCapacity*sizeof(wchar_t));
	}

	TString::TString(char* data):
		mCapacity(0)
	{
		while (data[mCapacity] != '\0') mCapacity++;
		mData = (wchar_t*)malloc(mCapacity*sizeof(wchar_t));
		auto sz = mbstowcs((wchar_t*)mData, data, mCapacity);
		mData[sz] = '\0';
	}

	TString::TString(const TString& other):
		mCapacity(other.mCapacity), mData((wchar_t*)malloc(other.mCapacity*sizeof(wchar_t)))
	{
		memcpy(mData, other.mData, mCapacity*sizeof(wchar_t));
	}

	TString::TString(int value):
		mCapacity(32), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		int len = 0;

		bool neg = value < 0;
		if (neg) value = -value;

		while (value > 0)
		{
			mData[len++] = (value % 10) + '0';
			value /= 10;
		}

		if (neg) mData[len++] = '-';

		for (int i = 0; i < len/2; i++)
			Math::Swap(mData[i], mData[len - 1 - i]);

		mData[len++] = '\0';
	}

	TString::TString(UInt value):
		mCapacity(32), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		int len = 0;

		while (value > 0)
		{
			mData[len++] = (value % 10) + '0';
			value /= 10;
		}

		for (int i = 0; i < len/2; i++)
			Math::Swap(mData[i], mData[len - 1 - i]);

		mData[len++] = '\0';
	}

	TString::TString(float value):
		mCapacity(64), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		if (isnan(value))
		{
			mData[0] = 'n'; mData[1] = 'a'; mData[2] = 'n'; mData[3] = '\0';
		}
		else if (isinf(value))
		{
			mData[0] = 'i'; mData[1] = 'n'; mData[2] = 'f'; mData[3] = '\0';
		}
		else if (value == 0.0f)
		{
			mData[0] = '0'; mData[1] = '\0'; 
		}
		else
		{
			int digit, m, m1;
			wchar_t* c = mData;

			int neg = (value < 0);
			if (neg) value = -value;

			// calculate magnitude
			m = log10(value);
			int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);

			if (neg) *(c++) = '-';

			// set up for scientific notation
			if (useExp)
			{
				if (m < 0)
					m -= 1.0;
				value = value / pow(10.0, m);
				m1 = m;
				m = 0;
			}

			if (m < 1.0) m = 0;

			// convert the number
			while (value > 0.00000001f || m >= 0)
			{
				double weight = pow(10.0, m);
				if (weight > 0 && !isinf(weight))
				{
					digit = floor(value / weight);
					value -= (digit * weight);
					*(c++) = '0' + digit;
				}
				if (m == 0 && value > 0)
					*(c++) = '.';
				m--;
			} 
			if (useExp)
			{
				// convert the exponent
				int i, j;
				*(c++) = 'e';
				if (m1 > 0)
				{
					*(c++) = '+';
				}
				else
				{
					*(c++) = '-';
					m1 = -m1;
				}
				m = 0;
				while (m1 > 0)
				{
					*(c++) = '0' + m1 % 10;
					m1 /= 10;
					m++;
				}
				c -= m;
				for (i = 0, j = m-1; i<j; i++, j--)
				{
					// swap without temporary
					c[i] ^= c[j];
					c[j] ^= c[i];
					c[i] ^= c[j];
				}
				c += m;
			}
			*(c) = '\0';
		}
	}

	TString::TString(const Vec2F& value):
		mCapacity(256), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		*this += (TString)value.x;
		*this += (TString)";";
		*this += (TString)value.x;
	}

	TString::TString(const Vec2I& value):
		mCapacity(256), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		*this += (TString)value.x;
		*this += (TString)";";
		*this += (TString)value.x;
	}

	TString::TString(const RectF& value):
		mCapacity(512), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		*this += (TString)value.left;
		*this += (TString)";";
		*this += (TString)value.top;
		*this += (TString)";";
		*this += (TString)value.right;
		*this += (TString)";";
		*this += (TString)value.bottom;
	}

	TString::TString(const RectI& value):
		mCapacity(512), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		*this += (TString)value.left;
		*this += (TString)";";
		*this += (TString)value.top;
		*this += (TString)";";
		*this += (TString)value.right;
		*this += (TString)";";
		*this += (TString)value.bottom;
	}

	TString::TString(const Color4& value):
		mCapacity(512), mData((wchar_t*)malloc(mCapacity*sizeof(wchar_t)))
	{
		*this += (TString)value.r;
		*this += (TString)";";
		*this += (TString)value.g;
		*this += (TString)";";
		*this += (TString)value.b;
		*this += (TString)";";
		*this += (TString)value.a;
	}

	TString::~TString()
	{
		free(mData);
	}

	TString& TString::operator=(const TString& other)
	{
		Reserve(other.mCapacity);
		memcpy(mData, other.mData, other.mCapacity*sizeof(wchar_t));
		return *this;
	}

	TString& TString::operator=(wchar_t* data)
	{
		int dataLength = 0;
		while (data[dataLength] != '\0') dataLength++;
		Reserve(dataLength);
		memcpy(mData, data, dataLength*sizeof(wchar_t));
		return *this;
	}

	TString& TString::operator=(char* data)
	{
		int dataLength = 0;
		while (data[dataLength] != '\0') dataLength++;
		Reserve(dataLength);
		auto sz = mbstowcs((wchar_t*)mData, data, mCapacity);
		mData[sz] = '\0';
		return *this;
	}

	TString::operator wchar_t*() const
	{
		return mData;
	}

	TString::operator int() const
	{
		int res = 0;
		int m = 1;
		int l = Length();
		for (int i = l - 1; i >= 0; i--)
		{
			auto c = mData[i];
			if ((c >= '0' && c <= '9') || (i == 0 && c == '-'))
			{
				res += m*(c - '0');
				m += 10;
			}
			else return 0;
		}

		if (mData[0] == '-') res = -res;

		return res;
	}

	TString::operator float() const
	{
		float integerPart = 0;
		float fractionPart = 0;
		int divisorForFraction = 1;
		int sign = 1;
		bool inFraction = false;
		wchar_t* num = mData;

		/*Take care of +/- sign*/
		if (*num == '-')
		{
			++num;
			sign = -1;
		}
		else if (*num == '+')
		{
			++num;
		}
		while (*num != '\0')
		{
			if (*num >= '0' && *num <= '9')
			{
				if (inFraction)
				{
					/*See how are we converting a character to integer*/
					fractionPart = fractionPart*10.0f + (*num - '0');
					divisorForFraction *= 10.0f;
				}
				else
				{
					integerPart = integerPart*10.0f + (*num - '0');
				}
			}
			else if (*num == '.')
			{
				if (inFraction)
					return sign*(integerPart + fractionPart/divisorForFraction);
				else
					inFraction = true;
			}
			else
			{
				return sign*(integerPart + fractionPart/divisorForFraction);
			}
			++num;
		}
		return sign*(integerPart + fractionPart/divisorForFraction);
	}

	TString::operator UInt() const
	{
		int res = 0;
		int m = 1;
		int l = Length();
		for (int i = l - 1; i >= 0; i--)
		{
			auto c = mData[i];
			if ((c >= '0' && c <= '9') || (i == 0 && c == '-'))
			{
				res += m*(c - '0');
				m += 10;
			}
			else return 0;
		}

		return res;
	}

	TString::operator Vec2F() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 2)
			return Vec2F();

		splitted[0].Trim();
		splitted[1].Trim();

		return Vec2F((float)splitted[0], (float)splitted[1]);
	}

	TString::operator Vec2I() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 2)
			return Vec2F();

		splitted[0].Trim();
		splitted[1].Trim();

		return Vec2I((int)splitted[0], (int)splitted[1]);
	}

	TString::operator RectF() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 4)
			return RectF();

		splitted[0].Trim();
		splitted[1].Trim();
		splitted[3].Trim();
		splitted[4].Trim();

		return RectF((float)splitted[0], (float)splitted[1], (float)splitted[2], (float)splitted[3]);
	}

	TString::operator RectI() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 4)
			return RectI();

		splitted[0].Trim();
		splitted[1].Trim();
		splitted[3].Trim();
		splitted[4].Trim();

		return RectI((int)splitted[0], (int)splitted[1], (int)splitted[2], (int)splitted[3]);
	}

	TString::operator Color4() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 4)
			return Color4();

		splitted[0].Trim();
		splitted[1].Trim();
		splitted[3].Trim();
		splitted[4].Trim();

		return Color4((int)splitted[0], (int)splitted[1], (int)splitted[2], (int)splitted[3]);
	}

	bool TString::operator==(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();
		if (l1 != l2)
			return false;

		for (int i = 0; i < l1; i++)
		{
			if (mData[i] != other.mData[i])
				return false;
		}

		return true;
	}

	bool TString::operator!=(const TString& other) const
	{
		return !(*this == other);
	}

	bool TString::operator>(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();

		for (int i = 0; i < l1 && i < l2; i++)
		{
			if (mData[i] > other.mData[i])
				return true;
		}

		return l1 > l2;
	}

	TString TString::operator+(const TString& other) const
	{
		TString res(*this);
		res.Append(other);
		return res;
	}

	TString& TString::operator+=(const TString& other)
	{
		Append(other);
		return *this;
	}

	wchar_t& TString::operator[](int idx)
	{
		idx = Math::Clamp(idx, 0, mCapacity - 1);
		return mData[idx];
	}

	wchar_t* TString::Data() const
	{
		return mData;
	}

	void TString::Reserve(int size)
	{
		if (size <= mCapacity)
			return;

		mCapacity = size;

		mData = (wchar_t*)realloc(mData, mCapacity*sizeof(wchar_t));
	}

	int TString::Length() const
	{
		for (int i = 0; i < mCapacity; i++)
		{
			if (mData[i] == '\0')
				return i;
		}

		return 0;
	}

	void TString::Clear()
	{
		mData[0] = '\0';
	}

	bool TString::IsEmpty() const
	{
		return mData[0] == '\0';
	}

	void TString::Append(const TString& other)
	{
		int l1 = Length(), l2 = other.Length();
		Reserve(l1 + l2 + 10);
		for (int i = 0; i < l2; i++)
			mData[i + l1] = other.mData[i];
		mData[l1 + l2] = '\0';
	}

	void TString::Insert(const TString& other, int position /*= 0*/)
	{
		int l1 = Length(), l2 = other.Length();
		Reserve(l1 + l2 + 10);
		for (int i = 0; i < l2; i++)
		{
			mData[i + l2 + position] = mData[i + position];
			mData[i + position] = other.mData[i];
		}
	}

	void TString::Erase(int begin, int end)
	{
		if (end < begin)
			return;

		int l = Length();
		int d = end - begin;
		for (int i = begin; i < l - d + 1; i++)
			mData[i] = mData[i + d];
	}

	void TString::Replace(const TString& other, int begin, int end)
	{
		if (end < begin)
			return;

		Erase(begin, end);
		Insert(other, begin);
	}

	int TString::Find(const TString& other, int startIdx /*= 0*/) const
	{
		int l1 = Length(), l2 = other.Length();
		int lastFndStartIdx = startIdx;
		int fndIdx = 0;
		for (int i = startIdx; i < l1; i++)
		{
			if (mData[i] == other.mData[fndIdx])
			{
				if (fndIdx == l2 - 1)
					return lastFndStartIdx;
			}
			else
			{
				fndIdx = 0;
				lastFndStartIdx = i;
			}
		}

		return -1;
	}

	int TString::CountOf(const TString& other, int startIdx /*= 0*/) const
	{
		int res = 0;
		int searchIdx = startIdx;
		int l1 = Length(), l2 = other.Length();
		do 
		{
			int srch = Find(other, searchIdx);
			if (srch < 0)
				break;

			searchIdx = srch + l2;
		} 
		while (searchIdx < l1);

		return res;
	}

	int TString::FindLast(const TString& other, int startIdx /*= -1*/) const
	{
		int l1 = Length(), l2 = other.Length();
		int lastFndStartIdx = startIdx;
		int fndIdx = 0;
		for (int i = startIdx; i < l1; i++)
		{
			if (mData[i] != other.mData[fndIdx])
			{
				fndIdx = 0;
				lastFndStartIdx = i;
			}
		}

		if (lastFndStartIdx == l1 - 1)
			return -1;

		return lastFndStartIdx;
	}

	bool TString::EndsWith(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();
		for (int i = 0; i < l1 && i < l2; i++)
		{
			if (mData[l1 - 1 - i] == other.mData[l2 - 1 - i] && i == l2 - 1)
				return true;
		}

		return false;
	}

	bool TString::StartsWith(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();
		for (int i = 0; i < l1 && i < l2; i++)
		{
			if (mData[l1 - 1 - i] == other.mData[l2 - 1 - i] && i == l2 - 1)
				return true;
		}

		return false;
	}

	TString TString::SubStr(int begin, int end /*= -1*/) const
	{
		int b = Math::Min(begin, end);
		int e = Math::Max(begin, end);
		int d = e - b;

		TString res;
		res.Reserve(d + 1);
		memcpy(res.mData, mData + b, d*sizeof(wchar_t));
		res.mData[d] = '\0';
		return res;
	}

	Array<TString> TString::Split(const TString& splitStr) const
	{
		Array<TString> res;
		int lastFnd = 0;

	}

	TString TString::ToLowerCase() const
	{

	}

	TString TString::ToUpperCase() const
	{

	}

	void TString::Trim(const TString& trimSymbols /*= " "*/)
	{

	}

	void TString::TrimStart(const TString& trimSymbols /*= " "*/)
	{

	}

	void TString::TrimEnd(const TString& trimSymbols /*= " "*/)
	{

	}

	TString TString::Format(const TString& format, ...)
	{

	}

	TString TString::Format(const TString& format, va_list vlist)
	{

	}

}