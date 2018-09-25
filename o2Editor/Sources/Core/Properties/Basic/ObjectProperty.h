#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UISpoiler;
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

		// Copy constructor
		ObjectProperty(const ObjectProperty& other);

		// Copy operator
		ObjectProperty& operator=(const ObjectProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Specializes field type
		void SpecializeType(const Type* type) override;

		// Returns specialized type
		const Type* GetSpecializedType() const override;

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

		IOBJECT(ObjectProperty);

	protected:
		const Type*         mObjectType = nullptr;          // Type of target objects

		bool                mPropertiesInitialized = false; // True when properties were built and initialized
		TargetsVec          mTargetObjects;                 // Target objects
		FieldPropertiesInfo mFieldProperties;               // Field properties information

		UISpoiler*          mSpoiler = nullptr;             // Properties spoiler

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when expanding spoiler, initializes properties
		void OnExpand();

		// Returns object proxy pointer
		IObject* GetProxyPtr(IAbstractValueProxy* proxy) const;
	};
}

CLASS_BASES_META(Editor::ObjectProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectProperty)
{
	PROTECTED_FIELD(mObjectType);
	PROTECTED_FIELD(mPropertiesInitialized);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::ObjectProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(const Type*, GetSpecializedType);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(const FieldPropertiesInfo&, GetPropertiesInfo);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnExpand);
	PROTECTED_FUNCTION(IObject*, GetProxyPtr, IAbstractValueProxy*);
}
END_META;
