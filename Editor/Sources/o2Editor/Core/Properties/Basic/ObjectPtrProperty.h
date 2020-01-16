#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

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
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		// Specializes field info, processing attributes
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Sets basic object type, used in create
		void SetBasicType(const ObjectType* type);

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
		const ObjectType* mBasicObjectType = nullptr;   // Base object type, used for create
		const ObjectType* mCurrentObjectType = nullptr; // Type of target objects

		bool mDontDeleteEnabled = false; // When it is true, delete button is disabled

		TargetsVec               mTargetObjects;          // Target objects
		IObjectPropertiesViewer* mObjectViewer = nullptr; // Object viewer

		HorizontalLayout* mHeaderContainer = nullptr;    // Type caption and create/delete button container widget, placed on spoiler head
		Spoiler*          mSpoiler = nullptr;            // Properties spoiler
		Label*            mTypeCaption = nullptr;        // Caption that shows type of object or nullptr
		Button*           mCreateDeleteButton = nullptr; // Create and delete button. Create - when value is nullptr, delete - when not
		ContextMenu*      mCreateMenu = nullptr;         // Create object context menu. Initializes with types derived from mObjectType 
													     // when this type changing and create button were pressed

		bool mContextInitialized = false;    // True when context menu initialized with available types of objects. 
		                                     // Context menu initializes when type changed and create button pressed

		bool mImmediateCreateObject = false; // True when no reason to show context menu, because there is only one available type

	protected:
		// It is called when property puts in buffer. Here you can release your shared resources
		void OnFreeProperty() override;

		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Checks viewer type, creates new if needed
		void CheckViewer();

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
	PROTECTED_FIELD(mBasicObjectType);
	PROTECTED_FIELD(mCurrentObjectType);
	PROTECTED_FIELD(mDontDeleteEnabled);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mObjectViewer);
	PROTECTED_FIELD(mHeaderContainer);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mTypeCaption);
	PROTECTED_FIELD(mCreateDeleteButton);
	PROTECTED_FIELD(mCreateMenu);
	PROTECTED_FIELD(mContextInitialized);
	PROTECTED_FIELD(mImmediateCreateObject);
}
END_META;
CLASS_METHODS_META(Editor::ObjectPtrProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PUBLIC_FUNCTION(void, SetFieldInfo, const FieldInfo*);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(Button*, GetRemoveButton);
	PUBLIC_FUNCTION(void, SetBasicType, const ObjectType*);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PROTECTED_FUNCTION(void, OnFreeProperty);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, CheckViewer);
	PROTECTED_FUNCTION(void, OnCreateOrDeletePressed);
	PROTECTED_FUNCTION(void, CreateObject, const ObjectType*);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
	PROTECTED_FUNCTION(IObject*, GetProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, SetProxy, IAbstractValueProxy*, IObject*);
}
END_META;
