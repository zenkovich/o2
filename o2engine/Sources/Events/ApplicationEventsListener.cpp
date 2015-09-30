#include "ApplicationEventsListener.h"

#include "Events/EventSystem.h"

namespace o2
{
	ApplicationEventsListener::ApplicationEventsListener()
	{
		o2Events.mApplicationListeners.Add(this);
	}

	ApplicationEventsListener::~ApplicationEventsListener()
	{
		o2Events.mApplicationListeners.Remove(this);
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