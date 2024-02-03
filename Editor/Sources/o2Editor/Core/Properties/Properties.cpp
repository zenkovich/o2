#include "o2Editor/stdafx.h"
#include "Properties.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/StringUtils.h"
#include "o2/Utils/System/Time/Timer.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/EnumProperty.h"
#include "o2Editor/Core/Properties/Basic/ObjectProperty.h"
#include "o2Editor/Core/Properties/Basic/ObjectPtrProperty.h"
#include "o2Editor/Core/Properties/Basic/VectorProperty.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"

DECLARE_SINGLETON(Editor::Properties);

namespace Editor
{
    Editor::IPropertyField::OnChangeCompletedFunc Properties::mOnPropertyCompletedChangingUndoCreateDelegate;

    Properties::Properties()
    {
        InitializeAvailablePropertiesFields();
        InitializeAvailableObjectPropertiesViewers();

        mOnPropertyCompletedChangingUndoCreateDelegate =
            MakeFunction<ActionsList, void, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&>(
                &o2EditorApplication, &EditorApplication::DoneActorPropertyChangeAction);
    }

    Properties::~Properties()
    {
        for (auto kv : mPropertiesPool)
        {
            for (auto p : kv.second)
                delete p;
        }

        for (auto kv : mObjectPropertiesViewersPool)
        {
            for (auto p : kv.second)
                delete p;
        }
    }

    void Properties::InitializeAvailablePropertiesFields()
    {
        auto avaialbleTypes = TypeOf(IPropertyField).GetDerivedTypes();

        avaialbleTypes.Remove(&TypeOf(ObjectProperty));
        avaialbleTypes.Remove(&TypeOf(ObjectPtrProperty));
        avaialbleTypes.Remove(&TypeOf(VectorProperty));
        avaialbleTypes.RemoveAll([](const Type* type) { return type->GetName().Contains("TPropertyField"); });

        for (auto x : avaialbleTypes)
        {
            if (auto valueType = x->InvokeStatic<const Type*>("GetValueTypeStatic"))
                mAvailablePropertiesFields[valueType] = x;
        }
    }

    void Properties::InitializeAvailableObjectPropertiesViewers()
    {
        auto availableTypes = TypeOf(IObjectPropertiesViewer).GetDerivedTypes();
        availableTypes.Remove(&TypeOf(IObjectPropertiesViewer));
        availableTypes.Remove(&TypeOf(DefaultObjectPropertiesViewer));

        for (auto x : availableTypes)
        {
            if (auto objectType = x->InvokeStatic<const Type*>("GetViewingObjectTypeStatic"))
                mAvailableObjectPropertiesViewers[objectType] = x;
        }
    }

    const Type* Properties::GetClosesBasedTypeObjectViewer(const Type* type) const
    {
        Vector<const Type*> itTypes = { type };
        while (!itTypes.IsEmpty())
        {
            Vector<const Type*> nextItTypes;

            for (auto t : itTypes)
            {
                const Type* viewerType = nullptr;
                if (mAvailableObjectPropertiesViewers.TryGetValue(t, viewerType))
                    return viewerType;

                nextItTypes.Add(t->GetBaseTypes().Convert<const Type*>([](const Type::BaseType& x) { return x.type; }));
            }

            itTypes = nextItTypes;
        }

        return nullptr;
    }

