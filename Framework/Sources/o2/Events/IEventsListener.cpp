#include "o2/stdafx.h"
#include "IEventsListener.h"

#include "o2/Events/EventSystem.h"

namespace o2
{
	IEventsListener::IEventsListener() :
		mEnabledListeningEvents(EventSystem::eventsListenersEnabledByDefault)
	{ }

	IEventsListener::~IEventsListener()
	{ }

	bool IEventsListener::IsListeningEvents() const
	{
		return mEnabledListeningEvents;
	}

	void IEventsListener::SetListeningEvents(bool isListening)
	{
		mEnabledListeningEvents = isListening;
	}

}
