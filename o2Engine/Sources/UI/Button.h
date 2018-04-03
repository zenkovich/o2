#pragma once

#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/Widget.h"
#include "Utils/ShortcutKeys.h"

namespace o2
{
	class Sprite;
	class Text;

	// -------------
	// Button widget
	// -------------
	class UIButton: public UIWidget, public CursorAreaEventsListener, public KeyboardEventsListener
	{
	public:
		PROPERTIES(UIButton);
		PROPERTY(WString, caption, SetCaption, GetCaption); // Caption property. Searches "caption" layer and sets text
		PROPERTY(Sprite*, icon, SetIcon, GetIcon);          // Icon image asset setter. Searches sprite layer with name "icon" and sets image

		Function<void()> onClick;      // Click event
		ShortcutKeys     shortcut;     // Shortcut keys

		// Default constructor
		UIButton();

		// Copy-constructor
		UIButton(const UIButton& other);

		// Assign operator
		UIButton& operator=(const UIButton& other);

		// Draws widget
		void Draw() override;

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCaption(const WString& text);

		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		WString GetCaption() const;

		// Sets icon sprite. Searches sprite layer "icon". Creates a new icon if isn't exist
		void SetIcon(Sprite* sprite);

		// Returns icon sprite
		Sprite* GetIcon() const;

		// Returns is this widget can be selected
		bool IsFocusable() const override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		SERIALIZABLE(UIButton);

	protected:
		Text*   mCaptionText = nullptr; // Caption layer text
		Sprite* mIconSprite = nullptr;  // Icon layer sprite

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when cursor pressed on this. Sets state "pressed" to true
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time). Sets state "pressed" to false.
		// It is called onClicked if cursor is still above this
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object. Sets state "select" to true
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object. Sets state "select" to false
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// It is called when layer added and updates drawing sequence
		void OnLayerAdded(UIWidgetLayer* layer) override;

		// It is called when visible was changed
		void OnVisibleChanged() override;
	};
}

CLASS_BASES_META(o2::UIButton)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::CursorAreaEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::UIButton)
{
	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(icon);
	PUBLIC_FIELD(onClick);
	PUBLIC_FIELD(shortcut);
	PROTECTED_FIELD(mCaptionText);
	PROTECTED_FIELD(mIconSprite);
}
END_META;
CLASS_METHODS_META(o2::UIButton)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
}
END_META;