    IPropertyField* Properties::BuildField(VerticalLayout* layout, const FieldInfo* fieldInfo,
                                           PropertiesContext& context, const String& path,
                                           const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
                                           const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
    {
        Timer timer;

        const Type* fieldType = fieldInfo->GetType();
        String propertyName = GetSmartName(fieldInfo->GetName());

        auto fieldWidget = Ref<IPropertyField>::make(CreateFieldProperty(fieldType, propertyName, onChangeCompleted, onChanged));
        if (!fieldWidget)
            return nullptr;

        fieldWidget->name = fieldInfo->GetName() + " : " + fieldInfo->GetType()->GetName();
        fieldWidget->SetParentContext(const_cast<const PropertiesContext*>(&context));
        fieldWidget->SetValuePath(path + fieldInfo->GetName());
        fieldWidget->SetFieldInfo(fieldInfo);

        if (auto invokeOnChangeAttribute = fieldInfo->GetAttribute<InvokeOnChangeAttribute>())
        {
            fieldWidget->onChanged += [&, invokeOnChangeAttribute](const Ref<IPropertyField>&) {
                for (auto target : context.targets)
                {
                    auto& targetType = target.first->GetType();
                    auto& targetObjType = dynamic_cast<const ObjectType&>(targetType);
                    targetObjType.Invoke<void>(invokeOnChangeAttribute->methodName, targetObjType.DynamicCastFromIObject(target.first));
                }
            };
        }

        layout->AddChild(fieldWidget);

        context.properties.Add(fieldInfo, fieldWidget);

        //o2Debug.Log("Field " + path + "/" + fieldInfo->GetName() + " for " + (String)timer.GetDeltaTime());

        return fieldWidget.get();
    }

    IPropertyField* Properties::BuildField(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
                                           PropertiesContext& context,
                                           const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
                                           const IPropertyField::OnChangedFunc& onChanged)
    {
        const FieldInfo* fieldInfo = objectType.GetField(fieldName);
        if (fieldInfo)
        {
            return BuildField(layout, fieldInfo, context, path, onChangeCompleted, onChanged);
        }
        return nullptr;
    }

    IPropertyField* Properties::CreateFieldProperty(const Type* type, const String& name,
                                                    const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
                                                    const IPropertyField::OnChangedFunc& onChanged)
    {
        auto fieldType = type;
        if (mAvailablePropertiesFields.TryGetValue(type, fieldType))
        {
            return fieldType->InvokeStatic<IPropertyField*>("CreateFieldWidget", fieldType, name, onChangeCompleted, onChanged);
        }
        return nullptr;
    }

    void Properties::GetFieldPropertiesFromObjects(const Type& objectType, const String& fieldName, const Vector<Object*>& objects,
                                                   Vector<FieldPropertyAndValue>& outFieldProperties)
    {
        for (auto obj : objects)
        {
            FieldPropertyAndValue fieldPropertyAndValue;
            fieldPropertyAndValue.propertyName = fieldName;
            fieldPropertyAndValue.object = obj;

            Value value;
            obj->ReadField(fieldName, value);
            fieldPropertyAndValue.value = value;

            outFieldProperties.Add(fieldPropertyAndValue);
        }
    }

    void Properties::GetFieldPropertiesFromObjectProperties(const Type& objectType, const String& fieldName, const Vector<ObjectProperty*>& objectProperties,
                                                            Vector<FieldPropertyAndValue>& outFieldProperties)
    {
        for (auto objProp : objectProperties)
        {
            if (objProp->blobData)
            {
                FieldPropertyAndValue fieldPropertyAndValue;
                fieldPropertyAndValue.propertyName = fieldName;
                fieldPropertyAndValue.objectProperty = objProp;

                Value value;
                objProp->blobData->ReadField(fieldName, value);
                fieldPropertyAndValue.value = value;

                outFieldProperties.Add(fieldPropertyAndValue);
            }
        }
    }

    void Properties::GetFieldPropertiesFromTargets(const Type& objectType, const String& fieldName, const PropertiesContext& context,
                                                   Vector<FieldPropertyAndValue>& outFieldProperties)
    {
        GetFieldPropertiesFromObjects(objectType, fieldName, context.objects, outFieldProperties);
        GetFieldPropertiesFromObjectProperties(objectType, fieldName, context.objectProperties, outFieldProperties);
    }

    Ref<IObjectPropertiesViewer> Properties::CreateObjectPropertiesViewer(const Object* object, const String& name /*= ""*/)
    {
        const Type* objectType = object->GetType();
        const Type* viewerType = GetClosesBasedTypeObjectViewer(objectType);

        if (viewerType)
        {
            auto objectViewer = Ref<IObjectPropertiesViewer>::make(viewerType, object, name);
            objectViewer->SetViewerInstanceName(name);
            return objectViewer;
        }

        return nullptr;
    }

    void Properties::GetObjectPropertiesViewersFor(const Type* type, Vector<Ref<IObjectPropertiesViewer>>& outViewers) const
    {
        outViewers.Clear();

        for (auto kv : mAvailableObjectPropertiesViewers)
        {
            if (kv.first->IsAssignable(type))
            {
                outViewers.Add(Ref<IObjectPropertiesViewer>::make(kv.second));
            }
        }
    }

    void Properties::GetObjectsPropertiesViewers(const Vector<Object*>& objects, Vector<Ref<IObjectPropertiesViewer>>& outViewers) const
    {
        outViewers.Clear();

        for (auto obj : objects)
        {
            auto objType = obj->GetType();
            Vector<Ref<IObjectPropertiesViewer>> objViewers;
            GetObjectPropertiesViewersFor(objType, objViewers);

            for (auto viewer : objViewers)
            {
                if (!outViewers.Contains(viewer))
                {
                    outViewers.Add(viewer);
                }
            }
        }
    }

    void Properties::RemoveViewerFromViewers(Vector<Ref<IObjectPropertiesViewer>>& viewers, IObjectPropertiesViewer* viewer)
    {
        auto it = std::find_if(viewers.begin(), viewers.end(), [viewer](const Ref<IObjectPropertiesViewer>& v) { return v.get() == viewer; });
        if (it != viewers.end())
        {
            viewers.erase(it);
        }
    }

    void Properties::InvalidateProperties()
    {
        for (auto item : mPropertiesPool)
        {
            for (auto prop : item.second)
            {
                prop->Invalidate();
            }
        }
    }

    const Object* Properties::GetCommonObject(const PropertiesContext& context) const
    {
        const Object* commonObject = nullptr;

        for (size_t i = 0; i < context.objects.Size(); ++i)
        {
            if (i == 0)
            {
                commonObject = context.objects[i];
            }
            else if (commonObject != context.objects[i])
            {
                return nullptr;
            }
        }

        return commonObject;
    }

    const ObjectProperty* Properties::GetCommonObjectProperty(const PropertiesContext& context) const
    {
        const ObjectProperty* commonObjectProperty = nullptr;

        for (size_t i = 0; i < context.objectProperties.Size(); ++i)
        {
            if (i == 0)
            {
                commonObjectProperty = context.objectProperties[i];
            }
            else if (commonObjectProperty != context.objectProperties[i])
            {
                return nullptr;
            }
        }

        return commonObjectProperty;
    }

    Properties::Value& Properties::GetPropertyValue(const Object* object, const Property* prop) const
    {
        auto it = mPropertiesPool.find(object);
        if (it != mPropertiesPool.end())
        {
            auto& properties = it->second;
            auto it2 = std::find_if(properties.begin(), properties.end(), [prop](const PropertyAndValue& pv) { return pv.property == prop; });
            if (it2 != properties.end())
            {
                return it2->value;
            }
        }
        return sDefaultValue;
    }

    void Properties::ChangeProperty(const Object* object, const Property* prop, const Value& value)
    {
        auto& propValue = GetPropertyValue(object, prop);

        if (propValue != value)
        {
            propValue = value;

            onChangeProperty.Emit(prop);
        }
    }

    bool Properties::HasDefaultValue(const Object* object, const Property* prop) const
    {
        auto& propValue = GetPropertyValue(object, prop);

        return propValue == sDefaultValue;
    }

    void Properties::SetDefaultValue(const Object* object, const Property* prop)
    {
        auto& propValue = GetPropertyValue(object, prop);

        propValue = sDefaultValue;
    }

    void Properties::ResetValues()
    {
        for (auto& item : mPropertiesPool)
        {
            for (auto& prop : item.second)
            {
                SetDefaultValue(item.first, prop.property);
            }
        }
    }
}Replace raw pointers with Ref<>: 

```cpp
Ref<VerticalLayout> layout;
Vector<const FieldInfo*> fields;
PropertiesContext& context;
String path;
Ref<IPropertyField::OnChangeCompletedFunc> onChangeCompleted;
Ref<IPropertyField::OnChangedFunc> onChanged;

void Properties::BuildFields(const Ref<VerticalLayout>& layout, const Ref<Vector<const FieldInfo*>>& fields,
							 PropertiesContext& context, const String& path,
							 const Ref<IPropertyField::OnChangeCompletedFunc>& onChangeCompleted, 
							 const Ref<IPropertyField::OnChangedFunc>& onChanged)
{
	Timer timer; 

	for (const auto& fieldInfo : *fields)
		BuildField(layout, fieldInfo, context, path, onChangeCompleted, onChanged);

	//o2Debug.Log(">>> Fields created for " + (String)timer.GetDeltaTime());
}

void Properties::BuildFields(const Ref<VerticalLayout>& layout, const Type& objectType, 
							 const Ref<Vector<String>>& fieldsNames, const String& path, 
							 PropertiesContext& context, 
							 const Ref<IPropertyField::OnChangeCompletedFunc>& onChangeCompleted, 
							 const Ref<IPropertyField::OnChangedFunc>& onChanged)
{
	for (const auto& name : *fieldsNames)
		BuildField(layout, objectType, name, path, context, onChangeCompleted, onChanged);
}

void Properties::SetPrivateFieldsVisible(bool visible)
{
	mPrivateVisible = visible;
}

bool Properties::IsPrivateFieldsVisible() const
{
	return mPrivateVisible;
}

bool Properties::IsFieldTypeSupported(const Type* type) const
{
	if (type->GetUsage() == Type::Usage::Vector)
		return IsFieldTypeSupported(DynamicCast<const VectorType*>(type)->GetElementType());

	if (type->IsBasedOn(TypeOf<IObject>()))
		return true;

	if (type->GetUsage() == Type::Usage::Pointer && DynamicCast<const PointerType*>(type)->GetBaseType()->IsBasedOn(TypeOf<IObject>()))
		return true;

	if (type->GetUsage() == Type::Usage::Enumeration)
		return true;

	if (type->GetUsage() == Type::Usage::Property)
		return IsFieldTypeSupported(DynamicCast<const PropertyType*>(type)->GetValueType());

	if (GetFieldPropertyType(type))
		return true;

	return false;
}

void Properties::FreeProperties(PropertiesContext& context)
{
	for (const auto& prop : context.properties)
		FreeProperty(prop.second);

	context.properties.Clear();
}

void Properties::FreeProperty(const Ref<IPropertyField>& field)
{
	if (!mPropertiesPool.ContainsKey(field->GetValueType()))
		mPropertiesPool.Add(field->GetValueType(), Vector<Ref<IPropertyField>>());

	mPropertiesPool[field->GetValueType()].Add(field.Get());
	field->OnFreeProperty();
	field->SetParent(nullptr, false);
}

bool Properties::IsPropertyVisible(const FieldInfo* info, bool allowPrivate) const
{
	if (info->HasAttribute<IgnoreEditorPropertyAttribute>())
		return false;

	if (info->GetProtectionSection() == ProtectSection::Public || allowPrivate)
		return true;

	if (info->HasAttribute<EditorPropertyAttribute>())
		return true;

	return false;
}

bool Properties::IsPropertyVisible(const FieldInfo* info) const
{
	return IsPropertyVisible(info, mPrivateVisible);
}

void Properties::BuildObjectProperties(const Ref<VerticalLayout>& layout, const Type* type,
									   PropertiesContext& context, const String& path,
									   const Ref<IPropertyField::OnChangeCompletedFunc>& onChangeCompleted, 
									   const Ref<IPropertyField::OnChangedFunc>& onChanged)
{
	BuildObjectProperties(layout, type->GetFieldsWithBaseClasses(), context, path, onChangeCompleted, onChanged);
}

void Properties::BuildObjectProperties(const Ref<VerticalLayout>& layout, const Ref<Vector<const FieldInfo*>>& fields,
									   PropertiesContext& context, const String& path,
									   const Ref<IPropertyField::OnChangeCompletedFunc>& onChangeCompleted,
									   const Ref<IPropertyField::OnChangedFunc>& onChanged)
{
	PushEditorScopeOnStack scope;

	Vector<const FieldInfo*> regularFields = fields.FindAll(
		[&](const FieldInfo* x) { return IsPropertyVisible(x, false); });

	BuildFields(layout, Ref<Vector<const FieldInfo*>>(regularFields), context, path, onChangeCompleted, onChanged);

	if (mPrivateVisible)
	{
		Vector<const FieldInfo*> privateFields = fields.FindAll(
			[&](const FieldInfo* x) { return IsPropertyVisible(x, true) && !regularFields.Contains(x); });

		if (!privateFields.IsEmpty())
		{
			Ref<Spoiler> privates = context.privatePropertiesSpoiler;

			if (!privates)
				privates = layout->GetChildByType<Spoiler>("privates");

			if (!privates)
			{
				privates = o2UI.CreateWidget<Spoiler>("expand with caption");
				privates->name = "privates";
				privates->SetCaption("Private");
				layout->AddChild(privates.Get());
			}
			else privates->SetIndexInSiblings(layout->GetChildren().Count() - 1);

			context.privatePropertiesSpoiler.#include <memory>
#include <type_traits>

template<typename T>
class Ref
{
public:
	Ref() = default;
	Ref(T* ptr) : m_ptr(ptr) {}
	Ref(const Ref& other) : m_ptr(other.m_ptr) {}
	Ref(Ref&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr)) {}
	~Ref() { delete m_ptr; }

	Ref& operator=(const Ref& other)
	{
		if (this != &other)
			m_ptr = other.m_ptr;

		return *this;
	}

	Ref& operator=(Ref&& other) noexcept
	{
		if (this != &other)
		{
			delete m_ptr;
			m_ptr = std::exchange(other.m_ptr, nullptr);
		}

		return *this;
	}

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }

	operator bool() const { return m_ptr != nullptr; }

private:
	T* m_ptr = nullptr;
};

template<typename T>
class WeakRef
{
public:
	WeakRef() = default;
	WeakRef(const WeakRef& other) = default;
	WeakRef(WeakRef&& other) = default;
	~WeakRef() = default;

	WeakRef& operator=(const WeakRef& other) = default;
	WeakRef& operator=(WeakRef&& other) = default;

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }

	operator bool() const { return m_ptr != nullptr; }

private:
	T* m_ptr = nullptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
	return dynamic_cast<Derived*>(ptr.get());
}

class IPropertyField
{
public:
	using OnChangeCompletedFunc = void(*)(void*);
	using OnChangedFunc = void(*)(void*);
};

class IObject {};

class Type
{
public:
	enum class Usage { Vector, Pointer, Reference, Enumeration, Property };

	virtual Usage GetUsage() const = 0;
	virtual bool IsBasedOn(const Type* type) const = 0;
	virtual const Type* GetBaseType() const = 0;
	virtual ~Type() = default;
};

class VectorType : public Type
{
public:
	const Type* GetElementType() const;
};

class PointerType : public Type
{
public:
	const Type* GetBaseType() const;
};

class ObjectType : public Type {};

class ReferenceType : public Type
{
public:
	const Type* GetBaseType() const;
};

class PropertyType : public Type
{
public:
	const Type* GetValueType() const;
};

class EnumProperty : public IPropertyField
{
public:
	void SetCaption(const String& name);
	void SpecializeType(const Type* type);
};

class ObjectProperty : public IPropertyField
{
public:
	void SetCaption(const String& name);
};

class Properties
{
public:
	void BuildFields()
	{
		if (mPrivateVisible)
		{
			if (mPrivateVisible && !context.builtWithPrivateProperties)
			{
				for (auto& privateField : privateFields)
				{
					if (const auto& pointerType = privateField->GetBaseType()->dynamic_cast<PointerType*>())
					{
						const auto& baseType = pointerType->GetBaseType();
						if (baseType->IsBasedOn(TypeOf(IObject)))
						{
							privateField = CreateObjectPtrField(static_cast<const ObjectType*>(baseType), privateField, onChangeCompleted, onChanged);
						}
					}
					else if (const auto& referenceType = privateField->GetBaseType()->dynamic_cast<ReferenceType*>())
					{
                        const auto& baseType = referenceType->GetBaseType();
						if (baseType->IsBasedOn(TypeOf(IObject)))
						{
							privateField = CreateObjectPtrField(static_cast<const ObjectType*>(baseType), privateField, onChangeCompleted, onChanged);
						}
					}
				}
			}
		}
		else if (context.privatePropertiesSpoiler)
		{
			context.privatePropertiesSpoiler->SetEnabled(false);
		}

		context.builtWithPrivateProperties = mPrivateVisible;
	}

	IPropertyField* CreateFieldProperty(const Type* type, const String& name,
										const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
										const IPropertyField::OnChangedFunc& onChanged)
	{
		PushEditorScopeOnStack enterScope;

		if (GetFieldPropertyType(type))
			return CreateRegularField(type, name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Vector)
		{
			auto elementType = type->GetElementType();
			if (IsFieldTypeSupported(elementType))
				return CreateVectorField(type, name, onChangeCompleted, onChanged);

			return nullptr;
		}

		if (type->IsBasedOn(TypeOf(IObject)))
			return CreateObjectField(name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Pointer)
		{
            auto pointerType = dynamic_cast<const PointerType*>(type);
            auto baseType = pointerType->GetBaseType();
			if (baseType->IsBasedOn(TypeOf(IObject)))
			{
				return CreateObjectPtrField(static_cast<const ObjectType*>(baseType), name, onChangeCompleted, onChanged);
			}

			return nullptr;
		}

		if (type->GetUsage() == Type::Usage::Reference)
        {
            auto referenceType = dynamic_cast<const ReferenceType*>(type);
			auto baseType = referenceType->GetBaseType();
            if (baseType->IsBasedOn(TypeOf(IObject)))
            {
                return CreateObjectPtrField(static_cast<const ObjectType*>(baseType), name, onChangeCompleted, onChanged);
            }

            return nullptr;
		}

		if (type->GetUsage() == Type::Usage::Enumeration)
			return CreateEnumField(type, name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = dynamic_cast<const PropertyType*>(type)->GetValueType();
			return CreateFieldProperty(valueType, name, onChangeCompleted, onChanged);
		}

		return nullptr;
	}

	IPropertyField* CreateRegularField(const Type* type, const String& name,
									   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
									   const IPropertyField::OnChangedFunc& onChanged)
	{
		const Type* fieldPropertyType = GetFieldPropertyType(type);
		if (!fieldPropertyType)
			return nullptr;

		IPropertyField* fieldProperty = nullptr;

		if (mPropertiesPool[type].size() > 0)
			fieldProperty = mPropertiesPool[type].back();
		else
			fieldProperty = new typename std::remove_pointer<decltype(fieldProperty)>::type; // create widget

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* CreateEnumField(const Type* type, const String& name,
									const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
									const IPropertyField::OnChangedFunc& onChanged)
	{
		EnumProperty* fieldProperty = nullptr;

		if (mPropertiesPool[type].size() > 0)
			fieldProperty = mPropertiesPool[type].back();
		else
			fieldProperty = new typename std::remove_pointer<decltype(fieldProperty)>::type; // create widget

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);
		fieldProperty->SpecializeType(type);

		return fieldProperty;
	}

	IPropertyField* CreateObjectField(const String& name,
									 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
									 const IPropertyField::OnChangedFunc& onChanged)
	{
		ObjectProperty* fieldProperty = nullptr;

		const Type* objectType = &TypeOf(IObject);
		if (mPropertiesPool[objectType].size() > 0)
			fieldProperty = mPropertiesPool[objectType].back();
		else
			fieldProperty = new typename std::remove_pointer<decltype(fieldProperty)>::type;

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* CreateObjectPtrField(const ObjectType* basicType, const String& name,
										 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
										 const IPropertyField::OnChangedFunc& onChanged)
	{
		IPropertyField* fieldProperty = nullptr;

		const Type* objectType = &TypeOf(IObject);
		if (mPropertiesPool[objectType].size() > 0)
			fieldProperty = mPropertiesPool[objectType].back();
		else
			fieldProperty = new typename std::remove_pointer<decltype(fieldProperty)>::type;

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

private:
	// Use Ref instead of raw pointers, except void* pointers
	std::unordered_map<const Type*, std::vector<Ref<IPropertyField>>> mPropertiesPool;

	bool mPrivateVisible;
	Context context;
	std::vector<IPropertyField*> privateFields;
	IPropertyField::OnChangeCompletedFunc onChangeCompleted;
	IPropertyField::OnChangedFunc onChanged;
};

int main()
{
	return 0;
}#include <memory>

template<typename T>
class Ref
{
public:
	Ref() : ptr(nullptr) {}
	Ref(T* p) : ptr(p) {}
	Ref(const Ref<T>& other) : ptr(other.ptr) {}
	~Ref() { delete ptr; }

	Ref<T>& operator=(const Ref<T>& other)
	{
		if (this != &other)
		{
			delete ptr;
			ptr = other.ptr;
		}
		return *this;
	}

	T& operator*() const { return *ptr; }
	T* operator->() const { return ptr; }
	operator bool() const { return ptr != nullptr; }

private:
	T* ptr;
};

template<typename T>
class WeakRef
{
public:
	WeakRef() : ptr(nullptr) {}
	WeakRef(const Ref<T>& ref) : ptr(ref.ptr) {}
	WeakRef(const WeakRef<T>& other) : ptr(other.ptr) {}

	WeakRef<T>& operator=(const WeakRef<T>& other)
	{
		if (this != &other)
		{
			ptr = other.ptr;
		}
	}

	Ref<T> Lock() const { return Ref<T>(ptr); }

private:
	T* ptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
class DynamicCast
{
public:
	template<typename U>
	static Ref<U> Cast(const Ref<T>& ref)
	{
		return Ref<U>(dynamic_cast<U*>(ref.operator->()));
	}
};

class IPropertyField
{
public:
	using OnChangedFunc = std::function<void()>;
	using OnChangeCompletedFunc = std::function<void()>;

	virtual ~IPropertyField() {}

	virtual void SetCaption(const std::string& caption) = 0;
	virtual void SetBasicType(const std::string& basicType) = 0;
};

class ObjectPtrProperty : public IPropertyField
{
public:
	void SetCaption(const std::string& caption) override {}

	void SetBasicType(const std::string& basicType) override {}

	using OnChangedFunc = IPropertyField::OnChangedFunc;
	using OnChangeCompletedFunc = IPropertyField::OnChangeCompletedFunc;

	OnChangedFunc onChanged;
	OnChangeCompletedFunc onChangeCompleted;
};

class VectorProperty : public IPropertyField
{
public:
	void SetCaption(const std::string& caption) override {}

	void SetBasicType(const std::string& basicType) override {}

	using OnChangedFunc = IPropertyField::OnChangedFunc;
	using OnChangeCompletedFunc = IPropertyField::OnChangeCompletedFunc;

	OnChangedFunc onChanged;
	OnChangeCompletedFunc onChangeCompleted;
};

class IObjectPropertiesViewer
{
public:
	virtual ~IObjectPropertiesViewer() {}

	std::string path;

	using OnChangedFunc = IPropertyField::OnChangedFunc;
	using OnChangeCompletedFunc = IPropertyField::OnChangeCompletedFunc;

	OnChangedFunc onChanged;
	OnChangeCompletedFunc onChangeCompleted;

	virtual void OnFree() = 0;
	virtual IObjectPropertiesViewer* GetSpoiler() = 0;
	virtual const Type* GetViewingObjectType() const = 0;
};

class DefaultObjectPropertiesViewer : public IObjectPropertiesViewer
{
public:
	void OnFree() override {}

	IObjectPropertiesViewer* GetSpoiler() override { return nullptr; }

	const Type* GetViewingObjectType() const override { return nullptr; }
};

class Type
{
public:
	enum class Usage
	{
		Pointer,
		Other
	};

	virtual ~Type() {}

	virtual Usage GetUsage() const = 0;
	virtual bool IsBasedOn(const Type& other) const = 0;
};

class PointerType : public Type
{
public:
	const Type* GetBaseType() const { return nullptr; }
};

class String
{
public:
	String() {}
	String(const char* str) {}
};

class Properties
{
public:
	template<typename T>
	using RefVector = std::vector<Ref<T>>;

	Ref<ObjectPtrProperty> CreateField(const String& name, const String& basicType,
	                                  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
	                                  const IPropertyField::OnChangedFunc& onChanged);

	IPropertyField* CreateVectorField(const Type* type, const String& name,
	                                  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
	                                  const IPropertyField::OnChangedFunc& onChanged);

	IObjectPropertiesViewer* CreateObjectViewer(const Type* type, const String& path,
	                                            const IPropertyField::OnChangeCompletedFunc& onChangeCompleted,
	                                            const IPropertyField::OnChangedFunc& onChanged);

	void FreeObjectViewer(IObjectPropertiesViewer* viewer);

	const Type* GetFieldPropertyType(const Type* valueType) const;

private:
	template<typename T>
	bool ContainsObjectPtrProperty() const;

	template<typename T>
	void AddObjectPtrProperty();

	template<typename T>
	void RemoveObjectPtrProperty();

	std::vector<IPropertyField*> mPropertiesPool;
	std::map<const Type*, RefVector<IPropertyField>> mAvailablePropertiesFields;
	std::map<const Type*, RefVector<IObjectPropertiesViewer>> mObjectPropertiesViewersPool;
};

template<typename T>
bool Properties::ContainsObjectPtrProperty() const
{
	return !mAvailablePropertiesFields[typeid(IObject*)].empty();
}

template<typename T>
void Properties::AddObjectPtrProperty()
{
	mPropertiesPool.push_back(mmake<ObjectPtrProperty>());
}

template<typename T>
void Properties::RemoveObjectPtrProperty()
{
	auto it = std::find_if(mPropertiesPool.begin(), mPropertiesPool.end(), [](IPropertyField* field)
	{
		return dynamic_cast<ObjectPtrProperty*>(field) != nullptr;
	});

	if (it != mPropertiesPool.end())
	{
		delete *it;
		mPropertiesPool.erase(it);
	}
}

Ref<ObjectPtrProperty> Properties::CreateField(const String& name, const String& basicType,
                                               const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
                                               const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
{
	Ref<ObjectPtrProperty> fieldProperty;

	const Type* objectType = &typeid(IObject*);
	if (mAvailablePropertiesFields.count(objectType) > 0 && mAvailablePropertiesFields[objectType].size() > 0)
	{
		fieldProperty = DynamicCast<IPropertyField>::Cast<ObjectPtrProperty>(mAvailablePropertiesFields[objectType].back());
		mAvailablePropertiesFields[objectType].pop_back();
	}
	else
	{
		fieldProperty = mmake<ObjectPtrProperty>();
	}

	fieldProperty->onChanged = onChanged;
	fieldProperty->onChangeCompleted = onChangeCompleted;
	fieldProperty->SetCaption(name);
	fieldProperty->SetBasicType(basicType);

	return fieldProperty;
}

IPropertyField* Properties::CreateVectorField(const Type* type, const String& name,
                                              const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
                                              const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
{
	IPropertyField* fieldProperty = nullptr;

	if (mAvailablePropertiesFields.count(type) > 0 && mAvailablePropertiesFields[type].size() > 0)
	{
		fieldProperty = DynamicCast<IPropertyField>::Cast<VectorProperty>(mAvailablePropertiesFields[type].back());
		mAvailablePropertiesFields[type].pop_back();
	}
	else
	{
		fieldProperty = mmake<VectorProperty>();
	}

	fieldProperty->onChanged = onChanged;
	fieldProperty->onChangeCompleted = onChangeCompleted;
	fieldProperty->SetCaption(name);

	return fieldProperty;
}

IObjectPropertiesViewer* Properties::CreateObjectViewer(const Type* type, const String& path,
                                                        const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
                                                        const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
{
	const Type* viewerType = GetClosesBasedTypeObjectViewer(type);
	if (!viewerType)
		viewerType = &typeid(DefaultObjectPropertiesViewer);

	IObjectPropertiesViewer* viewer = nullptr;

	if (mObjectPropertiesViewersPool.count(type) > 0 && !mObjectPropertiesViewersPool[type].empty())
	{
		viewer = mObjectPropertiesViewersPool[type].back();
		mObjectPropertiesViewersPool[type].pop_back();
	}

	if (!viewer)
		viewer = dynamic_cast<IObjectPropertiesViewer*>(viewerType->CreateSample());

	viewer->path = path;
	viewer->onChanged = onChanged;
	viewer->onChangeCompleted = onChangeCompleted;

	return viewer;
}

void Properties::FreeObjectViewer(IObjectPropertiesViewer* viewer)
{
	const Type* type = viewer->GetViewingObjectType();

	if (mObjectPropertiesViewersPool.count(type) == 0)
		mObjectPropertiesViewersPool[type] = std::vector<IObjectPropertiesViewer*>();

	mObjectPropertiesViewersPool[type].push_back(viewer);
	viewer->OnFree();
	viewer->GetSpoiler()->SetParent(nullptr);
}

const Type* Properties::GetFieldPropertyType(const Type* valueType) const
{
	for (auto& kv : mAvailablePropertiesFields)
	{
		const Type* keyType = kv.first;
		if (valueType->GetUsage() == Type::Usage::Pointer && keyType->GetUsage() == Type::Usage::Pointer)
		{
			if (DynamicCast<Type>::Cast<PointerType>(valueType)->GetBaseType()->IsBasedOn(*DynamicCast<Type>::Cast<PointerType>(keyType)->GetBaseType()))
				return kv.second;
		}
		else if (valueType->IsBasedOn(*keyType))
		{
			return kv.second;
		}
	}

	return nullptr;
}