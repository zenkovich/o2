#pragma once

#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/System/ShortcutKeys.h"

namespace o2
{
	class Text;

	// -------------
	// Button widget
	// -------------
	class Button: public Widget, public CursorAreaEventsListener, public KeyboardEventsListener
	{
	public:
		PROPERTIES(Button);
		PROPERTY(WString, caption, SetCaption, GetCaption); // Caption property. Searches "caption" layer and sets text
		PROPERTY(Sprite*, icon, SetIcon, GetIcon);          // Icon image asset setter. Searches sprite layer with name "icon" and sets image

	public:
		Function<void()>             onClick;       // Click event
		Function<bool(const Vec2F&)> isPointInside; // Checking pointer function. When this empty using default widget pointer check

	public:
		ShortcutKeys shortcut; // Shortcut keys

	public:
		// Default constructor
		Button();

		// Copy-constructor
		Button(const Button& other);

		// Assign operator
		Button& operator=(const Button& other);

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

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(Button);

	protected:
		Text*   mCaptionText = nullptr; // Caption layer text
		Sprite* mIconSprite = nullptr;  // Icon layer sprite

	protected:
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
		void OnLayerAdded(WidgetLayer* layer) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// It is called when listener becomes interactable, disabled "inactive" state when exists
		void OnBecomeInteractable() override;

		// It is called when listener stops interacting, enables "inactive" state when exists
		void OnBecomeNotInteractable() override;
	};
}

CLASS_BASES_META(o2::Button)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::CursorAreaEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::Button)
{
	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(icon);
	PUBLIC_FIELD(onClick);
	PUBLIC_FIELD(isPointInside);
	PUBLIC_FIELD(shortcut);
	PROTECTED_FIELD(mCaptionText).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mIconSprite).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(o2::Button)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnLayerAdded, WidgetLayer*);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnBecomeInteractable);
	PROTECTED_FUNCTION(void, OnBecomeNotInteractable);
}
END_META;
