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

	class ObjectPtrProperty : public IPropertyField
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
		Ref<const ObjectType> mBasicObjectType = nullptr;
		Ref<const ObjectType> mCurrentObjectType = nullptr;
		Ref<const ObjectType> mBuiltObjectType = nullptr;

		bool mAvailableMultipleTypes = false;
		bool mDontDeleteEnabled = false;
		bool mNoHeader = false;
		bool mExpanded = false;

		IObjectPropertiesViewer* mObjectViewer = nullptr;

		Ref<Label> mCaption = nullptr;
		Ref<HorizontalLayout> mHeaderContainer = nullptr;
		Ref<Label> mTypeCaption = nullptr;
		Ref<Button> mCreateDeleteButton = nullptr;
		Ref<ContextMenu> mCreateMenu = nullptr;

		bool mContextInitialized = false;
		bool mImmediateCreateObject = false;

	protected:
		// Called when property puts in buffer. Here you can release your shared resources
		void OnFreeProperty() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Updates viewer header caption and header container 
		void UpdateViewerHeader();

		// Called when create button pressed and shows create object menu
		void OnCreateOrDeletePressed();

		// Creates object by type
		void CreateObject(const ObjectType* type);

		// Stores values to data
		void StoreValues(Vector<DataDocument>& data) const override;

		// Converts proxy to IObject property, gets value and returns
		Ref<IObject> GetProxy(Ref<IAbstractValueProxy> proxy) const;

		// Converts proxy to IObject proxy, then sets value via proxy
		void SetProxy(Ref<IAbstractValueProxy> proxy, Ref<IObject> object);
	};
}replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#define DEFAULT_VALUE(expr) (expr)
#define NAME(expr) (expr)

class Editor
{
public:
    using ObjectPtrProperty = Ref<Editor>;

    Editor();
    Editor(const ObjectPtrProperty&);
    void SetValueAndPrototypeProxy(const TargetsVec&);
    void Refresh();
    const Type* GetValueType();
    static const Type* GetValueTypeStatic();
    void SetFieldInfo(const FieldInfo*);
    void SetCaption(const WString&);
    WString GetCaption();
    Button* GetRemoveButton();
    void SetBasicType(const ObjectType*);
    void Expand();
    void Collapse();
    void SetExpanded(bool);
    bool IsExpanded();

protected:
    void OnFreeProperty();
    void InitializeControls();
    void UpdateViewerHeader();
    void OnCreateOrDeletePressed();
    void CreateObject(const ObjectType*);
    void StoreValues(Vector<DataDocument>&);
    IObject* GetProxy(IAbstractValueProxy*);
    void SetProxy(IAbstractValueProxy*, IObject*);

private:
    Ref<const FieldInfo> mCaption;
    Ref<const FieldInfo> mHeaderContainer;
    Ref<const FieldInfo> mTypeCaption;
    Ref<const FieldInfo> mCreateDeleteButton;
    Ref<const FieldInfo> mCreateMenu;
    bool mContextInitialized;
    bool mImmediateCreateObject;
};

// --- END META ---