#pragma once

#include "Application/Input.h"
#include "Events/IEventsListener.h"

namespace o2
{
	// ------------------------
	// Keyboard events listener
	// ------------------------
	class KeyboardEventsListener: public IEventsListener
	{
	public:
		// Default constructor
		KeyboardEventsListener();

		// Virtual destructor
		virtual ~KeyboardEventsListener();

	protected:
		// It is called when key was pressed
		virtual void OnKeyPressed(const Input::Key& key);

		// It is called when key was released
		virtual void OnKeyReleased(const Input::Key& key);

		// It is called when key stay down during frame
		virtual void OnKeyStayDown(const Input::Key& key);

		friend class EventSystem;
	};
}
