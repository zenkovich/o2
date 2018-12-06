#pragma once

#include "Events/IEventsListener.h"

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

		// It is called when application was started
		virtual void OnApplicationStarted();

		// It is called when application began closing
		virtual void OnApplicationClosing();

		// It is called when application was activated
		virtual void OnApplicationActivated();

		// It is called when application was deactivated
		virtual void OnApplicationDeactivated();

		// It is called when application frame was resized
		virtual void OnApplicationSized();
	};
}
