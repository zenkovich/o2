#pragma once

namespace o2
{
	typedef unsigned long long UInt64;
	typedef unsigned short     UInt16;
	typedef unsigned char      UInt8;
	typedef long long          Int64;

	typedef unsigned int       UInt;
	typedef unsigned long      ULong;

	typedef unsigned int       AssetId;

	typedef int                KeyboardKey;
	typedef int                CursorId;

	enum class BaseCorner { Left, Right, Top, Bottom, Center, LeftBottom, LeftTop, RightBottom, RightTop };

	enum class CursorType
	{
		AppStarting, Arrow, Cross, Hand, Help, IBeam, Icon, No, SizeAll, SizeNeSw, SizeNS, SizeNwSe,
		SizeWE, UpArrow, Wait
	};

	enum class Side { Left, Bottom, Right, Top, None };

	enum class TwoDirection { Horizontal, Vertical };
}