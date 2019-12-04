#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class Spoiler;
}

namespace Editor
{
	class IObjectPropertiesViewer;

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

		// Specializes field info, processing attributes
		void SpecializeFieldInfo(const FieldInfo* fieldInfo) override;

		// Returns specialized type
		const Type* GetSpecializedType() const override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

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
		struct TargetObjectData
		{
			IAbstractValueProxy* proxy = nullptr;
			IObject* data = nullptr;
			bool isCreated = false;

			bool operator==(const TargetObjectData& other) const { return proxy == other.proxy; }

			void Refresh();
			void SetValue();
		};
		typedef Vector<Pair<TargetObjectData, TargetObjectData>> TargetObjectsVec;

	protected:
		const Type* mObjectType = nullptr; // Type of target objects
							     								    
		bool                     mPropertiesInitialized = false;    // True when properties were built and initialized
		TargetObjectsVec         mTargetObjects;                    // Target objects
		IObjectPropertiesViewer* mObjectPropertiesViewer = nullptr; // Object viewer

		Spoiler* mSpoiler = nullptr; // Properties spoiler

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when expanding spoiler, initializes properties
		void OnExpand();

		// Returns object target data from proxy. Creates copy of object when it is property proxy, or gets pointer from pointer proxy
		TargetObjectData GetObjectFromProxy(IAbstractValueProxy* proxy);

		// It is called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after);
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
	PROTECTED_FIELD(mObjectPropertiesViewer);
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::ObjectProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(void, SpecializeFieldInfo, const FieldInfo*);
	PUBLIC_FUNCTION(const Type*, GetSpecializedType);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(Button*, GetRemoveButton);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnExpand);
	PROTECTED_FUNCTION(TargetObjectData, GetObjectFromProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
