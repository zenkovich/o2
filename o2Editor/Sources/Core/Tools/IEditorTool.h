#pragma once

#include "Application/Input.h"
#include "Utils/IObject.h"

using namespace o2;

namespace o2
{
	class Actor;
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

		// Calls when scene actors was changed
		virtual void OnSceneChanged(Vector<Actor*> changedActors) {}

		// Updates tool
		virtual void Update(float dt) {}

		// Calls when tool was enabled
		virtual void OnEnabled() {}

		// Calls when tool was disabled
		virtual void OnDisabled() {}

		// Calls when actors selection was changed
		virtual void OnActorsSelectionChanged(Vector<Actor*> actors) {}

		// Calls when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor) {}

		// Calls when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor) {}

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		virtual void OnCursorPressBreak(const Input::Cursor& cursor) {}

		// Calls when cursor stay down during frame
		virtual void OnCursorStillDown(const Input::Cursor& cursor) {}

		// Calls when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorMoved(const Input::Cursor& cursor) {}

		// Calls when cursor enters this object
		virtual void OnCursorEnter(const Input::Cursor& cursor) {}

		// Calls when cursor exits this object
		virtual void OnCursorExit(const Input::Cursor& cursor) {}

		// Calls when right mouse button was pressed on this
		virtual void OnCursorRightMousePressed(const Input::Cursor& cursor) {}

		// Calls when right mouse button stay down on this
		virtual void OnCursorRightMouseStayDown(const Input::Cursor& cursor) {}

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		virtual void OnCursorRightMouseReleased(const Input::Cursor& cursor) {}

		// Calls when middle mouse button was pressed on this
		virtual void OnCursorMiddleMousePressed(const Input::Cursor& cursor) {}

		// Calls when middle mouse button stay down on this
		virtual void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) {}

		// Calls when middle mouse button was released (only when middle mouse button pressed this at previous time)
		virtual void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) {}

		// Calls when scrolling
		virtual void OnScrolled(float scroll) {}

		// Calls when key was pressed
		virtual void OnKeyPressed(const Input::Key& key) {}

		// Calls when key was released
		virtual void OnKeyReleased(const Input::Key& key) {}

		// Calls when key stay down during frame
		virtual void OnKeyStayDown(const Input::Key& key) {}

		friend class SceneEditScreen;
	};
}