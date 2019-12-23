#pragma once

#include "Core/Properties/PropertiesContext.h"
#include "Core/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class Button;
	class ContextMenu;
	class Label;
	class Spoiler;
	class Widget;
}

namespace Editor
{
	class IObjectPropertiesViewer;

	// ------------------------------
	// Editor object pointer property
	// ------------------------------
	class ObjectPtrProperty: public IPropertyField
	{
	public:
		// Default constructor
		ObjectPtrProperty();

		// Copy constructor
		ObjectPtrProperty(const ObjectPtrProperty& other);

		// Copy operator
		ObjectPtrProperty& operator=(const ObjectPtrProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Specializes field type. Just storing type, but not creating fields
		void SpecializeType(const Type* type) override;

		// Specializes field info, processing attributes
		void SpecializeFieldInfo(const FieldInfo* fieldInfo) override;

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

		IOBJECT(ObjectPtrProperty);

	protected:
		const ObjectType* mObjectType = nullptr;    // Type of target objects
		const Type*       mObjectPtrType = nullptr; // Type of target object pointer

		bool mDontDeleteEnabled = false; // When it is true, delete button is disabled

		TargetsVec               mTargetObjects;   // Target objects
		IObjectPropertiesViewer* mObjectPropertiesViewer = nullptr; // Object viewer

		HorizontalLayout* mHeaderContainer = nullptr;    // Type caption and create/delete button container widget, placed on spoiler head
		Spoiler*          mSpoiler = nullptr;            // Properties spoiler
		Label*            mTypeCaption = nullptr;        // Caption that shows type of object or nullptr
		Button*           mCreateDeleteButton = nullptr; // Create and delete button. Create - when value is nullptr, delete - when not
		ContextMenu*      mCreateMenu = nullptr;         // Create object context menu. Initializes with types derived from mObjectType 
													     // when this type changing and create button were pressed

		bool mPropertiesInitialized = false; // True when properties were built and initialized. 
		                                     // Properties building when spoiler is expanding or when changing type and spoiler is still expanding

		bool mContextInitialized = false;    // True when context menu initialized with available types of objects. 
		                                     // Context menu initializes when type changed and create button pressed

		bool mImmediateCreateObject = false; // True when no reason to show context menu, because there is only one available type

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when create button pressed and shows create object menu
		void OnCreateOrDeletePressed();

		// Creates object by type
		void CreateObject(const ObjectType* type);

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const override;

		// Converts proxy to IObject property, gets value and returns
		IObject* GetProxy(IAbstractValueProxy* proxy) const;

		// Converts proxy to IObject proxy, then sets value thru proxy
		void SetProxy(IAbstractValueProxy* proxy, IObject* object);
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
	PROTECTED_FIELD(mObjectPtrType);
	PROTECTED_FIELD(mDontDeleteEnabled);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mObjectPropertiesViewer);
	PROTECTED_FIELD(mHeaderContainer);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mTypeCaption);
	PROTECTED_FIELD(mCreateDeleteButton);
	PROTECTED_FIELD(mCreateMenu);
	PROTECTED_FIELD(mPropertiesInitialized);
	PROTECTED_FIELD(mContextInitialized);
	PROTECTED_FIELD(mImmediateCreateObject);
}
END_META;
CLASS_METHODS_META(Editor::ObjectPtrProperty)
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
	PROTECTED_FUNCTION(void, OnCreateOrDeletePressed);
	PROTECTED_FUNCTION(void, CreateObject, const ObjectType*);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
	PROTECTED_FUNCTION(IObject*, GetProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, SetProxy, IAbstractValueProxy*, IObject*);
}
END_META;
