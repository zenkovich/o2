#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"

namespace o2
{
	class UIWidget;
	class Text;
	class Actor;
	class Component;
}

namespace Editor
{
	// -----------------------------------
	// Editor actor component property box
	// -----------------------------------
	class ComponentProperty: public IPropertyField, public DrawableCursorEventsListener, public KeyboardEventsListener
	{
	public:
		// Default constructor
		ComponentProperty(UIWidget* widget = nullptr);

		// Destructor
		~ComponentProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		Component* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Specializes field type
		void SpecializeType(const FieldInfo* info);

		// Returns specialized type
		const Type* GetSpecializedType() const;

		// Sets value actor
		void SetValue(Component* value);

		IOBJECT(ComponentProperty);

	protected:
		Function<void(void*, Component*)> mAssignFunc; // Value assign function
		Function<Component*(void*)>       mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;  // Fields' pointers
		Component*    mCommonValue;     // Common field value (if not different)
		bool          mValuesDifferent; // Are values different
		const Type*   mComponentType;   // Component value type

		UIWidget*     mBox;             // Edit box 
		Text*         mNameText;        // Component name text

	protected:
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