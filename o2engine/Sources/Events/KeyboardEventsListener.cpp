#include "KeyboardEventsListener.h"

#include "Events/EventSystem.h"

namespace o2
{
	KeyboardEventsListener::KeyboardEventsListener()
	{
		o2Events.mKeyboardListeners.Add(this);
	}

	KeyboardEventsListener::~KeyboardEventsListener()
	{
		o2Events.mKeyboardListeners.Remove(this);
	}

	void KeyboardEventsListener::OnKeyPressed(const Input::Key& key)
	{}

	void KeyboardEventsListener::OnKeyReleased(const Input::Key& key)
	{}

	void KeyboardEventsListener::OnKeyStayDown(const Input::Key& key)
	{}
}