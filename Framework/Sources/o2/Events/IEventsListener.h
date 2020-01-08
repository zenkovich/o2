#pragma once

namespace o2
{
	// -------------------------------------------------------------------
	// Basic events listener interface. Can be enabler or disabled, 
	// that setting up in constructor by global setting from events system
	// -------------------------------------------------------------------
	class IEventsListener
	{
	public:
		// Default constructor. Takes global setting from events system and enables or disables this
		IEventsListener();

		// Virtual destructor
		virtual ~IEventsListener();

		// Returns is listener enabled
		bool IsListeningEvents() const;

		// Sets listener enabled to listen events
		void SetListeningEvents(bool isListening);

	protected:
		bool mEnabledListeningEvents = true;
	};
}
