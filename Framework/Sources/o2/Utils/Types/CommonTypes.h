#pragma once

#include "o2/Utils/Reflection/Enum.h"
#include <cstddef>

namespace o2
{
	typedef unsigned long long UInt64;
	typedef unsigned short     UInt16;
	typedef unsigned char      UInt8;
	typedef long long          Int64;

	typedef unsigned int  UInt;
	typedef unsigned long ULong;

	typedef int KeyboardKey;
	typedef int CursorId;

	typedef UInt64 SceneUID;

	typedef std::byte Byte;

	enum class BaseCorner { Left, Right, Top, Bottom, Center, LeftBottom, LeftTop, RightBottom, RightTop };

	enum class CursorType
	{
		AppStarting, Arrow, Cross, Hand, Help, IBeam, Icon, No, SizeAll, SizeNeSw, SizeNS, SizeNwSe,
		SizeWE, UpArrow, Wait
	};

	enum class Side { Left, Bottom, Right, Top, None };

	enum class Corner { LeftTop, RightTop, RightBottom, LeftBottom };

	enum class TwoDirection { Horizontal, Vertical };

	enum class SpriteMode { Default, Sliced, Tiled, FixedAspect, FillLeftToRight, FillRightToLeft, FillUpToDown, FillDownToUp, 
		Fill360CW, Fill360CCW };

	enum class VerAlign { Top, Middle, Bottom, Both };
	enum class HorAlign { Left, Middle, Right, Both };

	enum class ProtectSection { Public, Private, Protected };

	enum class Platform { Windows, MacOSX, iOS, Android };

	enum class LineType { Solid, Dash };

	enum class PrimitiveType { Polygon, PolygonWire, Line };

	enum class PixelFormat { R8G8B8A8, R8G8B8 };

	enum class Loop { None, Repeat, PingPong };

	enum class Units { Pixels, Centimeters, Millimeters, Inches };
}

PRE_ENUM_META(o2::BaseCorner);

PRE_ENUM_META(o2::CursorType);

PRE_ENUM_META(o2::Side);

PRE_ENUM_META(o2::Corner);

PRE_ENUM_META(o2::TwoDirection);

PRE_ENUM_META(o2::SpriteMode);

PRE_ENUM_META(o2::VerAlign);

PRE_ENUM_META(o2::HorAlign);

PRE_ENUM_META(o2::ProtectSection);

PRE_ENUM_META(o2::Platform);

PRE_ENUM_META(o2::LineType);

PRE_ENUM_META(o2::PrimitiveType);

PRE_ENUM_META(o2::PixelFormat);

PRE_ENUM_META(o2::Loop);

PRE_ENUM_META(o2::Units);
