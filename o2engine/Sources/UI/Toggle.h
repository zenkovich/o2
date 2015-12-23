#pragma once

#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/Widget.h"

namespace o2
{
	class Text;
	class Sprite;

	class UIToggle: public UIWidget, public CursorEventsListener, public KeyboardEventsListener
	{
	public:
		Property<WString>    caption;  // Caption property. Searches text layer with name "caption" or creates them if he's not exist
		Property<bool>       value;    // Current state value property
		Function<void()>     onClick;  // Click event
		Function<void(bool)> onToggle; // Toggle event

		// Default constructor
		UIToggle();

		// Copy-constructor
		UIToggle(const UIToggle& other);

		// Assign operator
		UIToggle& operator=(const UIToggle& other);

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCaption(const WString& text);

		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		WString GetCaption() const;

		// Sets current value
		void SetValue(bool value);

		// Returns current value
		bool GetValue() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is this widget can be selected
		bool IsSelectable() const;

		SERIALIZABLE(UIToggle);

	protected:
		bool               mValue;       // Current value
		Ptr<Text>          mCaptionText; // Caption layer text
		Ptr<UIWidgetLayer> mBackLayer;   // Background layer

	protected:
		// Calls when cursor pressed on this. Sets state "pressed" to true
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time). Sets state "pressed" to false.
		// Calls onClicked if cursor is still above this
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor enters this object. Sets state "select" to true
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object. Sets state "select" to false
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// Calls when key was released
		void OnKeyReleased(const Input::Key& key);

		// Calls when layer added and updates drawing sequence
		void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Calls when visible was changed
		void OnVisibleChanged();

		// Initializes properties
		void InitializeProperties();
	};
}