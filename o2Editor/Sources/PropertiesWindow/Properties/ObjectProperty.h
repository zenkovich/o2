#pragma once

#include "PropertiesWindow/Properties/FieldPropertiesInfo.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UILabel;
	class UISpoiler;
	class UIVerticalLayout;
}

namespace Editor
{
	// ----------------------
	// Editor object property
	// ----------------------
	class ObjectProperty: public IPropertyField
	{
	public:
		// Default constructor
		ObjectProperty();

		// Destructor
		~ObjectProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Specializes field type
		void SpecializeType(const FieldInfo* info);

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		IOBJECT(ObjectProperty);

	protected:
		const Type*         mObjectType;       // Type of target objects
		Vector<IObject*>    mTargetObjects;    // Target objects
		FieldPropertiesInfo mFieldProperties;  // Field properties information
		UIVerticalLayout*   mLayout;           // Property layout
		UILabel*            mNameLabel;        // Name label
		UIButton*           mExpandButton;     // Spoiler expanding button
		UISpoiler*          mSpoiler;          // Properties spoiler
		UIVerticalLayout*   mPropertiesLayout; // Properties vertical layout
	};
}