#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/IDrawable.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"

namespace o2
{
	class Sprite;

	// -----------
	// Drag handle
	// -----------
	class DragHandle: public IDrawable, public CursorAreaEventsListener
	{
	public:
		Sprite*                      regularSprite; // Regular view sprite
		Sprite*                      hoverSprite;   // Hovered view sprite
		Sprite*                      pressedSprite; // Pressed view sprite
		Function<void(const Vec2F&)> onChangedPos;  // On position changed event
		Property<Vec2F>              position;      // Current position property

		// Default constructor
		DragHandle();

		// Constructor with views
		DragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr);

		// Copy-constructor
		DragHandle(const DragHandle& other);

		// Destructor
		~DragHandle();

		// Copy-operator
		DragHandle& operator=(const DragHandle& other);

		// Draws handle
		void Draw();

		// Returns true if point is above this
		bool IsUnderPoint(const Vec2F& point);

		// Sets position
		void SetPosition(const Vec2F& position);

		// Returns position
		Vec2F GetPosition() const;

	protected:
		Vec2F mPosition; // Current handle position

	protected:
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Initializes properties
		void InitializeProperties();
	};
}
