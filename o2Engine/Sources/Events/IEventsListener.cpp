#include "stdafx.h"
#include "IEventsListener.h"

#include "Events/EventSystem.h"

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