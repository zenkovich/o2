#pragma once

#include <string>
#include <cstdarg>
#include "Utils/CommonTypes.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Color.h"
#include "Utils/Math/Rect.h"
#include "Utils/Containers/Array.h"

namespace o2
{
	template<typename T>
	class TString
	{
		T*  mData;
		int mCapacity;

	public:
		TString();

		template<typename T2>
		TString(const TString<T2>& other);

		template<typename T2>
		TString(T2* data);

		TString(const TString& other);

		explicit TString(bool value);
		explicit TString(int value);
		explicit TString(UInt value);
		explicit TString(float value);
		explicit TString(const Vec2F& value);
		explicit TString(const Vec2I& value);
		explicit TString(const RectF& value);
		explicit TString(const RectI& value);
		explicit TString(const Color4& value);

		~TString();

		template<typename T2>
		TString& operator=(const TString<T2>& other);

		TString& operator=(const TString& other);

		template<typename T2>
		TString& operator=(T2* data);

		operator T*() const;

		explicit operator bool() const;
		explicit operator int() const;
		explicit operator float() const;
		explicit operator UInt() const;
		explicit operator Vec2F() const;
		explicit operator Vec2I() const;
		explicit operator RectF() const;
		explicit operator RectI() const;
		explicit operator Color4() const;

		template<typename T2>
		bool operator==(T2* data) const;
		bool operator==(const TString& other) const;
		bool operator!=(const TString& other) const;
		bool operator>(const TString& other) const;

		TString operator+(const TString& other) const;
		TString& operator+=(const TString& other);

		T& operator[](int idx);

		T* Data() const;

		int Length() const;
		int Capacity() const;
		bool IsEmpty() const;

		void Clear();
		void Reserve(int size);

		void Append(const TString& other);
		TString Appended(const TString& other) const;

		void Insert(const TString& other, int position = 0);
		TString Inserted(const TString& other, int position = 0) const;

		void Erase(int begin, int end = -1);
		TString Erased(int begin, int end = -1) const;

		void Replace(const TString& other, int begin, int end);
		TString Replaced(const TString& other, int begin, int end) const;

		int Find(const TString& other, int startIdx = 0) const;
		int CountOf(const TString& other, int startIdx = 0) const;
		int FindLast(const TString& other, int startIdx = -1) const;

		bool EndsWith(const TString& other) const;
		bool StartsWith(const TString& other) const;

		TString SubStr(int begin, int end = -1) const;

		Array<TString> Split(const TString& splitStr) const;

		TString ToLowerCase() const;
		TString ToUpperCase() const;

		void Trim(const TString& trimSymbols = " ");
		void TrimStart(const TString& trimSymbols = " ");
		void TrimEnd(const TString& trimSymbols = " ");

		static TString Format(TString format, ...);
		static TString Format(TString format, va_list vlist);
	};

	typedef TString<wchar_t> WString;
	typedef TString<char> String;

	inline void ConvertStringPtr(char* dst, const wchar_t* src, int size)
	{
		wcstombs(dst, src, size);
	}

	inline void ConvertStringPtr(wchar_t* dst, const char* src, int size)
	{
		mbstowcs(dst, src, size);
	}

	inline void ConvertStringPtr(wchar_t* dst, const wchar_t* src, int size)
	{
		memcpy(dst, src, size*sizeof(wchar_t));
	}

	inline void ConvertStringPtr(char* dst, const char* src, int size)
	{
		memcpy(dst, src, size*sizeof(char));
	}

	inline String operator+(const char* left, const String& right)
	{
		String _left(left);
		return _left + right;
	}

	inline WString operator+(const wchar_t* left, const WString& right)
	{
		WString _left(left);
		return _left + right;
	}

	template<typename T>
	TString<T>::TString():
		mData((T*)malloc(20 * sizeof(T))), mCapacity(20)
	{
		mData[0] = '\0';
	}

	template<typename T>
	TString<T>::TString(const TString& other):
		mCapacity(other.mCapacity), mData((T*)malloc(other.mCapacity*sizeof(T)))
	{
		ConvertStringPtr(mData, other.mData, mCapacity);
	}

	template<typename T>
	template<typename T2>
	TString<T>::TString(T2* data):
		mCapacity(0)
	{
		while (data[mCapacity] != '\0') mCapacity++;
		mCapacity++;

		mData = (T*)malloc(mCapacity*sizeof(T));
		ConvertStringPtr(mData, data, mCapacity);
		mData[mCapacity - 1] = '\0';
	}


