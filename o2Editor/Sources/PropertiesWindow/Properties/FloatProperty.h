#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIEditBox;
}

// ------------------------------
// Editor float property edit box
// ------------------------------
class EditorFloatProperty: public IEditorPropertyField
{
public:
	// Default constructor
	EditorFloatProperty();

	// Constructor
	EditorFloatProperty(const Vector<void*>& targets, bool isProperty);

	// Destructor
	~EditorFloatProperty();

	// Sets fields
	void Setup(const Vector<void*>& targets, bool isProperty);

	// Updates and checks value
	void Update();

	// Returns root widget
	UIWidget* GetWidget() const;

	// Returns value
	float GetCommonValue() const;

	// Returns is values different
	bool IsValuesDifferent() const;

	// Returns editing by this field type
	const Type* GetFieldType() const;

	IOBJECT(EditorFloatProperty);

protected:
	Function<void(void*, const float&)> mAssignFunc; // Value assign function
	Function<float(void*)>              mGetFunc;    // Get value function

	Vector<void*> mValuesPointers;  // Fields' pointers
	float         mCommonValue;     // Common field value (if not different)
	bool          mValuesDifferent; // Are values different

	UIEditBox*    mEditBox;         // Edit box 

protected:
	// Initializes widget
	void InitializeWidget();

	// Edit box change event
	void OnEdited(const WString& data);
};
