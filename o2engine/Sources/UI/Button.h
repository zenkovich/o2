#pragma once

#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/Widget.h"

namespace o2
{
	class Sprite;
	class Text;

	// -----------------------
	// Button clickable widget
	// -----------------------
	class UIButton: public UIWidget, public CursorEventsListener, public KeyboardEventsListener
	{
	public:
		Property<WString>     caption; // Caption property. Searches text layer with name "caption" or creates them if he's not exist
		Property<Ptr<Sprite>> icon;    // Icon image asset setter. Searches sprite layer with name "icon". Creates him if can't find
		Function<void()>      onClick; // Click event

		// Default constructor
		UIButton();

		// Copy-constructor
		UIButton(const UIButton& other);

		// Assign operator
		UIButton& operator=(const UIButton& other);

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCaption(const WString& text);
		
		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		WString GetCaption() const;

		// Sets icon sprite. Searches sprite layer "icon". Creates a new icon if isn't exist
		void SetIcon(Ptr<Sprite> sprite);

		// Returns icon sprite
		Ptr<Sprite> GetIcon() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is this widget can be selected
		bool IsSelectable() const;

		// Returns interactable flag
		bool IsInteractable() const;

		SERIALIZABLE_IMPL(UIButton);

		IOBJECT(UIButton)
		{
			BASE_CLASS(UIWidget);

			FIELD(caption);
			FIELD(icon);
		}

	protected:
		Ptr<Text>   mCaptionText; // Caption layer text
		Ptr<Sprite> mIconSprite;  // Icon layer sprite

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

		// Initializes properties
		void InitializeProperties();
	};
}
