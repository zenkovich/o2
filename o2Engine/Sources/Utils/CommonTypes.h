#pragma once

namespace o2
{
	typedef unsigned long long UInt64;
	typedef unsigned short     UInt16;
	typedef unsigned char      UInt8;
	typedef long long          Int64;

	typedef unsigned int       UInt;
	typedef unsigned long      ULong;

	typedef int                KeyboardKey;
	typedef int                CursorId;

	typedef char               UnknownType;

	enum class BaseCorner { Left, Right, Top, Bottom, Center, LeftBottom, LeftTop, RightBottom, RightTop };

	enum class CursorType
	{
		AppStarting, Arrow, Cross, Hand, Help, IBeam, Icon, No, SizeAll, SizeNeSw, SizeNS, SizeNwSe,
		SizeWE, UpArrow, Wait
	};

	enum class Side { Left, Bottom, Right, Top, None };

	enum class Corner { LeftTop, RightTop, RightBottom, LeftBottom };

	enum class TwoDirection { Horizontal, Vertical };

	enum class SpriteMode { Default, Sliced, Tiled, FillLeftToRight, FillRightToLeft, FillUpToDown, FillDownToUp, 
		Fill360CW, Fill360CCW };

	enum class VerAlign { Top, Middle, Bottom, Both };
	enum class HorAlign { Left, Middle, Right, Both };

	enum class ProtectSection { Public, Private, Protected };
}