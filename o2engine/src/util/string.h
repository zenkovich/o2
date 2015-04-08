#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

typedef std::basic_string<uint16> WString;

/** Returns formatted string, Works same as sprintf(). */
String Format(const char* str, ...);

/** Returns converted string to wide string. */
WString ConvString2Wide(const String& str);

/** Returns converted wide string to string. */
String ConvWide2String(const WString& wide);

/** Extracting path string from file path. Ex "folder/xx/yy.png" returns "folder/xx/". */
String ExtractPath(const String& filePath);

/** Extracting extension from file path. Ex "folder/xx/yy.png" returns "folder/xx/yy". */
String ExtractExtension(const String& filePath);

/** Returns true, if whatPath path is wherePath path. */
bool IsPathInsideOtherPath(const String& whatPath, const String& wherePath, bool strongly = false);

/** Format path string to normalized. Ex "asd/das/dasd". */
String NormalizePathString(const String& path);

/** Returns character form virtual code. */
uint16 GetUnicodeFromVirtualCode(uint8 code);

/** Converting int value to string. */
String ToString(int value);

/** Converting float value to string. */
String ToString(float value);

/** Converting color value to string. */
String ToString(const Color4& value);

/** Converting vector value to string. */
String ToString(const Vec2F& value);

/** Converting string to integer. */
int ToInt(const String& str);

/** Converting string to float. */
float ToFloat(const String& str);

CLOSE_O2_NAMESPACE