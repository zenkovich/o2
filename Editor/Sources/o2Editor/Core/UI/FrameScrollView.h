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
		// Updates camera limits
		void UpdateCameraLimits(float dt);

		// Called when horizontal scroll bar was scrolled
		void OnHorScrollScrolled(float value);

		// Called when vertical scroll bar was scrolled
		void OnVerScrollScrolled(float value);

		// Called when camera position was changed
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
	FIELD().PUBLIC().NAME(onViewChanged);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mHorScrollbar);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mVerScrollbar);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mAvailableArea);
}
END_META;
CLASS_METHODS_META(Editor::FrameScrollView)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const FrameScrollView&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHorScrollbar, HorizontalScrollBar*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetVerScrollbar, VerticalScrollBar*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetViewArea, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetViewArea);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateCameraLimits, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnHorScrollScrolled, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnVerScrollScrolled, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCameraTransformChanged);
}
END_META;
