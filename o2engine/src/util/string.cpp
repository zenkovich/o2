#include "string.h"

#include <cstdarg>

OPEN_O2_NAMESPACE

String Format(const char* str, ...)
{
	const uint bufsz = 1096;
	static char buf[bufsz];

	va_list ap;
	va_start(ap, str);
	vsnprintf(buf, bufsz, str, ap);
	va_end(ap);

	return String(buf);
}

String ToString(int value)
{
	return Format("%i", value);
}

String ToString(float value)
{
	return Format("%f", value);
}

String ToString(const Color4& value)
{
	return Format("(%i %i %i %i)", value.r, value.g, value.b, value.a);
}

String ToString(const Vec2F& value)
{
	return Format("(%.3f %.3f)", value.x, value.y);
}

int ToInt(const String& str)
{
	return atoi(str.c_str());
}

float ToFloat(const String& str)
{
	return (float)atof(str.c_str());
}


#ifdef PLATFORM_WIN
#include <Windows.h>

WString ConvString2Wide(const String& str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), 0, 0);

	WString retvalue;
	retvalue.resize(len, 0);
	wchar_t* retvalueptr = reinterpret_cast<wchar_t*>(const_cast<uint16*>(retvalue.data()));

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), retvalueptr, len);

	return retvalue;
}

String ConvWide2String(const WString& wide)
{
	const wchar_t* wideptr = reinterpret_cast<const wchar_t*>(wide.data());

	int len =  WideCharToMultiByte(CP_ACP, 0, wideptr, wide.size(), NULL, 0, 0, 0);

	String retvalue;
	retvalue.resize(len, 0);
	char* retvalueptr = const_cast<char*>(retvalue.data());

	WideCharToMultiByte(CP_ACP, 0, wideptr, wide.size(), retvalueptr, len + 1, 0, 0);

	return retvalue;
}

uint16 GetUnicodeFromVirtualCode(uint8 code)
{
	HKL layout=GetKeyboardLayout(0);

	BYTE allKeys[256];
	GetKeyboardState(allKeys);

	uint16 unicode;
	ToUnicodeEx(code, 0, allKeys, reinterpret_cast<wchar_t*>(&unicode), 1, 0, layout);
	return unicode;
}

String ExtractPath(const String& filePath)
{
	int fnd = filePath.rfind("/");
	return filePath.substr(0, fnd);
}

String ExtractExtension(const String& filePath)
{
	int fnd = filePath.rfind(".");
	return filePath.substr(0, fnd);
}

bool IsPathInsideOtherPath(const String& whatPath, const String& wherePath, bool strongly /*= false*/)
{
	String whatNorm = NormalizePathString(whatPath);
	String whereNorm = NormalizePathString(wherePath);

	if (whatNorm == whereNorm)
		return false;

	int a = whatNorm.find(whereNorm);
	if (a != 0)
		return false;

	int whereLength = wherePath.length();
	if (whereLength > 0 && whatPath[whereLength] != '/')
		return false;

	if (!strongly)
		return true;

	int lastSlashIdx = whatNorm.rfind("/");
	if (lastSlashIdx != String::npos && lastSlashIdx > whereLength)
		return false;

	return true;
}

String NormalizePathString(const String& path)
{
	String res = path;
	int length = res.length();

	if (length == 0)
		return res;

	for (int i = 0; i < length; i++)
	if (res[i] == '\\')
		res[i] = '/';

	int first = res[0] == '/' ? 1:0;
	int end = res[length - 1] == '/' ? length - 1:length;

	res = res.substr(first, end - first);
	return res;
}

#endif //PLATFORM_WIN

CLOSE_O2_NAMESPACE