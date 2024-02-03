#pragma once

#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/String.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2/Utils/Types/Ref.h"
#include "o2/Utils/Types/WeakRef.h"

using namespace o2;

namespace o2
{
    class VerticalLayout;
    class Widget;
}

// Editor properties window accessor macros
#define o2EditorProperties Editor::Properties::Instance()

namespace Editor
{
    class IObjectPropertiesViewer;

    // ------------------------------------------------------------------------------
    // Editor properties building utility. Can cache, build and reuse property fields
    // ------------------------------------------------------------------------------
    class Properties: public Singleton<Properties>
    {
    public:
        Function<void(const Ref<IPropertyField>&)> onFieldChanged; // Some field changed event

    public:
        // Default constructor. Initializes properties samples and fills pools
        Properties();

        // Destructor
        ~Properties();

        // Sets private field viewing state
        void SetPrivateFieldsVisible(bool visible);

        // Returns is private fields visible
        bool IsPrivateFieldsVisible() const;

        // Free properties and put in cache
        void FreeProperties(PropertiesContext& context);

        // Free property field and put in cache
        void FreeProperty(const Ref<IPropertyField>& field);

        // Builds layout viewer by type for objects
        void BuildObjectProperties(VerticalLayout* layout, const Type& type, PropertiesContext& context, const String& path,
                                   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Builds layout viewer by fields
        void BuildObjectProperties(VerticalLayout* layout, const Vector<const FieldInfo*>& fields, PropertiesContext& context, const String& path,
                                   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Build layout viewer for field
        Ref<IPropertyField> BuildField(VerticalLayout* layout, const FieldInfo* fieldInfo, PropertiesContext& context, const String& path,
                                       const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                       const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Build layout viewer for field
        Ref<IPropertyField> BuildField(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
                                       PropertiesContext& context,
                                       const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                       const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Build layout viewer for fields
        void BuildFields(VerticalLayout* layout, const Type& objectType, const Vector<String>& fieldsNames, const String& path,
                          PropertiesContext& context,
                          const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                          const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Build layout viewer for field
        template<typename PropertyFieldType>
        Ref<PropertyFieldType> BuildFieldType(VerticalLayout* layout, const FieldInfo* fieldInfo, PropertiesContext& context, const String& path,
                                              const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                              const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Build layout viewer for field
        template<typename PropertyFieldType>
        Ref<PropertyFieldType> BuildFieldType(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
                                              PropertiesContext& context,
                                              const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                              const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

        // Checks that property with type can be created
        bool IsFieldTypeSupported(const Type& type) const;

        // Checks is property visible in properties window: 
        // checking protection section, ignore and editor attributes
        bool IsPropertyVisible(const FieldInfo* info, bool allowPrivate) const;

        // Checks is property visible in properties window using current private visibility state: 
        // checking protection section, ignore and editor attributes
        bool IsPropertyVisible(const FieldInfo* info) const;

        // Returns available field by type
        const Type& GetFieldPropertyType(const Type& valueType) const;

        // Creates field property by type 
        Ref<IPropertyField> CreateFieldProperty(const Type& type, conReplace raw pointers with `Ref<>` for all class members and function parameters. Use `const Ref<>&` for function arguments with pointer types and overridden methods. Do not replace `void*` pointers. Remove the default `nullptr` value for class members with type `Ref<>`. If a class variable is some kind of "parent", use `WeakRef<>`. Use `mmake<>()` to create `Ref<>` objects, similar to `make_shared<>()`. Replace `dynamic_cast<type*>` with `DynamicCast<type>` to cast `Ref<>` objects.

Here is the updated code:

```cpp
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

template<typename T, typename U>
inline Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class IPropertyField
{
public:
    typedef std::function<void()> OnChangeCompletedFunc;
    typedef std::function<void()> OnChangedFunc;

    // ...

protected:
    static OnChangeCompletedFunc mOnPropertyCompletedChangingUndoCreateDelegate;

    // ...
};

class Properties
{
public:
    // ...

    IPropertyField* CreateRegularField(const Type* type, const String& name,
                                       const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                       const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    IPropertyField* CreateEnumField(const Type* type, const String& name,
                                    const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                    const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    IPropertyField* CreateObjectField(const String& name,
                                      const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                      const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    IPropertyField* CreateObjectPtrField(const ObjectType* basicType, const String& name,
                                         const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                         const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    IPropertyField* CreateVectorField(const Type* type, const String& name,
                                      const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                      const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    IObjectPropertiesViewer* CreateObjectViewer(const Type* type, const String& path,
                                                const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                                const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    void FreeObjectViewer(IObjectPropertiesViewer* viewer);

protected:
    typedef std::map<const Type*, const Type*> PropertiesFieldsMap;
    typedef std::map<const Type*, std::vector<IPropertyField*>> TypePropertyMap;

    typedef std::map<const Type*, const Type*> IObjectPropertiesViewersMap;
    typedef std::map<const Type*, std::vector<IObjectPropertiesViewer*>> TypeObjectPropertiesViewerMap;

    int  mPropertyFieldsPoolStep = 5; // Field properties pools resize step
    bool mPrivateVisible = false;     // Is private fields visible

    PropertiesFieldsMap         mAvailablePropertiesFields;        // Available properties fields samples
    IObjectPropertiesViewersMap mAvailableObjectPropertiesViewers; // Available object properties viewers samples

    TypePropertyMap               mPropertiesPool;              // Pool of properties, grouped by property type
    TypeObjectPropertiesViewerMap mObjectPropertiesViewersPool; // Pool of object properties viewers, grouped by object type

protected:
    // Initializes available properties fields samples
    void InitializeAvailablePropertiesFields();

    // Initializes available object properties viewers samples
    void InitializeAvailableObjectPropertiesViewers();

    // Returns type of object viewer, which is closest ty type by based types hierarchy depth
    const Type* GetClosesBasedTypeObjectViewer(const Type* type) const;

    // Builds layout viewer by fields without filtering
    void BuildFields(VerticalLayout* layout, std::vector<const FieldInfo*> fields, PropertiesContext& context, const String& path,
                     const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                     const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

    template<typename PropertyFieldType>
    PropertyFieldType* BuildFieldType(VerticalLayout* layout, const FieldInfo* fieldInfo,
                                       PropertiesContext& context, const String& path,
                                       const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                       const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty)
    {
        return DynamicCast<PropertyFieldType>(BuildField(layout, fieldInfo, context, path, onChangeCompleted, onChanged));
    }

    template<typename PropertyFieldType>
    PropertyFieldType* BuildFieldType(VerticalLayout* layout, const Type& objectType, const String& fieldName,
                                       const String& path, PropertiesContext& context,
                                       const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
                                       const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty)
    {
        return DynamicCast<PropertyFieldType>(BuildField(layout, objectType, fieldName, path, context, onChangeCompleted, onChanged));
    }
};

```Replace raw pointers with Ref<>:

```cpp
#include <Ref.hpp>

Ref<PropertyFieldType> PropertyFieldFactory::BuildField(
    const AXUI::Node::WeakRef<AXUI::Node>& layout,
    const Ref<AXUI::Object>& objectType,
    const std::string& fieldName,
    const std::string& path,
    const std::string& context,
    CompletedFunc& onChangeCompleted,
    const IPropertyField::OnChangedFunc& onChanged)
{
    return DynamicCast<PropertyFieldType>(BuildField(layout, objectType, fieldName, path, context, onChangeCompleted, onChanged));
}
```

Remove default nullptr value for class members with type Ref<>:

```cpp
Ref<CompletedFunc> onChangeCompleted; // instead of "nullptr"
```

If class variable is some kind of "parent", use WeakRef<>:

```cpp
#include <WeakRef.hpp>

WeakRef<ParentType> mParent; // instead of "ParentType*"
```

Use `mmake<>()` to create Ref<>, just like make_shared<>():

```cpp
Ref<ObjectType> obj = mmake<ObjectType>(); // instead of "ObjectType* obj = new ObjectType();"
```

Replace `dynamic_cast<type*>` with `DynamicCast<type>` to cast Ref<>:

```cpp
DynamicCast<PropertyFieldType>(BuildField(layout, objectType, fieldName, path, context, onChangeCompleted, onChanged));
```

Updated code:

```cpp
#include <Ref.hpp>
#include <WeakRef.hpp>

Ref<PropertyFieldType> PropertyFieldFactory::BuildField(
    const AXUI::Node::WeakRef<AXUI::Node>& layout,
    const Ref<AXUI::Object>& objectType,
    const std::string& fieldName,
    const std::string& path,
    const std::string& context,
    const Ref<CompletedFunc>& onChangeCompleted,
    const IPropertyField::OnChangedFunc& onChanged)
{
    return DynamicCast<PropertyFieldType>(BuildField(layout, objectType, fieldName, path, context, onChangeCompleted, onChanged));
}

Ref<CompletedFunc> mParent;

Ref<ObjectType> obj = mmake<ObjectType>();
```