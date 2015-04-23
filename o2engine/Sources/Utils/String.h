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

		int Length() const;
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
}