	template<typename T>
	template<typename T2>
	TString<T>::TString(const TString<T2>& other):
		mCapacity(other.Capacity()), mData((T*)malloc(other.Capacity()*sizeof(T)))
	{
		ConvertStringPtr(mData, other.Data(), mCapacity);
	}

	template<typename T>
	TString<T>::TString(bool value):
		mCapacity(8), mData((T*)malloc(8 * sizeof(T)))
	{
		if (value)
			ConvertStringPtr(mData, "true", 5);
		else
			ConvertStringPtr(mData, "false", 6);
	}

	template<typename T>
	TString<T>::TString(int value):
		mCapacity(32), mData((T*)malloc(32 * sizeof(T)))
	{
		int len = 0;

		bool neg = value < 0;
		if (neg) value = -value;

		do
		{
			mData[len++] = (value % 10) + '0';
			value /= 10;
		}
		while (value > 0);

		if (neg) mData[len++] = '-';

		for (int i = 0; i < len / 2; i++)
			Math::Swap(mData[i], mData[len - 1 - i]);

		mData[len++] = '\0';
	}

	template<typename T>
	TString<T>::TString(UInt value):
		mCapacity(32), mData((T*)malloc(32 * sizeof(T)))
	{
		int len = 0;

		while (value > 0)
		{
			mData[len++] = (value % 10) + '0';
			value /= 10;
		}

		for (int i = 0; i < len / 2; i++)
			Math::Swap(mData[i], mData[len - 1 - i]);

		mData[len++] = '\0';
	}

	template<typename T>
	TString<T>::TString(float value):
		mCapacity(64), mData((T*)malloc(64 * sizeof(T)))
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
			T* c = mData;

			int neg = (value < 0);
			if (neg) value = -value;

