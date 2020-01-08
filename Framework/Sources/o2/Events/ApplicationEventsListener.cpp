#include "o2/stdafx.h"
#include "ApplicationEventsListener.h"

#include "o2/Events/EventSystem.h"

namespace o2
{
	ApplicationEventsListener::ApplicationEventsListener()
	{
		EventSystem::RegApplicationListener(this);
	}

	ApplicationEventsListener::~ApplicationEventsListener()
	{
		EventSystem::UnregApplicationListener(this);
	}

	void ApplicationEventsListener::OnApplicationStarted()
	{}

	void ApplicationEventsListener::OnApplicationClosing()
	{}

	void ApplicationEventsListener::OnApplicationActivated()
	{}

	void ApplicationEventsListener::OnApplicationDeactivated()
	{}

	void ApplicationEventsListener::OnApplicationSized()
	{}
}
