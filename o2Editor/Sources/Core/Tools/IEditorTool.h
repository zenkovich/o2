#pragma once

#include "Application/Input.h"
#include "Utils/IObject.h"
#include "Utils/Data/DataNode.h"

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

		// It is called when scene actors was changed
		virtual void OnSceneChanged(Vector<Actor*> changedActors) {}

		// Updates tool
		virtual void Update(float dt) {}

		// It is called when tool was enabled
		virtual void OnEnabled() {}

		// It is called when tool was disabled
		virtual void OnDisabled() {}

		// It is called when actors selection was changed
		virtual void OnActorsSelectionChanged(Vector<Actor*> actors) {}

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
