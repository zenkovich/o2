#pragma once

#include "ScrollView.h"

namespace o2
{
	class UIHorizontalScrollBar;
	class UIVerticalScrollBar;
}

namespace Editor
{
	// ----------------------------------------
	// Scroll view, where view limited by frame
	// ----------------------------------------
	class UIFrameScrollView: public UIScrollView
	{
	public:
		// Default constructor
		UIFrameScrollView();

		// Copy-constructor
		UIFrameScrollView(const UIFrameScrollView& other);

		// Destructor
		~UIFrameScrollView();

		// Copy-operator
		UIFrameScrollView& operator=(const UIFrameScrollView& other);

		// Draws widget, updates render target 
		void Draw();

		// Updates drawables, states and widget
		void Update(float dt);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Sets horizontal scrollbar
		void SetHorScrollbar(UIHorizontalScrollBar* scrollbar);

		// Sets vertical scrollbar
		void SetVerScrollbar(UIVerticalScrollBar* scrollbar);

		// Sets view area
		void SetViewArea(const RectF& area);

		// Returns view area
		RectF GetViewArea() const;

		SERIALIZABLE(UIFrameScrollView);

	protected:
		UIHorizontalScrollBar* mHorScrollbar = nullptr; // Horizontal view scrollbar @SERIALIZABLE
		UIVerticalScrollBar*   mVerScrollbar = nullptr; // Vertical view scrollbar @SERIALIZABLE
		RectF                  mAvailableArea;          // Available viewing area @SERIALIZABLE

	protected:
		// Updates camera limits
		void UpdateCameraLimits(float dt);

		// Calls when horizontal scroll bar was scrolled
		void OnHorScrollScrolled(float value);

		// Calls when vertical scroll bar was scrolled
		void OnVerScrollScrolled(float value);

		// Calls when camera position was changed
		void OnCameraTransformChanged();
	};
}
