#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/IDrawable.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	class Sprite;

	// -----------
	// Drag handle
	// -----------
	class DragHandle: public IDrawable, public CursorAreaEventsListener, public ISerializable
	{
	public:
		Function<void(const Vec2F&)> onChangedPos;   // On position changed event
		Property<Vec2F>              position;       // Current position property
		Property<bool>               enabled;        // Is handle enabled property. Disabled handle don't drawn and interact

		Function<Vec2F(const Vec2F&)> screenToLocalTransformFunc; // Screen position to local transformation function
		Function<Vec2F(const Vec2F&)> localToScreenTransformFunc; // Local position to screen transformation function
		Function<Vec2F(const Vec2F&)> checkPositionFunc;          // Position constraints checking function

		// Default constructor
		DragHandle();

		// Constructor with views
		DragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr, Sprite* selected = nullptr);

		// Copy-constructor
		DragHandle(const DragHandle& other);

		// Destructor
		virtual ~DragHandle();

		// Copy-operator
		DragHandle& operator=(const DragHandle& other);

		// Draws handle
		void Draw();

		// Returns true if point is above this
		bool IsUnderPoint(const Vec2F& point);

		// Sets position
		void SetPosition(const Vec2F& position);

		// Returns is handle selection
		bool IsSelected() const;

		// Sets handle selection
		void SetSelected(bool selected);

		// Returns position
		Vec2F GetPosition() const;

		// Set handle enabled. Disabled handle don't drawn and interact
		void SetEnabled(bool enabled);

		// Returns is handle enabled. Disabled handle don't drawn and interact
		bool IsEnabled() const;

		SERIALIZABLE(DragHandle);

	protected:
		bool    mEnabled = true;           // Is handle enabled. Disabled handle don't drawn and interact

		Sprite* mRegularSprite = nullptr;  // Regular view sprite @SERIALIZABLE
		Sprite* mHoverSprite = nullptr;    // Hovered view sprite @SERIALIZABLE
		Sprite* mPressedSprite = nullptr;  // Pressed view sprite @SERIALIZABLE
		Sprite* mSelectedSprite = nullptr; // Selected view sprite @SERIALIZABLE

		Vec2F  mPosition;                  // Current handle position, checked by checkPositionFunc
		Vec2F  mDragOffset;                // Dragging offset from cursor in local space to center
		Vec2F  mDragPosition;              // Current drag handle position
			   						       
		bool   mIsHovered = false;         // Is handle under cursor, used for hover sprite appearing
		bool   mIsPressed = false;         // Is handle pressed, used for pressed sprite appearing
		bool   mIsSelected = false;        // Is handle selected, used for selected sprite appearing

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
