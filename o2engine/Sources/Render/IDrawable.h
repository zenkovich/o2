#pragma once

namespace o2
{
	class IDrawable
	{
	public:
		virtual void Draw() = 0;
		float GetDrawingDepth() const { return mDrawingDepth; }

	protected:
		float mDrawingDepth = 0;
	};
}