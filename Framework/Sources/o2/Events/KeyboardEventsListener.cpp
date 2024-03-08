#include "o2/stdafx.h"
#include "KeyboardEventsListener.h"

#include "o2/Events/EventSystem.h"

namespace o2
{
    KeyboardEventsListener::KeyboardEventsListener()
    {
    }

    KeyboardEventsListener::~KeyboardEventsListener()
    {
        EventSystem::UnregKeyboardListener(this);
    }

	void KeyboardEventsListener::RefCounterInitialized()
	{
		EventSystem::RegKeyboardListener(this);
	}

	void KeyboardEventsListener::OnKeyPressed(const Input::Key& key)
    {}

    void KeyboardEventsListener::OnKeyReleased(const Input::Key& key)
    {}

    void KeyboardEventsListener::OnKeyStayDown(const Input::Key& key)
    {}
}
