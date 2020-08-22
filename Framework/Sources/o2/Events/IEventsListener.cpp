#include "o2/stdafx.h"
#include "IEventsListener.h"

#include "o2/Events/EventSystem.h"
#include "o2/Utils/Editor/EditorScope.h"

namespace o2
{
	IEventsListener::IEventsListener():
		mEnabledListeningEvents(EventSystem::eventsListenersEnabledByDefault)
	{
		if constexpr (IS_EDITOR)
			mIsEditorMode = EditorScope::IsInScope();
	}

	IEventsListener::~IEventsListener()
	{ }

	bool IEventsListener::IsListeningEvents() const
	{
		if constexpr (IS_EDITOR)
			return mEnabledListeningEvents && mIsEditorMode == EditorScope::IsInScope();
		else
			return mEnabledListeningEvents;
	}

	void IEventsListener::SetListeningEvents(bool isListening)
	{
		mEnabledListeningEvents = isListening;
	}

}
