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
	class IntegerProperty;

	// ----------------------
	// Editor vector property
	// ----------------------
	class VectorProperty: public IPropertyField
	{
	public:
		// Default constructor
		VectorProperty();

		// Destructor
		~VectorProperty();

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

		IOBJECT(VectorProperty);

	protected:
		struct PropertyDef
		{
			IPropertyField* propertyField = nullptr;
			UILabel*        nameLabel = nullptr;
			UIWidget*       widget = nullptr;

			bool operator==(const PropertyDef& other) const { return propertyField == other.propertyField; }
		};
		typedef Vector<PropertyDef> PropertyFieldsVec;

		const VectorType* mType = nullptr;             // Vector type
		TargetsVec        mTargetObjects;              // Target objects
		UIVerticalLayout* mLayout = nullptr;           // Property layout
		UILabel*          mNameLabel = nullptr;        // Name label
		UIButton*         mExpandButton = nullptr;     // Spoiler expanding button
		UISpoiler*        mSpoiler = nullptr;          // Properties spoiler
		UIVerticalLayout* mPropertiesLayout = nullptr; // Properties vertical layout
						 						    
		PropertyFieldsVec mValueProperties;            // Values properties
		PropertyFieldsVec mValuePropertiesPool;        // Unused value properties pool
		IntegerProperty*  mCountProperty = nullptr;    // Vector count property
		bool              mCountDifferents = false;    // Is targets counts of elements differents
		int               mCountOfElements;            // Common count of elements

		bool              mIsRefreshing = false;       // Is currently refreshing content. Need to prevent cycled size changing

	protected:
		// Returns free element property
		PropertyDef GetFreeValueProperty();

		// Frees element property
		void FreeValueProperty(PropertyDef def);

		// It is called when count property changing
		void OnCountChanged();
	};
}

CLASS_BASES_META(Editor::VectorProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::VectorProperty)
{
	PROTECTED_FIELD(mType);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mLayout);
	PROTECTED_FIELD(mNameLabel);
	PROTECTED_FIELD(mExpandButton);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mPropertiesLayout);
	PROTECTED_FIELD(mValueProperties);
	PROTECTED_FIELD(mValuePropertiesPool);
	PROTECTED_FIELD(mCountProperty);
	PROTECTED_FIELD(mCountDifferents);
	PROTECTED_FIELD(mCountOfElements);
	PROTECTED_FIELD(mIsRefreshing);
}
END_META;
CLASS_METHODS_META(Editor::VectorProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PROTECTED_FUNCTION(PropertyDef, GetFreeValueProperty);
	PROTECTED_FUNCTION(void, FreeValueProperty, PropertyDef);
	PROTECTED_FUNCTION(void, OnCountChanged);
}
END_META;
