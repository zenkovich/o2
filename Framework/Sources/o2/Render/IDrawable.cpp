#include "o2/stdafx.h"
#include "IDrawable.h"

#include "Render.h"

namespace o2
{
	void IDrawable::Draw()
	{
		OnDrawn();
	}

	bool IDrawable::IsUnderPoint(const Vec2F& point)
	{
		return false;
	}

	void IDrawable::OnDrawn()
	{
		mDrawingScissorRect = o2Render.GetResScissorRect();
		onDraw();
	}
}
