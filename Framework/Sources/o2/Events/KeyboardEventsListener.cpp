#include "o2/stdafx.h"
#include "KeyboardEventsListener.h"

#include "o2/Events/EventSystem.h"

namespace o2
{
    KeyboardEventsListener::KeyboardEventsListener(RefCounter* refCounter)
    {
        mRefCounter = refCounter;
        EventSystem::RegKeyboardListener(this);
    }

    KeyboardEventsListener::~KeyboardEventsListener()
    {
        EventSystem::UnregKeyboardListener(this);
    }

    void KeyboardEventsListener::OnKeyPressed(const Input::Key& key)
    {}

    void KeyboardEventsListener::OnKeyReleased(const Input::Key& key)
    {}

    void KeyboardEventsListener::OnKeyStayDown(const Input::Key& key)
    {}
}
