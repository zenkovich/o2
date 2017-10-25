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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Specializes field type
		void SpecializeType(const Type* type) override;

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		// Returns fields properties info
		const FieldPropertiesInfo& GetPropertiesInfo() const;

		IOBJECT(ObjectProperty);

	protected:
		const Type*         mObjectType = nullptr;       // Type of target objects

		TargetsVec          mTargetObjects;              // Target objects
		FieldPropertiesInfo mFieldProperties;            // Field properties information

		UIVerticalLayout*   mLayout = nullptr;           // Property layout
		UILabel*            mNameLabel = nullptr;        // Name label
		UIButton*           mExpandButton = nullptr;     // Spoiler expanding button
		UISpoiler*          mSpoiler = nullptr;          // Properties spoiler
		UIVerticalLayout*   mPropertiesLayout = nullptr; // Properties vertical layout
	};
}
