#include "MathTests.h"

#include "Utils/Math/Math.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Color.h"
#include "Tests.h"

using namespace o2;

void TestMath()
{
	TestVector2();
	TestVertex2();
	TestRect();
	TestColor();
}

void TestVector2()
{
	Vec2F a;
	Vec2F b(3, 4);
	Vec2F c(4, 5);
	Vec2F d(c);
	
	CHECK(b == b, "Vector2 operator== fail");
	CHECK(b != c, "Vector2 operator!= fail");
	CHECK(Math::Equals(b + c, Vec2F(7, 9)), "Vector2 operator+ fail");
	CHECK(Math::Equals(b - c, Vec2F(-1, -1)), "Vector2 operator- fail");
	CHECK(Math::Equals(b*c, Vec2F(12, 20)), "Vector2 operator* fail");
	CHECK(Math::Equals(b*2.0f, Vec2F(6, 8)), "Vector2 operator* fail");
	CHECK(Math::Equals(b/c, Vec2F(3/4.0f, 4/5.0f)), "Vector2 operator/ fail");
	CHECK(Math::Equals(b[0], 3.0f) && Math::Equals(b[1], 4.0f), "Vector2 operator[] fail");
	CHECK(Math::Equals(b^c, -1.0f), "Vector2 operator^ fail");
	CHECK(Math::Equals(b^1.0f, Vec2F(-4, 3)), "Vector2 operator^ fail");
	CHECK(Math::Equals(Vec2F::Right().Angle(Vec2F::Up()), Math::Deg2rad(90.0f)), "Vector2 Angle fail");
	CHECK(Math::Equals(Vec2F::Right().ClampLength(2.0f), Vec2F(2.0f, 0.0f)), "Vector2 ClampLength fail");
	CHECK(Math::Equals(b.Cross(c), -1.0f), "Vector2 Cross fail");
	CHECK(Math::Equals(b.Cross(1.0f), Vec2F(-4, 3)), "Vector2 Cross fail");
	CHECK(Math::Equals(b.Dot(c), 32.f), "Vector2 Cross fail");
	CHECK(Math::Equals(b.Get(0), 3.0f) && Math::Equals(b.Get(1), 4.0f), "Vector2 Get fail");
	CHECK(Math::Equals(b.Inverted(), Vec2F(-3, -4)), "Vector2 Inverted fail");
	CHECK(Math::Equals(b.InvertedX(), Vec2F(-3, 4)), "Vector2 InvertedX fail");
	CHECK(Math::Equals(b.InvertedY(), Vec2F(3, -4)), "Vector2 InvertedY fail");
	CHECK(Math::Equals(b.InvScale(c), Vec2F(0.75f, 0.8f)), "Vector2 InvScale fail");
	CHECK(Math::Equals(Vec2F(5, 0).Length(), 5.0f), "Vector2 Length fail");
	CHECK(Math::Equals(Vec2F(5, 0).SqrLength(), 25.0f), "Vector2 SqrLength fail");
	CHECK(Math::Equals(Vec2F(5, 0).Normalized(), Vec2F::Right()), "Vector2 Normalized fail");
	CHECK(Math::Equals(Vec2F(1, 1).Project(Vec2F(2, 0)), Vec2F::Right()), "Vector2 Project fail");
	CHECK(Math::Equals(Vec2F::Right().Rotate(Math::Deg2rad(90.0f)), Vec2F::Up()), "Vector2 Rotate fail");
	CHECK(Math::Equals(b.Scale(c), Vec2F(12, 20)), "Vector2 Scale fail");
	CHECK(Math::Equals(b.Scale(2.0f), Vec2F(6, 8)), "Vector2 Scale fail");

	printf("Vector2 Tested\n");
}

void TestVertex2()
{
	Vertex2 a;
	Vertex2 b(Vec2F::One(), 0x33333333, 0.5f, 0.5f);
	Vec2F c = b;

	printf("Vertex2 Tested\n");
}

