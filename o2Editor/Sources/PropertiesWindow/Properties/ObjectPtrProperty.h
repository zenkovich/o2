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
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Specializes field type
		void SpecializeType(const Type* type) override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

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

		IOBJECT(ObjectPtrProperty);

	protected:
		const Type*         mObjectType = nullptr;          // Type of target objects

		bool                mPropertiesInitialized = false; // True when properties were built and initialized
		TargetsVec          mTargetObjects;                 // Target objects
		FieldPropertiesInfo mFieldProperties;               // Field properties information

		UISpoiler*          mSpoiler;                       // Properties spoiler

	protected:
		// It is called when expanding spoiler, initializes properties
		void OnExpand();
	};
}

CLASS_BASES_META(Editor::ObjectPtrProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectPtrProperty)
{
	PROTECTED_FIELD(mObjectType);
	PROTECTED_FIELD(mPropertiesInitialized);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::ObjectPtrProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(const FieldPropertiesInfo&, GetPropertiesInfo);
	PROTECTED_FUNCTION(void, OnExpand);
}
END_META;
