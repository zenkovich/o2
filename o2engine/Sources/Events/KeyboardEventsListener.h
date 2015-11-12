#pragma once

#include "Application/Input.h"

namespace o2
{
	// ------------------------
	// Keyboard events listener
	// ------------------------
	class KeyboardEventsListener
	{
	public:
		// Default constructor
		KeyboardEventsListener();

		// Virtual destructor
		virtual ~KeyboardEventsListener();

	protected:
		// Calls when key was pressed
		virtual void OnKeyPressed(const Input::Key& key);

		// Calls when key was released
		virtual void OnKeyReleased(const Input::Key& key);

		// Calls when key stay down during frame
		virtual void OnKeyStayDown(const Input::Key& key);

		friend class EventSystem;
	};
}