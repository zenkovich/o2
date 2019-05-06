#pragma once

#include "Utils/Editor/DragHandle.h"
#include "Events/CursorEventsListener.h"

namespace o2
{
	class Sprite;
}

using namespace o2;

namespace Editor
{
	class AnimationTimeline;

	// -------------------------------------------
	// Handles sheet, manages selection of handles
	// -------------------------------------------
	class KeyHandlesSheet : public Widget, public SelectableDragHandlesGroup, public CursorAreaEventsListener
	{
	public:
		// Default constructor
		KeyHandlesSheet();

		// Copy-constructor
		KeyHandlesSheet(const KeyHandlesSheet& other);

		// Destructor
		~KeyHandlesSheet();


		// Copy-operator
		KeyHandlesSheet& operator=(const KeyHandlesSheet& other);


		// Updates selection frame
		void Update(float dt) override;

		// Draws selection
		void Draw() override;


		// Sets timeline
		void SetTimeline(AnimationTimeline* timeline);

		SERIALIZABLE(KeyHandlesSheet);

	private:
		AnimationTimeline* mTimeline = nullptr;

		Sprite* mSelectionFrame = nullptr;

	private:
		// It is called when selection is changed - some handle was added or removed from selection
		// Updating selection frame
		void OnSelectionChanged() override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor pressed outside this
		void OnCursorPressedOutside(const Input::Cursor& cursor) override;

		// It is called when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// It is called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when middle mouse button was pressed on this
		void OnCursorMiddleMousePressed(const Input::Cursor& cursor) override;

		// It is called when middle mouse button stay down on this
		void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when middle mouse button was released (only when middle mouse button pressed this at previous time)
		void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(Editor::KeyHandlesSheet)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::SelectableDragHandlesGroup);
	BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::KeyHandlesSheet)
{
	PRIVATE_FIELD(mSelectionFrame);
}
END_META;
CLASS_METHODS_META(Editor::KeyHandlesSheet)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PRIVATE_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorPressedOutside, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorDblClicked, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorMiddleMousePressed, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
	PRIVATE_FUNCTION(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
}
END_META;