void TestRect()
{
	RectF a;
	RectF b(1, 2, 3, 4);
	RectF c(Vec2F(), Vec2F::One());
	RectF d(c);

	CHECK(d == c, "Rect operator== fail");
	CHECK(d != b, "Rect operator!= fail");
	CHECK(Math::Equals(c*2.0f, RectF(0, 0, 2, 2)), "Rect operator* fail");
	CHECK(Math::Equals(c*Vec2F(2, 3), RectF(0, 0, 2, 3)), "Rect operator* fail");
	CHECK(Math::Equals(c/2.0f, RectF(0, 0, 0.5f, 0.5f)), "Rect operator/ fail");
	CHECK(Math::Equals(c/Vec2F(2, 4), RectF(0, 0, 0.5f, 0.25f)), "Rect operator/ fail");
	CHECK(Math::Equals(c + Vec2F(2, 3), RectF(2, 3, 3, 4)), "Rect operator+ fail");
	CHECK(Math::Equals(c + b, RectF(0, 0, 3, 4)), "Rect operator+ fail");
	CHECK(Math::Equals(c.AddSize(Vec2F(2, 3)), RectF(0, 0, 2, 3)), "Rect AddSize fail");
	CHECK(Math::Equals(c.Center(), Vec2F(0.5f, 0.5f)), "Rect Center fail");
	CHECK(Math::Equals(c.Expand(b), RectF(0, 0, 3, 4)), "Rect Expand fail");
	CHECK(Math::Equals(RectF(0, 0, 2, 2).GetIntersection(RectF(1, 1, 3, 3)), RectF(1, 1, 2, 2)), "Rect GetIntersection fail");
	CHECK(Math::Equals(c.Height(), 1.0f), "Rect Height fail");
	CHECK(Math::Equals(b.Width(), 2.0f), "Rect Width fail");
	CHECK(c.IsInside(Vec2F(0.5f, 0.5f)) && !c.IsInside(Vec2F(1.5f, 0.5f)), "Rect IsInside fail");
	CHECK(RectF(0, 0, 2, 2).IsIntersects(RectF(1, 1, 3, 3)), "Rect IsIntersects fail");
	CHECK(!c.IsZero() && RectF(0, 0, 0, 0).IsZero(), "Rect IsZero fail");
	CHECK(Math::Equals(c.LeftTop(), Vec2F(0, 1)), "Rect LeftTop fail");
	CHECK(Math::Equals(c.LeftBottom(), Vec2F(0, 0)), "Rect LeftBottom fail");
	CHECK(Math::Equals(c.RightTop(), Vec2F(1, 1)), "Rect RightTop fail");
	CHECK(Math::Equals(c.RightBottom(), Vec2F(1, 0)), "Rect RightBottom fail");
	CHECK(Math::Equals(c.Move(Vec2F::One()), RectF(1, 1, 2, 2)), "Rect Move fail");
	CHECK(Math::Equals(c.Position(), Vec2F(0, 1)), "Rect Position fail");
	CHECK(Math::Equals(c.Scale(Vec2F(2, 3)), RectF(0, 0, 2, 3)), "Rect Scale fail");
	CHECK(Math::Equals(c.Scale(2.0f), RectF(0, 0, 2, 2)), "Rect Scale fail");
	CHECK(Math::Equals(c.Size(), Vec2F(1, 1)), "Rect Size fail");

	printf("Rect Tested\n");
}

void TestColor()
{
	Color4 a;
	Color4 b(1.0f, 1.0f, 1.0f, 1.0f);
	Color4 c(255, 0, 0, 0);
	Color4 d(c);
	Color4 f(0.1f, 0.2f, 0.3f, 0.4f);

	CHECK(d == c, "Color4 operator== fail");
	CHECK(d != f, "Color4 operator!= fail");
	CHECK(Math::Equals<ULong>(c.ABGR(), 0x000000FF), "Color4 ABGR fail");
	CHECK(Math::Equals<ULong>(c.ARGB(), 0x00FF0000), "Color4 ARGB fail");
	CHECK(Math::Equals(f.RF(), 0.1f, 1.0f/255.0f), "Color4 RF fail");
	CHECK(Math::Equals(f.GF(), 0.2f, 1.0f/255.0f), "Color4 GF fail");
	CHECK(Math::Equals(f.BF(), 0.3f, 1.0f/255.0f), "Color4 BF fail");
	CHECK(Math::Equals(f.AF(), 0.4f, 1.0f/255.0f), "Color4 AF fail");
	CHECK(Math::Equals(Color4::Red() - Color4::Red(), Color4(0, 0, 0, 0)), "COlor4 operator- fail");
	CHECK(Math::Equals((Color4::Red() + Color4::Blue() + Color4::Green()).Normalized(), Color4::White()), "COlor4 operator+ fail");

	printf("Color4 Tested\n");
}
