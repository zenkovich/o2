#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"

namespace o2
{
	class UIWidget;
	class Text;
	class Actor;
}

namespace Editor
{
	// -------------------------
	// Editor actor property box
	// -------------------------
	class ActorProperty: public IPropertyField, public DrawableCursorEventsListener, public KeyboardEventsListener
	{
	public:
		// Default constructor
		ActorProperty();

		// Constructor
		ActorProperty(const Vector<void*>& targets, bool isProperty);

		// Destructor
		~ActorProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		Actor* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Sets value actor
		void SetValue(Actor* value);

		IOBJECT(ActorProperty);

	protected:
		Function<void(void*, Actor*)> mAssignFunc; // Value assign function
		Function<Actor*(void*)>       mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;  // Fields' pointers
		Actor*        mCommonValue;     // Common field value (if not different)
		bool          mValuesDifferent; // Are values different

		UIWidget*     mBox;             // Edit box 
		Text*         mNameText;        // Asset name text

	protected:
		// Initializes widget
		void InitializeWidget();

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);
	};
}