#pragma once

#include <string>
#include <cstdarg>

namespace o2
{
	typedef std::string String;

	template<typename _char_type>
	class TString
	{
		_char_type* mData;
		int         mCapacity;

	public:
		TString();
		TString(_char_type* data);
		TString(const TString& other);

		TString& operator=(const TString& other);
		TString& operator=(_char_type* data);

		operator _char_type*() const;

		bool operator==(const TString& other) const;
		bool operator!=(const TString& other) const;

		String operator+(const TString& other) const;
		String& operator+=(const TString& other);
	};

	String FormatStr(const char* format, ...);
	String FormatStr(const char* format, va_list vlist);
}