#include "stdafx.h"
#include "Properties.h"

#include "Basic/AssetProperty.h"
#include "Basic/EnumProperty.h"
#include "Basic/ObjectProperty.h"
#include "Basic/ObjectPtrProperty.h"
#include "Basic/VectorProperty.h"
#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Core/Properties/ObjectViewers/DefaultObjectViewer.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/WidgetLayout.h"
#include "Utils/System/Time/Timer.h"
#include "Utils/Editor/Attributes/InvokeOnChangeAttribute.h"

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
		for (auto field : mAvailablePropertiesFields)
			delete field;
	}

	void Properties::InitializeAvailablePropertiesFields()
	{
		auto a = TypeOf(IAssetProperty).GetDerivedTypes();
		auto b = TypeOf(IPropertyField).GetDerivedTypes();
		auto avaialbleTypes = a + b;

		avaialbleTypes.Remove(&TypeOf(IAssetProperty));
		avaialbleTypes.Remove(&TypeOf(ObjectProperty));
		avaialbleTypes.Remove(&TypeOf(ObjectPtrProperty));
		avaialbleTypes.Remove(&TypeOf(VectorProperty));
		avaialbleTypes.RemoveAll([](const Type* type) { return type->GetName().Contains("TPropertyField"); });

		for (auto x : avaialbleTypes)
		{
			auto objType = dynamic_cast<const ObjectType*>(x);
			auto sample = dynamic_cast<IPropertyField*>(objType->DynamicCastToIObject(objType->CreateSample()));

			if (!sample->GetValueType())
				delete sample;
			else 
				mAvailablePropertiesFields.Add(sample);
		}
	}

	void Properties::InitializeAvailableObjectPropertiesViewers()
	{
		auto availableTypes = TypeOf(IObjectPropertiesViewer).GetDerivedTypes();
		availableTypes.Remove(&TypeOf(IObjectPropertiesViewer));
		availableTypes.Remove(&TypeOf(DefaultObjectViewer));

		for (auto x : availableTypes)
		{
			auto sample = (IObjectPropertiesViewer*)x->CreateSample();
			mAvailableObjectPropertiesViewers.Add(sample);
		}
	}

	const Type* Properties::GetClosesBasedTypeObjectViewer(const Type* type) const
	{
		IObjectPropertiesViewer* viwerSample = nullptr;

		int minBaseDepth = INT_MAX;
		for (auto viewer : mAvailableObjectPropertiesViewers)
		{
			auto viewerType = viewer->GetViewingObjectType();
			Vector<const Type*> itTypes = { viewerType };
			int thisBaseDepth = 0;
			bool found = false;
			while (!itTypes.IsEmpty())
			{
				Vector<const Type*> nextItTypes;

				for (auto t : itTypes)
				{
					if (viewerType == type)
					{
						break;
						found = true;
					}

					nextItTypes.Add(t->GetBaseTypes().Select<const Type*>([](const Type::BaseType& x) { return x.type; }));
				}

				if (found)
					break;

				itTypes = nextItTypes;
				thisBaseDepth++;
			}

			if (found && thisBaseDepth < minBaseDepth)
			{
				minBaseDepth = thisBaseDepth;
				viwerSample = viewer;

				if (thisBaseDepth == 0)
					break;
			}
		}

		auto viewerType = viwerSample ? &viwerSample->GetType() : &TypeOf(DefaultObjectViewer);
		return viewerType;
	}

	IPropertyField* Properties::BuildField(VerticalLayout* layout, FieldInfo* fieldInfo,
										   PropertiesContext& propertiesInfo, const String& path,
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
		fieldWidget->SetValuePath(path + fieldInfo->GetName());
		fieldWidget->SetFieldInfo(fieldInfo);

		if (auto invokeOnChangeAttribute = fieldInfo->GetAttribute<InvokeOnChangeAttribute>())
		{
			fieldWidget->onChangeCompleted += [&, invokeOnChangeAttribute](const String&, const Vector<DataNode>&, const Vector<DataNode>&) {
				for (auto target : propertiesInfo.mTargets) 
				{
					auto& targetType = target.first->GetType();
					auto& targetObjType = dynamic_cast<const ObjectType&>(targetType);
					targetObjType.Invoke<void>(invokeOnChangeAttribute->methodName, targetObjType.DynamicCastFromIObject(target.first));
				}
			};
		}

		layout->AddChild(fieldWidget);

		propertiesInfo.mProperties.Add(fieldInfo, fieldWidget);

		//o2Debug.Log("Field " + path + "/" + fieldInfo->GetName() + " for " + (String)timer.GetDeltaTime());

		return fieldWidget;
	}

	IPropertyField* Properties::BuildField(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
										   PropertiesContext& propertiesInfo, 
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/, 
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		return BuildField(layout, objectType.GetField(fieldName), propertiesInfo, path, onChangeCompleted, onChanged);
	}

	void Properties::BuildFields(VerticalLayout* layout, Vector<FieldInfo*> fields,
								 PropertiesContext& propertiesInfo, const String& path,
								 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
								 const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		Timer timer; 

		for (auto fieldInfo : fields)
			BuildField(layout, fieldInfo, propertiesInfo, path, onChangeCompleted, onChanged);

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

		IPropertyField* fieldSample = GetFieldPropertyPrototype(type);
		if (fieldSample)
			return true;

		return false;
	}

	void Properties::FreeProperties(PropertiesContext& propertiesInfo)
	{
		for (auto prop : propertiesInfo.mProperties)
			FreeProperty(prop.second);

		propertiesInfo.mProperties.Clear();
	}

	void Properties::FreeProperty(IPropertyField* field)
	{
		if (!mPropertiesPool.ContainsKey(field->GetValueType()))
			mPropertiesPool.Add(field->GetValueType(), PropertiesFieldsVec());

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
										   PropertiesContext& propertiesInfo, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		BuildObjectProperties(layout, type->GetFieldsWithBaseClasses(), propertiesInfo, path, onChangeCompleted, onChanged);
	}

	void Properties::BuildObjectProperties(VerticalLayout* layout, Vector<FieldInfo*> fields,
										   PropertiesContext& propertiesInfo, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		PushEditorScopeOnStack scope;

		Vector<FieldInfo*> regularFields = fields.FindAll(
			[&](FieldInfo* x) { return IsPropertyVisible(x, false); });

		BuildFields(layout, regularFields, propertiesInfo, path, onChangeCompleted, onChanged);

		if (mPrivateVisible)
		{
			Vector<FieldInfo*> privateFields = fields.FindAll(
				[&](FieldInfo* x) { return IsPropertyVisible(x, true) && !regularFields.Contains(x); });

			if (!privateFields.IsEmpty())
			{
				Spoiler* privates = propertiesInfo.mPrivatePropertiesSpoiler;

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

				propertiesInfo.mPrivatePropertiesSpoiler = privates;
				BuildFields(privates, privateFields, propertiesInfo, path, onChangeCompleted, onChanged);
			}
		}
		else if (propertiesInfo.mPrivatePropertiesSpoiler)
		{
			propertiesInfo.mPrivatePropertiesSpoiler->SetEnabled(false);
		}

		propertiesInfo.mBuildWithPrivateProperties = mPrivateVisible;
	}

	IPropertyField* Properties::CreateFieldProperty(const Type* type, const String& name,
													const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
													const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		PushEditorScopeOnStack enterScope;

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
			if (dynamic_cast<const PointerType*>(type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
				return CreateObjectPtrField(name, onChangeCompleted, onChanged);

			return nullptr;
		}

		if (type->GetUsage() == Type::Usage::Enumeration)
			return CreateEnumField(type, name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = dynamic_cast<const PropertyType*>(type)->GetValueType();
			return CreateFieldProperty(valueType, name, onChangeCompleted, onChanged);
		}

		return CreateRegularField(type, name, onChangeCompleted, onChanged);
	}

	IPropertyField* Properties::CreateRegularField(const Type* type, const String& name,
												   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
												   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		const Type* fieldPropertyType = nullptr;
		if (auto fieldSample = GetFieldPropertyPrototype(type))
			fieldPropertyType = &fieldSample->GetType();

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
		IPropertyField* fieldProperty = nullptr;

		const Type* objectType = &TypeOf(IObject);
		if (mPropertiesPool.ContainsKey(objectType) && mPropertiesPool[objectType].Count() > 0)
			fieldProperty = mPropertiesPool[objectType].PopBack();
		else
			fieldProperty = mnew ObjectProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateObjectPtrField(const String& name,
													 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
													 const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		IPropertyField* fieldProperty = nullptr;

		const Type* objectType = &TypeOf(IObject*);
		if (mPropertiesPool.ContainsKey(objectType) && mPropertiesPool[objectType].Count() > 0)
			fieldProperty = mPropertiesPool[objectType].PopBack();
		else
			fieldProperty = mnew ObjectPtrProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

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
			mObjectPropertiesViewersPool.Add(type, IObjectPropertiesViewersVec());

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

	IPropertyField* Properties::GetFieldPropertyPrototype(const Type* type) const
	{
		for (auto field : mAvailablePropertiesFields)
		{
			auto fieldType = field->GetValueType();
			if (type->GetUsage() == Type::Usage::Pointer && fieldType->GetUsage() == Type::Usage::Pointer)
			{
				if (((PointerType*)type)->GetUnpointedType()->IsBasedOn(*((PointerType*)fieldType)->GetUnpointedType()))
					return field;
			}
			else if (type->IsBasedOn(*fieldType))
				return field;
		}

		return nullptr;
	}
}
