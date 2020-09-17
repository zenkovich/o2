#pragma once

#include "ScrollView.h"

namespace o2
{
	class HorizontalScrollBar;
	class VerticalScrollBar;
}

namespace Editor
{
	// ----------------------------------------
	// Scroll view, where view limited by frame
	// ----------------------------------------
	class FrameScrollView: public ScrollView
	{
	public:
		Function<void()> onViewChanged; // Camera move or zoom event

	public:
		// Default constructor
		FrameScrollView();

		// Copy-constructor
		FrameScrollView(const FrameScrollView& other);

		// Destructor
		~FrameScrollView();

		// Copy-operator
		FrameScrollView& operator=(const FrameScrollView& other);

		// Draws widget, updates render target 
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Updates layout
		void UpdateSelfTransform() override;

		// Sets horizontal scrollbar
		void SetHorScrollbar(HorizontalScrollBar* scrollbar);

		// Sets vertical scrollbar
		void SetVerScrollbar(VerticalScrollBar* scrollbar);

		// Sets view area
		void SetViewArea(const RectF& area);

		// Returns view area
		RectF GetViewArea() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(FrameScrollView);

	protected:
		HorizontalScrollBar* mHorScrollbar = nullptr; // Horizontal view scrollbar @SERIALIZABLE
		VerticalScrollBar*   mVerScrollbar = nullptr; // Vertical view scrollbar @SERIALIZABLE
		RectF                mAvailableArea;          // Available viewing area @SERIALIZABLE

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

CLASS_BASES_META(Editor::FrameScrollView)
{
	BASE_CLASS(Editor::ScrollView);
}
END_META;
CLASS_FIELDS_META(Editor::FrameScrollView)
{
	PUBLIC_FIELD(onViewChanged);
	PROTECTED_FIELD(mHorScrollbar).DEFAULT_VALUE(nullptr).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerScrollbar).DEFAULT_VALUE(nullptr).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAvailableArea).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::FrameScrollView)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(void, SetHorScrollbar, HorizontalScrollBar*);
	PUBLIC_FUNCTION(void, SetVerScrollbar, VerticalScrollBar*);
	PUBLIC_FUNCTION(void, SetViewArea, const RectF&);
	PUBLIC_FUNCTION(RectF, GetViewArea);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateCameraLimits, float);
	PROTECTED_FUNCTION(void, OnHorScrollScrolled, float);
	PROTECTED_FUNCTION(void, OnVerScrollScrolled, float);
	PROTECTED_FUNCTION(void, OnCameraTransformChanged);
}
END_META;
