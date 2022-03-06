#pragma once

#include "o2Editor/Core/Tools/IEditorTool.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class SceneEditableObject;
}

namespace Editor
{
	// ---------------------
	// Editor selection tool
	// ---------------------
	class SelectionTool: public IEditTool
	{
	public:
		// Default constructor
		SelectionTool();

		// Destructor
		~SelectionTool();

		IOBJECT(SelectionTool);

	protected:
		Sprite* mSelectionSprite = nullptr; // Selection frame sprite

		Vector<SceneEditableObject*> mCurrentSelectingObjects; // Current selecting objects (when cursor pressed, but not released yet)
		Vector<SceneEditableObject*> mBeforeSelectingObjects;  // Before selection objects array

		Vec2F mPressPoint;				 // Press point before selecting
		bool  mSelectingObjects = false; // Is selecting objects now

	protected:
		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Returns shortcut keys for toggle
		ShortcutKeys GetShortcut() const override;

		// Draws tool
		void DrawScene();

		// Draws tool
		void DrawScreen();

		// Updates tool
		void Update(float dt);

		// It is called when tool was enabled
		void OnEnabled();

		// It is called when tool was disabled
		void OnDisabled();

		// It is called when objects selection was changed
		void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key);
	};

}

CLASS_BASES_META(Editor::SelectionTool)
{
	BASE_CLASS(Editor::IEditTool);
}
END_META;
CLASS_FIELDS_META(Editor::SelectionTool)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSelectionSprite);
	FIELD().PROTECTED().NAME(mCurrentSelectingObjects);
	FIELD().PROTECTED().NAME(mBeforeSelectingObjects);
	FIELD().PROTECTED().NAME(mPressPoint);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mSelectingObjects);
}
END_META;
CLASS_METHODS_META(Editor::SelectionTool)
{

	FUNCTION().PROTECTED().SIGNATURE(String, GetPanelIcon);
	FUNCTION().PROTECTED().SIGNATURE(ShortcutKeys, GetShortcut);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawScene);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawScreen);
	FUNCTION().PROTECTED().SIGNATURE(void, Update, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectsSelectionChanged, Vector<SceneEditableObject*>);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
}
END_META;
