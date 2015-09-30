#pragma once

#include "Application/Input.h"

namespace o2
{
	class KeyboardEventsListener
	{
	public:
		KeyboardEventsListener();
		virtual ~KeyboardEventsListener();

		virtual void OnKeyPressed(const Input::Key& key);
		virtual void OnKeyReleased(const Input::Key& key);
		virtual void OnKeyStayDown(const Input::Key& key);

		friend class EventSystem;
	};
}