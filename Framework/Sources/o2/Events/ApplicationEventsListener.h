#pragma once

#include "o2/Events/IEventsListener.h"

namespace o2
{
	// ---------------------------
	// Application events listener
	// ---------------------------
	class ApplicationEventsListener: public IEventsListener
	{
	public:
		// Default constructor
		ApplicationEventsListener();

		// Virtual destructor
		virtual ~ApplicationEventsListener();

		// Called when application was started
		virtual void OnApplicationStarted();

		// Called when application began closing
		virtual void OnApplicationClosing();

		// Called when application was activated
		virtual void OnApplicationActivated();

		// Called when application was deactivated
		virtual void OnApplicationDeactivated();

		// Called when application frame was resized
		virtual void OnApplicationSized();
	};
}
