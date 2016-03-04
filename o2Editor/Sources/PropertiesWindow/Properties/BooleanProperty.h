#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIToggle;
}

// -----------------------
// Editor boolean property
// -----------------------
class EditorBooleanProperty: public IEditorPropertyField
{
public:
	// Default constructor
	EditorBooleanProperty();

	// Constructor
	EditorBooleanProperty(const Vector<void*>& targets, bool isProperty);

	// Destructor
	~EditorBooleanProperty();

	// Sets fields
	void Setup(const Vector<void*>& targets, bool isProperty);

	// Updates and checks value
	void Update();

	// Returns root widget
	UIWidget* GetWidget() const;

	// Returns value
	bool GetCommonValue() const;

	// Returns is values different
	bool IsValuesDifferent() const;

	// Returns editing by this field type
	const Type* GetFieldType() const;

	IOBJECT(EditorBooleanProperty);

protected:
	Function<void(void*, const bool&)> mAssignFunc; // Value assign function
	Function<bool(void*)>              mGetFunc;    // Get value function

	Vector<void*> mValuesPointers;  // Fields' pointers
	bool          mCommonValue;     // Common field value (if not different)
	bool          mValuesDifferent; // Are values different

	UIToggle*     mToggle;          // Toggle 

protected:
	// Initializes widget
	void InitializeWidget();

	// Edit box change event
	void OnEdited(bool value);
};
