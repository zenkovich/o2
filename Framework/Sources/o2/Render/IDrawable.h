#pragma once

#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	// -------------------------------
	// Basic drawable object interface
	// -------------------------------
	class IDrawable: public RefCounterable
	{
	public:
		Function<void()> onDraw; // Draw event

	public:
		virtual ~IDrawable() {}

		// Draws drawable
		virtual void Draw();

		// Returns true if point is under drawable
		virtual bool IsUnderPoint(const Vec2F& point);

	protected:
		RectF mDrawingScissorRect; // Scissor rectangle at last drawing

	protected:
		// Called when drawable was drawn. Storing render scissor rect, calling onDraw callback
		virtual void OnDrawn();
	};
}
