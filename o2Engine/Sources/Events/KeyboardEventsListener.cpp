#include "stdafx.h"
#include "KeyboardEventsListener.h"

#include "Events/EventSystem.h"

namespace o2
{
	KeyboardEventsListener::KeyboardEventsListener()
	{
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