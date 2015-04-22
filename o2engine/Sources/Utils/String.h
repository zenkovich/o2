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
	typedef std::string String;

	class TString
	{
		wchar_t* mData;
		int     mCapacity;

	public:
		TString();
		TString(const TString& other);
		TString(wchar_t* data);
		TString(char* data);
		explicit TString(int value);
		explicit TString(UInt value);
		explicit TString(float value);
		explicit TString(const Vec2F& value);
		explicit TString(const Vec2I& value);
		explicit TString(const RectF& value);
		explicit TString(const RectI& value);
		explicit TString(const Color4& value);
		~TString();

		TString& operator=(const TString& other);
		TString& operator=(wchar_t* data);
		TString& operator=(char* data);

		operator wchar_t*() const;

		explicit operator int() const;
		explicit operator float() const;
		explicit operator UInt() const;
		explicit operator Vec2F() const;
		explicit operator Vec2I() const;
		explicit operator RectF() const;
		explicit operator RectI() const;
		explicit operator Color4() const;

		bool operator==(const TString& other) const;
		bool operator!=(const TString& other) const;
		bool operator>(const TString& other) const;

		TString operator+(const TString& other) const;
		TString& operator+=(const TString& other);

		wchar_t& operator[](int idx);

		wchar_t* Data() const;
		void Reserve(int size);
		int Length() const;
		void Clear();
		bool IsEmpty() const;
		void Append(const TString& other);
		void Insert(const TString& other, int position = 0);
		void Erase(int begin, int end);
		void Replace(const TString& other, int begin, int end);
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

		static TString Format(const TString& format, ...);
		static TString Format(const TString& format, va_list vlist);
	};

	String FormatStr(const char* format, ...);
	String FormatStr(const char* format, va_list vlist);
}