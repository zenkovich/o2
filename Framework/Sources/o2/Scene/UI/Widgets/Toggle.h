#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/System/ShortcutKeys.h"

namespace o2
{
	class Text;
	class Sprite;
	class Toggle;

	// ------------
	// Toggle group
	// ------------
	class ToggleGroup
	{
	public:
		enum class Type { OnlySingleTrue, VerOneClick, HorOneClick };

	public:
		Function<void(bool)> onPressed;  // Toggle group press event
		Function<void(bool)> onReleased; // Toggle group release event

	public:
		// Contructor by type
		ToggleGroup(Type type);

		// Destructor
		~ToggleGroup();

		// Adds toggle to group
		void AddToggle(Toggle* toggle);

		// Removes toggle from group
		void RemoveToggle(Toggle* toggle);

		// Returns all toggles in group
		const Vector<Toggle*>& GetToggles() const;

		// Returns toggled toggles in group
		const Vector<Toggle*>& GetToggled() const;

	protected:
		bool mPressed = false;      // Is group in pressed state
		bool mPressedValue = false; // Group pressed value

		Vector<Toggle*> mToggles; // All toggles in group
		Vector<Toggle*> mToggled; // Toggled toggles in group

		Toggle* mOwner = nullptr; // Owner toggle

		Type mType; // Toggle group type

	protected:
		// It is called when some toggle was toggled, 
		void OnToggled(Toggle* toggle);

		friend class Toggle;
	};

	class Toggle: public Widget, public DrawableCursorEventsListener, public KeyboardEventsListener
	{
	public:
		PROPERTIES(Toggle);
		PROPERTY(bool, value, SetValue, GetValue);                           // Current state value property
		PROPERTY(WString, caption, SetCaption, GetCaption);                  // Caption property. Searches text layer with name "caption" or creates them if he's not exist
		PROPERTY(ToggleGroup*, toggleGroup, SetToggleGroup, GetToggleGroup); // Toggle group property

	public:
		Function<void()>     onClick;        // Click event
		Function<void(bool)> onToggle;       // Toggle event
		Function<void(bool)> onToggleByUser; // Toggle by user event 

	public:
		ShortcutKeys shortcut;// Shortcut keys

	public:
		// Default constructor
		Toggle();

		// Copy-constructor
		Toggle(const Toggle& other);

		// Assign operator
		Toggle& operator=(const Toggle& other);

		// Destructor
		~Toggle();

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCaption(const WString& text);

		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		WString GetCaption() const;

		// Sets current value
		void SetValue(bool value);

		// Sets value as unknown
		void SetValueUnknown();

		// Is value unknown
		bool IsValueUnknown() const;

		// Returns current value
		bool GetValue() const;

		// Sets toggle group
		void SetToggleGroup(ToggleGroup* toggleGroup);

		// Returns toggle group
		ToggleGroup* GetToggleGroup() const;

		// Returns is this widget can be selected
		bool IsFocusable() const override;

		SERIALIZABLE(Toggle);

	protected:
		bool mValue = false;        // Current value @SERIALIZABLE
		bool mValueUnknown = false; // Is value unknown @SERIALIZABLE

		Text*        mCaptionText = nullptr; // Caption layer text
		WidgetLayer* mBackLayer = nullptr;   // Background layer

		ToggleGroup* mToggleGroup = nullptr; // Toggle group

	protected:
		// It is called when deserialized
		void OnDeserialized(const DataNode& node) override;

		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when layer added and updates drawing sequence
		void OnLayerAdded(WidgetLayer* layer) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

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

		friend class ToggleGroup;
	};
}

CLASS_BASES_META(o2::Toggle)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::Toggle)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(toggleGroup);
	PUBLIC_FIELD(onClick);
	PUBLIC_FIELD(onToggle);
	PUBLIC_FIELD(onToggleByUser);
	PUBLIC_FIELD(shortcut);
	PROTECTED_FIELD(mValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mValueUnknown).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCaptionText);
	PROTECTED_FIELD(mBackLayer);
	PROTECTED_FIELD(mToggleGroup);
}
END_META;
CLASS_METHODS_META(o2::Toggle)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetValue, bool);
	PUBLIC_FUNCTION(void, SetValueUnknown);
	PUBLIC_FUNCTION(bool, IsValueUnknown);
	PUBLIC_FUNCTION(bool, GetValue);
	PUBLIC_FUNCTION(void, SetToggleGroup, ToggleGroup*);
	PUBLIC_FUNCTION(ToggleGroup*, GetToggleGroup);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnLayerAdded, WidgetLayer*);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
}
END_META;
