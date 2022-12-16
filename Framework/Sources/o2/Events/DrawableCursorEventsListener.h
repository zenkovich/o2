#pragma once

#include "o2/Events/CursorAreaEventsListener.h"

namespace o2
{
	// --------------------------------------------------------------------
	// Drawable cursor events listener. Reacts to something drawable object
	// --------------------------------------------------------------------
	class DrawableCursorEventsListener: virtual public CursorAreaEventsListener
	{
	public:
		PROPERTIES(DrawableCursorEventsListener);
		PROPERTY(IDrawable*, eventHandleDrawable, SetEventHandleDrawable, GetEventHandleDrawable); // Target event handling drawable property

	public:
		// Default constructor
		DrawableCursorEventsListener(IDrawable* drawable = nullptr);

		// Copy-constructor
		DrawableCursorEventsListener(const DrawableCursorEventsListener& other);

		// Destructor
		~DrawableCursorEventsListener();

		// Sets event handling drawable object
		void SetEventHandleDrawable(IDrawable* drawable);

		// Returns current event handling drawable object
		IDrawable* GetEventHandleDrawable() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

	protected:
		IDrawable* mEventHandleDrawable;

	protected:
		// Called when listener was drawn
		void OnDrawn();
	};
}
