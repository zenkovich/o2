#pragma once

#include "Utils/Delegates.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"

namespace o2
{
	// -------------------------------
	// Basic drawable object interface
	// -------------------------------
	class IDrawable
	{
	public:
		Function<void()> onDraw; // Draw event

		virtual ~IDrawable() {}

		// Draws drawable
		virtual void Draw();

		// Returns true if point is under drawable
		virtual bool IsUnderPoint(const Vec2F& point);

	protected:
		RectF mDrawingScissorRect; // Scissor rectangle at last drawing

	protected:
		// It is called when drawable was drawn
		virtual void OnDrawn();
	};
}