			// calculate magnitude
			m = (int)log10(value);
			int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);

			if (neg) *(c++) = '-';

			// set up for scientific notation
			if (useExp)
			{
				if (m < 0)
					m -= 1;
				value = value / pow(10.0f, m);
				m1 = m;
				m = 0;
			}

			if (m < 1.0) m = 0;

			// convert the number
			while (value > 0.00000001f || m >= 0)
			{
				float weight = pow(10.0f, m);
				if (weight > 0 && !isinf(weight))
				{
					digit = (int)floor(value / weight);
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
				for (i = 0, j = m - 1; i < j; i++, j--)
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

	template<typename T>
	TString<T>::TString(const Vec2F& value):
		mCapacity(256), mData((T*)malloc(256 * sizeof(T)))
	{
		mData[0] = '\0';
		Append((TString)value.x);
		Append((TString)";");
		Append((TString)value.x);
	}

	template<typename T>
	TString<T>::TString(const Vec2I& value):
		mCapacity(256), mData((T*)malloc(256 * sizeof(T)))
	{
		mData[0] = '\0';
		Append((TString)value.x);
		Append((TString)";");
		Append((TString)value.x);
	}

	template<typename T>
	TString<T>::TString(const RectF& value):
		mCapacity(512), mData((T*)malloc(512 * sizeof(T)))
	{
		mData[0] = '\0';
		Append((TString)value.left);
		Append((TString)";");
		Append((TString)value.top);
		Append((TString)";");
		Append((TString)value.right);
		Append((TString)";");
		Append((TString)value.bottom);
	}

	template<typename T>
	TString<T>::TString(const RectI& value):
		mCapacity(512), mData((T*)malloc(512 * sizeof(T)))
	{
		mData[0] = '\0';
		Append((TString)value.left);
		Append((TString)";");
		Append((TString)value.top);
		Append((TString)";");
		Append((TString)value.right);
		Append((TString)";");
		Append((TString)value.bottom);
	}

	template<typename T>
	TString<T>::TString(const Color4& value):
		mCapacity(512), mData((T*)malloc(512 * sizeof(T)))
	{
		mData[0] = '\0';
		Append((TString)value.r);
		Append((TString)";");
		Append((TString)value.g);
		Append((TString)";");
		Append((TString)value.b);
		Append((TString)";");
		Append((TString)value.a);
	}

	template<typename T>
	TString<T>::~TString()
	{
		free(mData);
	}

	template<typename T>
	template<typename T2>
	TString<T>& TString<T>::operator=(const TString<T2>& other)
	{
		Reserve(other.Capacity());
		ConvertStringPtr(mData, other.Data(), other.Capacity());
		return *this;
	}

	template<typename T>
	TString<T>& TString<T>::operator=(const TString& other)
	{
		Reserve(other.mCapacity);
		ConvertStringPtr(mData, other.mData, other.mCapacity);
		return *this;
	}

	template<typename T>
	template<typename T2>
	TString<T>& TString<T>::operator=(T2* data)
	{
		int dataLength = 0;
		while (data[dataLength] != '\0') dataLength++;
		Reserve(dataLength);
		ConvertStringPtr(mData, data, dataLength);
		mData[dataLength] = '\0';
		return *this;
	}

	template<typename T>
	TString<T>::operator T*() const
	{
		return mData;
	}

	template<typename T>
	TString<T>::operator bool() const
	{
		return *this == "true" || *this == "TRUE" || *this == "True";
	}

	template<typename T>
	TString<T>::operator int() const
	{
		int res = 0;
		int m = 1;
		int l = Length();
		for (int i = l - 1; i >= 0; i--)
		{
			auto c = mData[i];
			if (c >= '0' && c <= '9')
			{
				res += m*(c - '0');
				m *= 10;
			}
			else
			{
				if (i == 0 && c == '-')
					break;
				else
					return 0;
			}
		}

		if (mData[0] == '-') res = -res;

		return res;
	}

	template<typename T>
	TString<T>::operator float() const
	{
		float integerPart = 0;
		float fractionPart = 0;
		int divisorForFraction = 1;
		int sign = 1;
		bool inFraction = false;
		T* num = mData;

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
					fractionPart = fractionPart * 10 + (*num - '0');
					divisorForFraction *= 10;
				}
				else
				{
					integerPart = integerPart*10.0f + (*num - '0');
				}
			}
			else if (*num == '.')
			{
				if (inFraction)
					return sign*(integerPart + fractionPart / divisorForFraction);
				else
					inFraction = true;
			}
			else
			{
				return sign*(integerPart + fractionPart / divisorForFraction);
			}
			++num;
		}
		return sign*(integerPart + fractionPart / divisorForFraction);
	}

	template<typename T>
	TString<T>::operator UInt() const
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

	template<typename T>
	TString<T>::operator Vec2F() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 2)
			return Vec2F();

		splitted[0].Trim();
		splitted[1].Trim();

		return Vec2F((float)splitted[0], (float)splitted[1]);
	}

	template<typename T>
	TString<T>::operator Vec2I() const
	{
		auto splitted = Split(";");
		if (splitted.Count() < 2)
			return Vec2F();

		splitted[0].Trim();
		splitted[1].Trim();

		return Vec2I((int)splitted[0], (int)splitted[1]);
	}

	template<typename T>
	TString<T>::operator RectF() const
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

	template<typename T>
	TString<T>::operator RectI() const
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

	template<typename T>
	TString<T>::operator Color4() const
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

	template<typename T>
	template<typename T2>
	bool TString<T>::operator==(T2* data) const
	{
		int l2 = 0;
		while (data[l2] != '\0') l2++;
		l2++;

		int l1 = Length();
		if (l1 != l2)
			return false;

		for (int i = 0; i < l1; i++)
		{
			if (mData[i] != data[i])
				return false;
		}

		return true;
	}

	template<typename T>
	bool TString<T>::operator==(const TString& other) const
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

	template<typename T>
	bool TString<T>::operator!=(const TString& other) const
	{
		return !(*this == other);
	}

	template<typename T>
	bool TString<T>::operator>(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();

		for (int i = 0; i < l1 && i < l2; i++)
		{
			if (mData[i] > other.mData[i])
				return true;
		}

		return l1 > l2;
	}

	template<typename T>
	TString<T> TString<T>::operator+(const TString& other) const
	{
		TString res(*this);
		res.Append(other);
		return res;
	}

	template<typename T>
	TString<T>& TString<T>::operator+=(const TString& other)
	{
		Append(other);
		return *this;
	}

	template<typename T>
	T& TString<T>::operator[](int idx)
	{
		idx = Math::Clamp(idx, 0, mCapacity - 1);
		return mData[idx];
	}

	template<typename T>
	T* TString<T>::Data() const
	{
		return mData;
	}

	template<typename T>
	void TString<T>::Reserve(int size)
	{
		if (size <= mCapacity)
			return;

		mCapacity = size;

		mData = (T*)realloc(mData, mCapacity*sizeof(T));
	}

	template<typename T>
	int TString<T>::Length() const
	{
		for (int i = 0; i < mCapacity; i++)
		{
			if (mData[i] == '\0')
				return i;
		}

		return 0;
	}

	template<typename T>
	int TString<T>::Capacity() const
	{
		return mCapacity;
	}


	template<typename T>
	void TString<T>::Clear()
	{
		mData[0] = '\0';
	}

	template<typename T>
	bool TString<T>::IsEmpty() const
	{
		return mData[0] == '\0';
	}

	template<typename T>
	void TString<T>::Append(const TString& other)
	{
		int l1 = Length(), l2 = other.Length();
		Reserve(l1 + l2 + 10);
		for (int i = 0; i < l2; i++)
			mData[i + l1] = other.mData[i];
		mData[l1 + l2] = '\0';
	}

	template<typename T>
	void TString<T>::Insert(const TString& other, int position /*= 0*/)
	{
		int l1 = Length(), l2 = other.Length();
		Reserve(l1 + l2 + 10);
		for (int i = 0; i < l2; i++)
		{
			mData[i + l2 + position] = mData[i + position];
			mData[i + position] = other.mData[i];
		}
	}

	template<typename T>
	void TString<T>::Erase(int begin, int end /*= -1*/)
	{
		int l = Length();
		if (end < 0) end = l;

		if (end < begin || begin == end)
			return;

		int d = end - begin;
		for (int i = begin; i < l - d + 1; i++)
			mData[i] = mData[i + d];
	}

	template<typename T>
	void TString<T>::Replace(const TString& other, int begin, int end)
	{
		if (end < begin)
			return;

		Erase(begin, end);
		Insert(other, begin);
	}

	template<typename T>
	int TString<T>::Find(const TString& other, int startIdx /*= 0*/) const
	{
		int l1 = Length(), l2 = other.Length();
		int lastFndStartIdx = startIdx - 1;
		int fndIdx = 0;
		for (int i = startIdx; i < l1; i++)
		{
			if (mData[i] == other.mData[fndIdx])
			{
				fndIdx++;
				if (fndIdx == l2)
					return lastFndStartIdx + 1;
			}
			else
			{
				fndIdx = 0;
				lastFndStartIdx = i;
			}
		}

		return -1;
	}

	template<typename T>
	int TString<T>::CountOf(const TString& other, int startIdx /*= 0*/) const
	{
		int res = 0;
		int searchIdx = startIdx;
		int l1 = Length(), l2 = other.Length();
		do
		{
			int srch = Find(other, searchIdx);
			if (srch < 0)
				break;

			res++;
			searchIdx = srch + l2;
		}
		while (searchIdx < l1);

		return res;
	}

	template<typename T>
	int TString<T>::FindLast(const TString& other, int startIdx /*= -1*/) const
	{
		int l1 = Length(), l2 = other.Length();

		if (startIdx < 0) startIdx = l1 - 1;

		int fndIdx = l2 - 1;
		for (int i = startIdx; i >= 0; i--)
		{
			if (mData[i] == other.mData[fndIdx])
			{
				fndIdx--;

				if (fndIdx < 0)
					return i;
			}
			else fndIdx = l2 - 1;
		}

		return -1;

	}

	template<typename T>
	bool TString<T>::EndsWith(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();
		for (int i = 0; i < l1 && i < l2; i++)
		{
			if (mData[l1 - 1 - i] == other.mData[l2 - 1 - i] && i == l2 - 1)
				return true;
		}

		return false;
	}

	template<typename T>
	bool TString<T>::StartsWith(const TString& other) const
	{
		int l1 = Length(), l2 = other.Length();
		for (int i = 0; i < l1 && i < l2; i++)
		{
			if (mData[i] == other.mData[i] && i == l2 - 1)
				return true;
		}

		return false;
	}

	template<typename T>
	TString<T> TString<T>::SubStr(int begin, int end /*= -1*/) const
	{
		if (end < 0)
			end = Length();

		int b = Math::Min(begin, end);
		int e = Math::Max(begin, end);
		int d = e - b;

		TString res;
		res.Reserve(d + 1);
		memcpy(res.mData, mData + b, d*sizeof(T));
		res.mData[d] = '\0';
		return res;
	}

	template<typename T>
	Array<TString<T>> TString<T>::Split(const TString& splitStr) const
	{
		Array<TString> res;
		int lastFnd = 0;
		int i = 0;
		int l1 = Length(), l2 = splitStr.Length();
		while (i < l1)
		{
			int f = Find(splitStr, i);
			if (f < 0)
				break;

			res.Add(SubStr(i, f));
			i = f + l2;
		}
		res.Add(SubStr(i));

		return res;
	}

	template<typename T>
	TString<T> TString<T>::ToLowerCase() const
	{
		return *this;
	}

	template<typename T>
	TString<T> TString<T>::ToUpperCase() const
	{
		return *this;
	}

	template<typename T>
	void TString<T>::Trim(const TString& trimSymbols /*= " "*/)
	{
		TrimStart(trimSymbols);
		TrimEnd(trimSymbols);
	}

	template<typename T>
	void TString<T>::TrimStart(const TString& trimSymbols /*= " "*/)
	{
		int trimEnd = -1;
		int l1 = Length(), l2 = trimSymbols.Length();
		for (int i = 0; i < l1; i++)
		{
			bool breaking = true;
			auto dataChar = mData[i];
			for (int j = 0; j < l2; j++)
			{
				if (dataChar == trimSymbols.mData[j])
				{
					breaking = false;
					break;
				}
			}

			if (breaking)
				break;
			else
				trimEnd = i;
		}

		Erase(0, trimEnd + 1);
	}

	template<typename T>
	void TString<T>::TrimEnd(const TString& trimSymbols /*= " "*/)
	{
		int l1 = Length(), l2 = trimSymbols.Length();
		int trimEnd = l1;
		for (int i = l1 - 1; i >= 0; i--)
		{
			bool breaking = true;
			auto dataChar = mData[i];
			for (int j = 0; j < l2; j++)
			{
				if (dataChar == trimSymbols.mData[j])
				{
					breaking = false;
					break;
				}
			}

			if (breaking)
				break;
			else
				trimEnd = i;
		}

		Erase(trimEnd, l1);
	}

	template<typename T>
	TString<T> TString<T>::Format(const TString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		TString res = Format(format, vlist);

		va_end(vlist);

		return res;
	}

	template<typename T>
	TString<T> TString<T>::Format(const TString format, va_list vlist)
	{
		/*vsprintf(charBuf, format, vlist);
		return (TString)charBuf;*/

		int len = format.Length();

		TString res;
		res.Reserve(len * 2);

		int resLen = 0;

		auto appendStr = [&](const TString& str)
		{
			int l = str.Length();
			res.Reserve(resLen + l);
			for (int i = 0; i < l; i++)
				res.mData[resLen++] = str.mData[i];
		};

		for (int i = 0; i < len; i++)
		{
			if (format.mData[i] == '%')
			{
				bool success = true;

				if (format.mData[i + 1] == 'i')
				{
					appendStr((TString)va_arg(vlist, int));
				}
				else if (format.mData[i + 1] == 'f')
				{
					appendStr((TString)(float)va_arg(vlist, double));
				}
				else if (format.mData[i + 1] == 'd')
				{
					appendStr((TString)(float)va_arg(vlist, double));
				}
				else if (format.mData[i + 1] == 's')
				{
					appendStr((TString)va_arg(vlist, char*));
				}
				else if (format.mData[i + 1] == 'v' && format.mData[i + 2] == 'i')
				{
					appendStr((TString)va_arg(vlist, Vec2I));
					i++;
				}
				else if (format.mData[i + 1] == 'v' && format.mData[i + 2] == 'f')
				{
					appendStr((TString)va_arg(vlist, Vec2F));
					i++;
				}
				else if (format.mData[i + 1] == 'r' && format.mData[i + 2] == 'i')
				{
					appendStr((TString)va_arg(vlist, RectI));
					i++;
				}
				else if (format.mData[i + 1] == 'r' && format.mData[i + 2] == 'f')
				{
					appendStr((TString)va_arg(vlist, RectF));
					i++;
				}
				else if (format.mData[i + 1] == 'c' && format.mData[i + 2] == 'l')
				{
					appendStr((TString)va_arg(vlist, Color4));
					i++;
				}
				else if (format.mData[i + 1] == 't' && format.mData[i + 2] == 's')
				{
					appendStr(va_arg(vlist, TString));
					i++;
				}
				else if (format.mData[i + 1] == 'c')
				{
					res.mData[resLen++] = va_arg(vlist, char);
				}
				else
				{
					res.mData[resLen++] = format.mData[i];
					continue;
				}

				i++;
				continue;
			}

			res.mData[resLen++] = format.mData[i];
		}

		res.mData[resLen] = '\0';
		return res;
	}

	template<typename T>
	TString<T> TString<T>::Appended(const TString& other) const
	{
		TString res(*this);
		res.Append(other);
		return res;
	}

	template<typename T>
	TString<T> TString<T>::Inserted(const TString& other, int position /*= 0*/) const
	{
		TString res(*this);
		res.Insert(other, position);
		return res;
	}

	template<typename T>
	TString<T> TString<T>::Erased(int begin, int end /*= -1*/) const
	{
		TString res(*this);
		res.Erase(begin, end);
		return res;
	}

	template<typename T>
	TString<T> TString<T>::Replaced(const TString& other, int begin, int end) const
	{
		TString res(*this);
		res.Replace(other, begin, end);
		return res;
	}
}