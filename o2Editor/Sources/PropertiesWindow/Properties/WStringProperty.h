#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIEditBox;
}

namespace Editor
{
	// -------------------------------
	// Editor string property edit box
	// -------------------------------
	class WStringProperty: public IPropertyField
	{
	public:
		// Default constructor
		WStringProperty();

		// Constructor
		WStringProperty(const Vector<void*>& targets, bool isProperty);

		// Destructor
		~WStringProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		WString GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(WStringProperty);

	protected:
		Function<void(void*, const WString&)> mAssignFunc; // Value assign function
		Function<WString(void*)>              mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;  // Fields' pointers
		WString       mCommonValue;     // Common field value (if not different)
		bool          mValuesDifferent; // Are values different

		UIEditBox*    mEditBox;         // Edit box 

	protected:
		// Initializes widget
		void InitializeWidget();

		// Edit box change event
		void OnEdited(const WString& data);
	};
}