#pragma once

namespace o2
{
	// ---------------------------
	// Application events listener
	// ---------------------------
	class ApplicationEventsListener
	{
	public:
		// Default constructor
		ApplicationEventsListener();

		// Virtual destructor
		virtual ~ApplicationEventsListener();

		// Calls when application was started
		virtual void OnApplicationStarted();

		// Calls when application began closing
		virtual void OnApplicationClosing();

		// Calls when application was activated
		virtual void OnApplicationActivated();

		// Calls when application was deactivated
		virtual void OnApplicationDeactivated();

		// Calls when application frame was resized
		virtual void OnApplicationSized();
	};
}