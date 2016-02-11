#pragma once

#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/Widget.h"
#include "Utils/ShortcutKeys.h"

namespace o2
{
	class Sprite;
	class Text;
	class UIButton;

	// -----------------------------
	// Buttons one cursor down group
	// -----------------------------
	class UIButtonGroup
	{
	public:
		typedef Vector<UIButton*> ButtonsVec;

	public:
		UIButtonGroup();
		~UIButtonGroup();

		void AddButton(UIButton* toggle);
		void RemoveButton(UIButton* toggle);

		const ButtonsVec& GetButtons() const;

	protected:
		ButtonsVec mButtons;
		UIButton*  mOwner;
		bool       mPressed;

		friend class UIButton;
	};

	// -----------------------
	// Button clickable widget
	// -----------------------
	class UIButton: public UIWidget, public CursorEventsListener, public KeyboardEventsListener
	{
	public:
		Property<WString>        caption;      // Caption property. Searches text layer with name "caption" or creates them if he's not exist
		Property<Sprite*>        icon;         // Icon image asset setter. Searches sprite layer with name "icon". Creates him if can't find
		Property<UIButtonGroup*> buttonsGroup; // Buttons group property
		Function<void()>         onClick;      // Click event
		ShortcutKeys             shortcut;     // Shortcut keys

		// Default constructor
		UIButton();

		// Copy-constructor
		UIButton(const UIButton& other);

		// Assign operator
		UIButton& operator=(const UIButton& other);

		// Draws widget
		void Draw();

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCaption(const WString& text);
		
		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		WString GetCaption() const;

		// Sets icon sprite. Searches sprite layer "icon". Creates a new icon if isn't exist
		void SetIcon(Sprite* sprite);

		// Returns icon sprite
		Sprite* GetIcon() const;

		// Sets button group
		void SetButtonGroup(UIButtonGroup* group);

		// Return button group
		UIButtonGroup* GetButtonGroup() const;

		// Returns is this widget can be selected
		bool IsSelectable() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UIButton);

	protected:
		Text*          mCaptionText; // Caption layer text
		Sprite*        mIconSprite;  // Icon layer sprite
		UIButtonGroup* mButtonGroup; // Button group

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
		void OnLayerAdded(UIWidgetLayer* layer);

		// Calls when visible was changed
		void OnVisibleChanged();

		// Initializes properties
		void InitializeProperties();

		friend class UIButtonGroup;
	};
}
