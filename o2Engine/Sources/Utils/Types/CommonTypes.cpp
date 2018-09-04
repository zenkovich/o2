#include "stdafx.h"
#include "CommonTypes.h"

#include "Utils/Reflection/Reflection.h"

ENUM_META_(o2::BaseCorner, BaseCorner)
{
	ENUM_ENTRY(Bottom);
	ENUM_ENTRY(Center);
	ENUM_ENTRY(Left);
	ENUM_ENTRY(LeftBottom);
	ENUM_ENTRY(LeftTop);
	ENUM_ENTRY(Right);
	ENUM_ENTRY(RightBottom);
	ENUM_ENTRY(RightTop);
	ENUM_ENTRY(Top);
}
END_ENUM_META;

ENUM_META_(o2::CursorType, CursorType)
{
	ENUM_ENTRY(AppStarting);
	ENUM_ENTRY(Arrow);
	ENUM_ENTRY(Cross);
	ENUM_ENTRY(Hand);
	ENUM_ENTRY(Help);
	ENUM_ENTRY(IBeam);
	ENUM_ENTRY(Icon);
	ENUM_ENTRY(No);
	ENUM_ENTRY(SizeAll);
	ENUM_ENTRY(SizeNS);
	ENUM_ENTRY(SizeNeSw);
	ENUM_ENTRY(SizeNwSe);
	ENUM_ENTRY(SizeWE);
	ENUM_ENTRY(UpArrow);
	ENUM_ENTRY(Wait);
}
END_ENUM_META;

ENUM_META_(o2::Side, Side)
{
	ENUM_ENTRY(Bottom);
	ENUM_ENTRY(Left);
	ENUM_ENTRY(None);
	ENUM_ENTRY(Right);
	ENUM_ENTRY(Top);
}
END_ENUM_META;

ENUM_META_(o2::Corner, Corner)
{
	ENUM_ENTRY(LeftBottom);
	ENUM_ENTRY(LeftTop);
	ENUM_ENTRY(RightBottom);
	ENUM_ENTRY(RightTop);
}
END_ENUM_META;

ENUM_META_(o2::TwoDirection, TwoDirection)
{
	ENUM_ENTRY(Horizontal);
	ENUM_ENTRY(Vertical);
}
END_ENUM_META;

ENUM_META_(o2::SpriteMode, SpriteMode)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(Fill360CCW);
	ENUM_ENTRY(Fill360CW);
	ENUM_ENTRY(FillDownToUp);
	ENUM_ENTRY(FillLeftToRight);
	ENUM_ENTRY(FillRightToLeft);
	ENUM_ENTRY(FillUpToDown);
	ENUM_ENTRY(Sliced);
	ENUM_ENTRY(Tiled);
}
END_ENUM_META;

ENUM_META_(o2::VerAlign, VerAlign)
{
	ENUM_ENTRY(Both);
	ENUM_ENTRY(Bottom);
	ENUM_ENTRY(Middle);
	ENUM_ENTRY(Top);
}
END_ENUM_META;

ENUM_META_(o2::HorAlign, HorAlign)
{
	ENUM_ENTRY(Both);
	ENUM_ENTRY(Left);
	ENUM_ENTRY(Middle);
	ENUM_ENTRY(Right);
}
END_ENUM_META;

ENUM_META_(o2::ProtectSection, ProtectSection)
{
	ENUM_ENTRY(Private);
	ENUM_ENTRY(Protected);
	ENUM_ENTRY(Public);
}
END_ENUM_META;

ENUM_META_(o2::Platform, Platform)
{
	ENUM_ENTRY(Android);
	ENUM_ENTRY(MacOSX);
	ENUM_ENTRY(Windows);
	ENUM_ENTRY(iOS);
}
END_ENUM_META;

ENUM_META_(o2::LineType, LineType)
{
	ENUM_ENTRY(Dash);
	ENUM_ENTRY(Solid);
}
END_ENUM_META;

ENUM_META_(o2::PrimitiveType, PrimitiveType)
{
	ENUM_ENTRY(Line);
	ENUM_ENTRY(Polygon);
	ENUM_ENTRY(PolygonWire);
}
END_ENUM_META;

ENUM_META_(o2::PixelFormat, PixelFormat)
{
	ENUM_ENTRY(R8G8B8);
	ENUM_ENTRY(R8G8B8A8);
}
END_ENUM_META;
