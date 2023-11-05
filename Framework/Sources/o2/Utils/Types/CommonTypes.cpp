#include "o2/stdafx.h"
#include "CommonTypes.h"

#include "o2/Utils/Reflection/Reflection.h"
// --- META ---

ENUM_META(o2::BaseCorner)
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

ENUM_META(o2::CursorType)
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

ENUM_META(o2::Side)
{
    ENUM_ENTRY(Bottom);
    ENUM_ENTRY(Left);
    ENUM_ENTRY(None);
    ENUM_ENTRY(Right);
    ENUM_ENTRY(Top);
}
END_ENUM_META;

ENUM_META(o2::Corner)
{
    ENUM_ENTRY(LeftBottom);
    ENUM_ENTRY(LeftTop);
    ENUM_ENTRY(RightBottom);
    ENUM_ENTRY(RightTop);
}
END_ENUM_META;

ENUM_META(o2::TwoDirection)
{
    ENUM_ENTRY(Horizontal);
    ENUM_ENTRY(Vertical);
}
END_ENUM_META;

ENUM_META(o2::SpriteMode)
{
    ENUM_ENTRY(Default);
    ENUM_ENTRY(Fill360CCW);
    ENUM_ENTRY(Fill360CW);
    ENUM_ENTRY(FillDownToUp);
    ENUM_ENTRY(FillLeftToRight);
    ENUM_ENTRY(FillRightToLeft);
    ENUM_ENTRY(FillUpToDown);
    ENUM_ENTRY(FixedAspect);
    ENUM_ENTRY(Sliced);
    ENUM_ENTRY(Tiled);
}
END_ENUM_META;

ENUM_META(o2::VerAlign)
{
    ENUM_ENTRY(Both);
    ENUM_ENTRY(Bottom);
    ENUM_ENTRY(Middle);
    ENUM_ENTRY(Top);
}
END_ENUM_META;

ENUM_META(o2::HorAlign)
{
    ENUM_ENTRY(Both);
    ENUM_ENTRY(Left);
    ENUM_ENTRY(Middle);
    ENUM_ENTRY(Right);
}
END_ENUM_META;

ENUM_META(o2::ProtectSection)
{
    ENUM_ENTRY(Private);
    ENUM_ENTRY(Protected);
    ENUM_ENTRY(Public);
}
END_ENUM_META;

ENUM_META(o2::Platform)
{
    ENUM_ENTRY(Android);
    ENUM_ENTRY(Linux);
    ENUM_ENTRY(Mac);
    ENUM_ENTRY(Windows);
    ENUM_ENTRY(iOS);
}
END_ENUM_META;

ENUM_META(o2::DeviceType)
{
    ENUM_ENTRY(PC);
    ENUM_ENTRY(Phone);
    ENUM_ENTRY(Tablet);
}
END_ENUM_META;

ENUM_META(o2::LineType)
{
    ENUM_ENTRY(Dash);
    ENUM_ENTRY(Solid);
}
END_ENUM_META;

ENUM_META(o2::PrimitiveType)
{
    ENUM_ENTRY(Line);
    ENUM_ENTRY(Polygon);
    ENUM_ENTRY(PolygonWire);
}
END_ENUM_META;

ENUM_META(o2::PixelFormat)
{
    ENUM_ENTRY(R8G8B8);
    ENUM_ENTRY(R8G8B8A8);
}
END_ENUM_META;

ENUM_META(o2::TextureFormat)
{
    ENUM_ENTRY(DXT5);
    ENUM_ENTRY(R8G8B8A8);
}
END_ENUM_META;

ENUM_META(o2::Loop)
{
    ENUM_ENTRY(None);
    ENUM_ENTRY(PingPong);
    ENUM_ENTRY(Repeat);
}
END_ENUM_META;

ENUM_META(o2::Units)
{
    ENUM_ENTRY(Centimeters);
    ENUM_ENTRY(Inches);
    ENUM_ENTRY(Millimeters);
    ENUM_ENTRY(Pixels);
}
END_ENUM_META;
// --- END META ---
