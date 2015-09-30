#pragma once

namespace o2
{
	class ApplicationEventsListener
	{
	public:
		ApplicationEventsListener();
		virtual ~ApplicationEventsListener();

		virtual void OnApplicationStarted();
		virtual void OnApplicationClosing();
		virtual void OnApplicationActivated();
		virtual void OnApplicationDeactivated();
		virtual void OnApplicationSized();
	};
}