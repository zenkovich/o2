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
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSelectionSprite).PROTECTED();
	FIELD().NAME(mCurrentSelectingObjects).PROTECTED();
	FIELD().NAME(mBeforeSelectingObjects).PROTECTED();
	FIELD().NAME(mPressPoint).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mSelectingObjects).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::SelectionTool)
{

	PROTECTED_FUNCTION(void, DrawScene);
	PROTECTED_FUNCTION(void, DrawScreen);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnObjectsSelectionChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
}
END_META;
