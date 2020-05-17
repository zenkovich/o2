#pragma once

#include "o2/Application/Input.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Serialization/DataValue.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	// ---------------------
	// Editor tool interface
	// ---------------------
	class IEditTool: public IObject
	{
	public:
		virtual ~IEditTool() {}

		IOBJECT(IEditTool);

	protected:
		bool mNeedRedraw = false; // True when need redraw scene

	protected:
		// Draws tool
		virtual void DrawScene() {}

		// Draws tool
		virtual void DrawScreen() {}

		// It is called when scene actors was changed
		virtual void OnSceneChanged(Vector<SceneEditableObject*> changedObjects) {}

		// Updates tool
		virtual void Update(float dt) {}

		// It is called when tool was enabled
		virtual void OnEnabled() {}

		// It is called when tool was disabled
		virtual void OnDisabled() {}

		// It is called when objects selection was changed
		virtual void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects) {}

		// It is called when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor) {}

		// It is called when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor) {}

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		virtual void OnCursorPressBreak(const Input::Cursor& cursor) {}

		// It is called when cursor stay down during frame
		virtual void OnCursorStillDown(const Input::Cursor& cursor) {}

		// It is called when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorMoved(const Input::Cursor& cursor) {}

		// It is called when cursor enters this object
		virtual void OnCursorEnter(const Input::Cursor& cursor) {}

		// It is called when cursor exits this object
		virtual void OnCursorExit(const Input::Cursor& cursor) {}

		// It is called when right mouse button was pressed on this
		virtual void OnCursorRightMousePressed(const Input::Cursor& cursor) {}

		// It is called when right mouse button stay down on this
		virtual void OnCursorRightMouseStayDown(const Input::Cursor& cursor) {}

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		virtual void OnCursorRightMouseReleased(const Input::Cursor& cursor) {}

		// It is called when middle mouse button was pressed on this
		virtual void OnCursorMiddleMousePressed(const Input::Cursor& cursor) {}

		// It is called when middle mouse button stay down on this
		virtual void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) {}

		// It is called when middle mouse button was released (only when middle mouse button pressed this at previous time)
		virtual void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) {}

		// It is called when scrolling
		virtual void OnScrolled(float scroll) {}

		// It is called when key was pressed
		virtual void OnKeyPressed(const Input::Key& key) {}

		// It is called when key was released
		virtual void OnKeyReleased(const Input::Key& key) {}

		// It is called when key stay down during frame
		virtual void OnKeyStayDown(const Input::Key& key) {}

		friend class SceneEditScreen;
	};
}

CLASS_BASES_META(Editor::IEditTool)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IEditTool)
{
	PROTECTED_FIELD(mNeedRedraw);
}
END_META;
CLASS_METHODS_META(Editor::IEditTool)
{

	PROTECTED_FUNCTION(void, DrawScene);
	PROTECTED_FUNCTION(void, DrawScreen);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnObjectsSelectionChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
}
END_META;
