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
	class IntegerProperty;

	// ----------------------
	// Editor vector property
	// ----------------------
	class VectorProperty: public IPropertyField
	{
	public:
		// Default constructor
		VectorProperty();

		// Copy constructor
		VectorProperty(const VectorProperty& other);

		// Copy operator
		VectorProperty& operator=(const VectorProperty& other);

		// Destructor
		~VectorProperty();

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

		IOBJECT(VectorProperty);

	protected:
		struct TargetObjectData
		{
			IAbstractValueProxy* proxy = nullptr;
			void* data = nullptr;
			bool isCreated = false;

			bool operator==(const TargetObjectData& other) const { return proxy == other.proxy; }

			void Refresh();
			void SetValue();
		};
		typedef Vector<Pair<TargetObjectData, TargetObjectData>> TargetObjectsVec;

		typedef Vector<IPropertyField*> PropertyFieldsVec;

		const VectorType* mType = nullptr;             // Vector type

		Spoiler*        mSpoiler;                    // Properties spoiler

		TargetObjectsVec  mTargetObjects;              // Target objects
						 						    
		PropertyFieldsVec mValueProperties;            // Values properties
		PropertyFieldsVec mValuePropertiesPool;        // Unused value properties pool
		IntegerProperty*  mCountProperty = nullptr;    // Vector count property
		bool              mCountDifferents = false;    // Is targets counts of elements differents
		int               mCountOfElements = 0;        // Common count of elements

		bool              mIsRefreshing = false;       // Is currently refreshing content. Need to prevent cycled size changing

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Returns value pointer from proxy when proxy is pointer proxy
		void* GetProxyValuePointer(IAbstractValueProxy* proxy) const;

		// Returns free element property
		IPropertyField* GetFreeValueProperty();

		// Frees element property
		void FreeValueProperty(IPropertyField* def);

		// It is called when count property changing
		void OnCountChanged(IPropertyField* def);

		// It is called when expanding spoiler, refreshing array properties
		void OnExpand();

		// Returns object target data from proxy. Creates copy of object when it is property proxy, or gets pointer from pointer proxy
		TargetObjectData GetObjectFromProxy(IAbstractValueProxy* proxy);

		// It is called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after);
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
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mTargetObjects);
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
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void*, GetProxyValuePointer, IAbstractValueProxy*);
	PROTECTED_FUNCTION(IPropertyField*, GetFreeValueProperty);
	PROTECTED_FUNCTION(void, FreeValueProperty, IPropertyField*);
	PROTECTED_FUNCTION(void, OnCountChanged, IPropertyField*);
	PROTECTED_FUNCTION(void, OnExpand);
	PROTECTED_FUNCTION(TargetObjectData, GetObjectFromProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
