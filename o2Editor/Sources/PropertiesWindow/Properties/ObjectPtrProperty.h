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
	// ------------------------------
	// Editor object pointer property
	// ------------------------------
	class ObjectPtrProperty: public IPropertyField
	{
	public:
		// Default constructor
		ObjectPtrProperty();

		// Destructor
		~ObjectPtrProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Specializes field type
		void SpecializeType(const Type* type);

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		IOBJECT(ObjectPtrProperty);

	protected:
		const Type*         mObjectType = nullptr;       // Type of target objects
		Vector<IObject**>   mTargetObjects;              // Target objects
		FieldPropertiesInfo mFieldProperties;            // Field properties information
		UIVerticalLayout*   mLayout = nullptr;           // Property layout
		UILabel*            mNameLabel = nullptr;        // Name label
		UIButton*           mExpandButton = nullptr;     // Spoiler expanding button
		UISpoiler*          mSpoiler = nullptr;          // Properties spoiler
		UIVerticalLayout*   mPropertiesLayout = nullptr; // Properties vertical layout
	};
}