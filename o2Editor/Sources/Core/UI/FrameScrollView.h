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
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Updates layout
		void UpdateSelfTransform() override;

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
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Updates camera limits
		void UpdateCameraLimits(float dt);

		// It is called when horizontal scroll bar was scrolled
		void OnHorScrollScrolled(float value);

		// It is called when vertical scroll bar was scrolled
		void OnVerScrollScrolled(float value);

		// It is called when camera position was changed
		void OnCameraTransformChanged() override;
	};
}

CLASS_BASES_META(Editor::UIFrameScrollView)
{
	BASE_CLASS(Editor::UIScrollView);
}
END_META;
CLASS_FIELDS_META(Editor::UIFrameScrollView)
{
	PROTECTED_FIELD(mHorScrollbar).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerScrollbar).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAvailableArea).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::UIFrameScrollView)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(void, SetHorScrollbar, UIHorizontalScrollBar*);
	PUBLIC_FUNCTION(void, SetVerScrollbar, UIVerticalScrollBar*);
	PUBLIC_FUNCTION(void, SetViewArea, const RectF&);
	PUBLIC_FUNCTION(RectF, GetViewArea);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateCameraLimits, float);
	PROTECTED_FUNCTION(void, OnHorScrollScrolled, float);
	PROTECTED_FUNCTION(void, OnVerScrollScrolled, float);
	PROTECTED_FUNCTION(void, OnCameraTransformChanged);
}
END_META;
