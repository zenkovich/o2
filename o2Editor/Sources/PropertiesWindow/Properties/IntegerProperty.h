#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIEditBox;
}

// --------------------------------
// Editor integer property edit box
// --------------------------------
class EditorIntegerProperty: public IEditorPropertyField
{
public:
	// Default constructor
	EditorIntegerProperty();

	// Constructor
	EditorIntegerProperty(const Vector<void*>& targets, bool isProperty);

	// Destructor
	~EditorIntegerProperty();

	// Sets fields
	void Setup(const Vector<void*>& targets, bool isProperty);

	// Updates and checks value
	void Update();

	// Returns root widget
	UIWidget* GetWidget() const;

	// Returns value
	int GetCommonValue() const;

	// Returns is values different
	bool IsValuesDifferent() const;

	// Returns editing by this field type
	const Type* GetFieldType() const;

	IOBJECT(EditorIntegerProperty);

protected:
	Function<void(void*, const int&)> mAssignFunc; // Value assign function
	Function<int(void*)>              mGetFunc;    // Get value function

	Vector<void*> mValuesPointers;  // Fields' pointers
	int           mCommonValue;     // Common field value (if not different)
	bool          mValuesDifferent; // Are values different

	UIEditBox*    mEditBox;         // Edit box 

protected:
	// Initializes widget
	void InitializeWidget();

	// Edit box change event
	void OnEdited(const WString& data);
};
