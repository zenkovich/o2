#include "o2Editor/stdafx.h"
#include "Properties.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/System/Time/Timer.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/EditorScope.h"
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

		mOnPropertyCompletedChangingUndoCreateDelegate = MakeFunction<EditorApplication, void, const String&, const Vector<DataNode>&, const Vector<DataNode>&>(
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

				nextItTypes.Add(t->GetBaseTypes().Select<const Type*>([](const Type::BaseType& x) { return x.type; }));
			}

			itTypes = nextItTypes;
		}

		return nullptr;
	}

	IPropertyField* Properties::BuildField(VerticalLayout* layout, FieldInfo* fieldInfo,
										   PropertiesContext& context, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		Timer timer;

		const Type* fieldType = fieldInfo->GetType();
		String propertyName = MakeSmartFieldName(fieldInfo->GetName());

		auto fieldWidget = CreateFieldProperty(fieldType, propertyName, onChangeCompleted, onChanged);
		if (!fieldWidget)
			return nullptr;

		fieldWidget->name = fieldInfo->GetName() + " : " + fieldInfo->GetType()->GetName();
		fieldWidget->SetParentContext(&context);
		fieldWidget->SetValuePath(path + fieldInfo->GetName());
		fieldWidget->SetFieldInfo(fieldInfo);

		if (auto invokeOnChangeAttribute = fieldInfo->GetAttribute<InvokeOnChangeAttribute>())
		{
			fieldWidget->onChanged += [&, invokeOnChangeAttribute](IPropertyField*) {
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

		return fieldWidget;
	}

	IPropertyField* Properties::BuildField(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
										   PropertiesContext& context, 
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/, 
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		return BuildField(layout, objectType.GetField(fieldName), context, path, onChangeCompleted, onChanged);
	}

	void Properties::BuildFields(VerticalLayout* layout, Vector<FieldInfo*> fields,
								 PropertiesContext& context, const String& path,
								 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
								 const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		Timer timer; 

		for (auto fieldInfo : fields)
			BuildField(layout, fieldInfo, context, path, onChangeCompleted, onChanged);

		//o2Debug.Log(">>> Fields created for " + (String)timer.GetDeltaTime());
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
			return IsFieldTypeSupported(dynamic_cast<const VectorType*>(type)->GetElementType());

		if (type->IsBasedOn(TypeOf(IObject)))
			return true;

		if (type->GetUsage() == Type::Usage::Pointer && ((PointerType*)type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			return true;

		if (type->GetUsage() == Type::Usage::Enumeration)
			return true;

		if (type->GetUsage() == Type::Usage::Property)
			return IsFieldTypeSupported(dynamic_cast<const PropertyType*>(type)->GetValueType());

		if (GetFieldPropertyType(type))
			return true;

		return false;
	}

	void Properties::FreeProperties(PropertiesContext& context)
	{
		for (auto prop : context.properties)
			FreeProperty(prop.second);

		context.properties.Clear();
	}

	void Properties::FreeProperty(IPropertyField* field)
	{
		if (!mPropertiesPool.ContainsKey(field->GetValueType()))
			mPropertiesPool.Add(field->GetValueType(), Vector<IPropertyField*>());

		mPropertiesPool[field->GetValueType()].Add(field);
		field->OnFreeProperty();
		field->SetParent(nullptr, false);
	}

	bool Properties::IsPropertyVisible(FieldInfo* info, bool allowPrivate) const
	{
		if (info->HasAttribute<IgnoreEditorPropertyAttribute>())
			return false;

		if (info->GetProtectionSection() == ProtectSection::Public || allowPrivate)
			return true;

		if (info->HasAttribute<EditorPropertyAttribute>())
			return true;

		return false;
	}

	bool Properties::IsPropertyVisible(FieldInfo* info) const
	{
		return IsPropertyVisible(info, mPrivateVisible);
	}

	void Properties::BuildObjectProperties(VerticalLayout* layout, const Type* type,
										   PropertiesContext& context, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		BuildObjectProperties(layout, type->GetFieldsWithBaseClasses(), context, path, onChangeCompleted, onChanged);
	}

	void Properties::BuildObjectProperties(VerticalLayout* layout, Vector<FieldInfo*> fields,
										   PropertiesContext& context, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		PushEditorScopeOnStack scope;

		Vector<FieldInfo*> regularFields = fields.FindAll(
			[&](FieldInfo* x) { return IsPropertyVisible(x, false); });

		BuildFields(layout, regularFields, context, path, onChangeCompleted, onChanged);

		if (mPrivateVisible)
		{
			Vector<FieldInfo*> privateFields = fields.FindAll(
				[&](FieldInfo* x) { return IsPropertyVisible(x, true) && !regularFields.Contains(x); });

			if (!privateFields.IsEmpty())
			{
				Spoiler* privates = context.privatePropertiesSpoiler;

				if (!privates)
					privates = layout->GetChildByType<Spoiler>("privates");

				if (!privates)
				{
					privates = o2UI.CreateWidget<Spoiler>("expand with caption");
					privates->name = "privates";
					privates->SetCaption("Private");
					layout->AddChild(privates);
				}
				else privates->SetIndexInSiblings(layout->GetChildren().Count() - 1);

				context.privatePropertiesSpoiler = privates;
				BuildFields(privates, privateFields, context, path, onChangeCompleted, onChanged);
			}
		}
		else if (context.privatePropertiesSpoiler)
		{
			context.privatePropertiesSpoiler->SetEnabled(false);
		}

		context.builtWithPrivateProperties = mPrivateVisible;
	}

	IPropertyField* Properties::CreateFieldProperty(const Type* type, const String& name, 
													const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
													const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		PushEditorScopeOnStack enterScope;

		if (GetFieldPropertyType(type))
			return CreateRegularField(type, name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Vector)
		{
			auto elementType = dynamic_cast<const VectorType*>(type)->GetElementType();
			if (IsFieldTypeSupported(elementType))
				return CreateVectorField(type, name, onChangeCompleted, onChanged);

			return nullptr;
		}

		if (type->IsBasedOn(TypeOf(IObject)))
			return CreateObjectField(name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Pointer)
		{
			auto pointerType = dynamic_cast<const PointerType*>(type);
			if (pointerType->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			{
				return CreateObjectPtrField(dynamic_cast<const ObjectType*>(pointerType->GetUnpointedType()), name,
											onChangeCompleted, onChanged);
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

	IPropertyField* Properties::CreateRegularField(const Type* type, const String& name,
												   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
												   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		const Type* fieldPropertyType = GetFieldPropertyType(type);
		if (!fieldPropertyType)
			return nullptr;

		IPropertyField* fieldProperty = nullptr;

		if (mPropertiesPool.ContainsKey(type) && mPropertiesPool[type].Count() > 0)
			fieldProperty = mPropertiesPool[type].PopBack();
		else
			fieldProperty = dynamic_cast<IPropertyField*>(o2UI.CreateWidget(*fieldPropertyType, "with caption"));

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateEnumField(const Type* type, const String& name, 
												const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/, 
												const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		EnumProperty* fieldProperty = nullptr;

		if (mPropertiesPool.ContainsKey(type) && mPropertiesPool[type].Count() > 0)
			fieldProperty = dynamic_cast<EnumProperty*>(mPropertiesPool[type].PopBack());
		else
			fieldProperty = o2UI.CreateWidget<EnumProperty>("with caption");

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);
		fieldProperty->SpecializeType(type);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateObjectField(const String& name, 
												  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
												  const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		ObjectProperty* fieldProperty = nullptr;

		const Type* objectType = &TypeOf(IObject);
		if (mPropertiesPool.ContainsKey(objectType) && mPropertiesPool[objectType].Count() > 0)
			fieldProperty = dynamic_cast<ObjectProperty*>(mPropertiesPool[objectType].PopBack());
		else
			fieldProperty = mnew ObjectProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateObjectPtrField(const ObjectType* basicType, const String& name, 
													 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
													 const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		ObjectPtrProperty* fieldProperty = nullptr;

		const Type* objectType = &TypeOf(IObject*);
		if (mPropertiesPool.ContainsKey(objectType) && mPropertiesPool[objectType].Count() > 0)
			fieldProperty = dynamic_cast<ObjectPtrProperty*>(mPropertiesPool[objectType].PopBack());
		else
			fieldProperty = mnew ObjectPtrProperty();

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
		VectorProperty* fieldProperty = nullptr;

		if (mPropertiesPool.ContainsKey(type) && mPropertiesPool[type].Count() > 0)
			fieldProperty = dynamic_cast<VectorProperty*>(mPropertiesPool[type].PopBack());
		else 
			fieldProperty = mnew VectorProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SpecializeType(type);
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IObjectPropertiesViewer* Properties::CreateObjectViewer(const Type* type, const String& path, 
															const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
															const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		auto viewerType = GetClosesBasedTypeObjectViewer(type);
		if (!viewerType)
			viewerType = &TypeOf(DefaultObjectPropertiesViewer);

		IObjectPropertiesViewer* viewer = nullptr;

		if (mObjectPropertiesViewersPool.ContainsKey(type))
		{
			if (!mObjectPropertiesViewersPool[type].IsEmpty())
				viewer = mObjectPropertiesViewersPool[type].PopBack();
		}

		if (!viewer)
			viewer = (IObjectPropertiesViewer*)(viewerType->CreateSample());

		viewer->path = path;
		viewer->onChanged = onChanged;
		viewer->onChangeCompleted = onChangeCompleted;

		return viewer;
	}

	void Properties::FreeObjectViewer(IObjectPropertiesViewer* viewer)
	{
		auto type = viewer->GetViewingObjectType();

		if (!mObjectPropertiesViewersPool.ContainsKey(type))
			mObjectPropertiesViewersPool.Add(type, Vector<IObjectPropertiesViewer*>());

		mObjectPropertiesViewersPool[type].Add(viewer);
		viewer->GetLayout()->SetParent(nullptr);
	}

	String Properties::MakeSmartFieldName(const String& fieldName)
	{
		String begn;

		if (fieldName[0] == 'm' && fieldName[1] >= 'A' && fieldName[1] <= 'Z')
			begn = fieldName.SubStr(1);
		else if (fieldName[0] == 'm' && fieldName[1] == '_')
			begn = fieldName.SubStr(2);
		else if (fieldName[0] == '_')
			begn = fieldName.SubStr(1);
		else
			begn = fieldName;

		if (begn.StartsWith("o2::"))
			begn.Erase(0, 4);

		if (begn.StartsWith("Editor::"))
			begn.Erase(0, 9);

		if (begn.StartsWith("UI"))
			begn = begn;

		String res;
		int len = begn.Length();
		bool newWord = true;
		bool lastUpper = false;
		for (int i = 0; i < len; i++)
		{
			if (begn[i] == '_')
			{
				res += ' ';
				newWord = true;
				lastUpper = false;
			}
			else if (newWord && begn[i] >= 'a' && begn[i] <= 'z')
			{
				res += begn[i] + ('A' - 'a');
				lastUpper = true;
			}
			else if (!newWord && begn[i] >= 'A' && begn[i] <= 'Z')
			{
				if (!lastUpper)
					res += ' ';

				res += begn[i];
				lastUpper = begn[i] >= 'A' && begn[i] <= 'Z';
			}
			else if (i < len - 1 && begn[i] == ':' && begn[i + 1] == ':')
			{
				res += ": ";
				lastUpper = false;
				i++;
			}
			else
			{
				res += begn[i];
				lastUpper = begn[i] >= 'A' && begn[i] <= 'Z';
			}

			newWord = begn[i] >= '0' && begn[i] <= '9';
		}

		return res;
	}

	const Type* Properties::GetFieldPropertyType(const Type* valueType) const
	{
		for (auto kv : mAvailablePropertiesFields)
		{
			if (valueType->GetUsage() == Type::Usage::Pointer && kv.first->GetUsage() == Type::Usage::Pointer)
			{
				if (((PointerType*)valueType)->GetUnpointedType()->IsBasedOn(*((PointerType*)kv.first)->GetUnpointedType()))
					return kv.second;
			}
			else if (valueType->IsBasedOn(*kv.first))
				return kv.second;
		}

		return nullptr;
	}
}
