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
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

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

		IOBJECT(VectorProperty);

	protected:
		struct PropertyDef
		{
			IPropertyField* propertyField;
			UILabel*        nameLabel;
			UIWidget*       widget;

			bool operator==(const PropertyDef& other) const { return propertyField == other.propertyField; }
		};
		typedef Vector<PropertyDef> PropertyFieldsVec;

		const VectorType* mType = nullptr;             // Vector type
		Vector<void*>     mTargetObjects;              // Target objects
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

	protected:
		// Returns free element property
		PropertyDef GetFreeValueProperty();

		// Frees element property
		void FreeValueProperty(PropertyDef def);

		// Calls when count property changing
		void OnCountChanged();
	};
}