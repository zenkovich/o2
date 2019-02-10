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

DECLARE_SINGLETON(Editor::Properties);

namespace Editor
{
	Editor::IPropertyField::OnChangeCompletedFunc Properties::mOnPropertyCompletedChangingUndoCreateDelegate;

	Properties::Properties()
	{
		InitializeAvailablePropertiesFields();
		InitializeAvailableObjectPropertiesViewers();

		mOnPropertyCompletedChangingUndoCreateDelegate = Func<EditorApplication, void, const String&, const Vector<DataNode>&, const Vector<DataNode>&>(
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
		avaialbleTypes.RemoveAll([](const Type* type) { return type->GetName().Contains("TPropertyField"); });

		for (auto x : avaialbleTypes)
		{
			auto sample = (IPropertyField*)x->CreateSample();
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

	IPropertyField* Properties::BuildField(UIVerticalLayout* layout, FieldInfo* fieldInfo,
										   FieldPropertiesInfo& propertiesInfo, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		Timer timer;

		const Type* fieldType = fieldInfo->GetType();
		String propertyName = MakeSmartFieldName(fieldInfo->GetName());

		auto fieldWidget = CreateFieldProperty(fieldType, propertyName, onChangeCompleted, onChanged);
		if (!fieldWidget)
			return nullptr;

		fieldWidget->SetValuePath(path + fieldInfo->GetName());
		fieldWidget->SpecializeFieldInfo(fieldInfo);

		layout->AddChild(fieldWidget, false);

		propertiesInfo.properties.Add(fieldInfo, fieldWidget);

		o2Debug.Log("Field " + path + "/" + fieldInfo->GetName() + " for " + (String)timer.GetDeltaTime());

		return fieldWidget;
	}

	IPropertyField* Properties::BuildField(UIVerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
										   FieldPropertiesInfo& propertiesInfo, 
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/, 
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		return BuildField(layout, objectType.GetField(fieldName), propertiesInfo, path, onChangeCompleted, onChanged);
	}

	void Properties::BuildFields(UIVerticalLayout* layout, Vector<FieldInfo*> fields,
								 FieldPropertiesInfo& propertiesInfo, const String& path,
								 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
								 const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		Timer t; // tags, layer, enum
		for (auto fieldInfo : fields)
			BuildField(layout, fieldInfo, propertiesInfo, path, onChangeCompleted, onChanged);

		o2Debug.Log(">>> Fields created for " + (String)t.GetDeltaTime());
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

		IPropertyField* fieldSample = GetFieldPropertyPrototype(type);
		if (fieldSample)
			return true;

		if (type->IsBasedOn(TypeOf(IObject)))
			return true;

		if (type->GetUsage() == Type::Usage::Pointer && ((PointerType*)type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			return true;

		if (type->GetUsage() == Type::Usage::Enumeration)
			return true;

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = ((const PropertyType*)type)->GetValueType();

			if (valueType->GetUsage() == Type::Usage::Enumeration)
				return true;

			fieldSample = GetFieldPropertyPrototype(valueType);
			if (fieldSample)
				return true;
		}

		return false;
	}

	void Properties::FreeProperties(FieldPropertiesInfo& propertiesInfo)
	{
		for (auto prop : propertiesInfo.properties)
			FreeProperty(prop.Value());

		propertiesInfo.properties.Clear();
	}

	void Properties::FreeProperty(IPropertyField* field)
	{
		if (!mPropertiesPool.ContainsKey(&field->GetType()))
			mPropertiesPool.Add(&field->GetType(), PropertiesFieldsVec());

		mPropertiesPool[&field->GetType()].Add(field);
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

	void Properties::BuildObjectProperties(UIVerticalLayout* layout, const Type* type,
										   FieldPropertiesInfo& propertiesInfo, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		BuildObjectProperties(layout, type->GetFieldsWithBaseClasses(), propertiesInfo, path, onChangeCompleted, onChanged);
	}

	void Properties::BuildObjectProperties(UIVerticalLayout* layout, Vector<FieldInfo*> fields,
										   FieldPropertiesInfo& propertiesInfo, const String& path,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
										   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		PushScopeEnterOnStack scope;

		Vector<FieldInfo*> regularFields = fields.FindAll(
			[&](FieldInfo* x) { return IsPropertyVisible(x, false); });

		BuildFields(layout, regularFields, propertiesInfo, path, onChangeCompleted, onChanged);

		if (mPrivateVisible)
		{
			Vector<FieldInfo*> privateFields = fields.FindAll(
				[&](FieldInfo* x) { return IsPropertyVisible(x, true) && !regularFields.Contains(x); });

			if (!privateFields.IsEmpty())
			{
				UISpoiler* privates = propertiesInfo.privatePropertiesSpoiler;

				if (!privates)
					privates = layout->GetChildByType<UISpoiler>("privates");

				if (!privates)
				{
					privates = o2UI.CreateWidget<UISpoiler>("expand with caption");
					privates->name = "privates";
					privates->SetCaption("Private");
					layout->AddChild(privates);
				}
				else privates->SetIndexInSiblings(layout->GetChildren().Count() - 1);

				propertiesInfo.privatePropertiesSpoiler = privates;
				BuildFields(privates, privateFields, propertiesInfo, path, onChangeCompleted, onChanged);
			}
		}
		else if (propertiesInfo.privatePropertiesSpoiler)
		{
			propertiesInfo.privatePropertiesSpoiler->SetEnabled(false);
		}
	}

	IPropertyField* Properties::CreateFieldProperty(const Type* type, const String& name,
													const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
													const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		PushScopeEnterOnStack enterScope;

		if (type->GetUsage() == Type::Usage::Vector)
			return CreateVectorField(type, name, onChangeCompleted, onChanged);

		IPropertyField* fieldSample = GetFieldPropertyPrototype(type);
		if (fieldSample)
			return CreateRegularField(&fieldSample->GetType(), name, onChangeCompleted, onChanged);

		if (type->IsBasedOn(TypeOf(IObject)))
			return CreateObjectField(type, name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Pointer && ((PointerType*)type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			return CreateObjectPtrField(type, name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Enumeration)
			return CreateRegularField(&TypeOf(EnumProperty), name, onChangeCompleted, onChanged);

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = ((const PropertyType*)type)->GetValueType();
			return CreateFieldProperty(valueType, name, onChangeCompleted, onChanged);
		}

		return nullptr;
	}

	IPropertyField* Properties::CreateRegularField(const Type* fieldPropertyType, const String& name,
												   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
												   const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		IPropertyField* fieldProperty = nullptr;

		if (mPropertiesPool.ContainsKey(fieldPropertyType) && mPropertiesPool[fieldPropertyType].Count() > 0)
			fieldProperty = mPropertiesPool[fieldPropertyType].PopBack();
		else
			fieldProperty = dynamic_cast<IPropertyField*>(o2UI.CreateWidget(*fieldPropertyType, "with caption"));

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateObjectField(const Type* type, const String& name,
												  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
												  const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		IPropertyField* fieldProperty = nullptr;

		const Type* objectPropertyType = &TypeOf(ObjectProperty);
		if (mPropertiesPool.ContainsKey(objectPropertyType) && mPropertiesPool[objectPropertyType].Count() > 0)
		{
			IPropertyField* sameTypeProperty = mPropertiesPool[objectPropertyType].
				FindMatch([=](IPropertyField* fld) { return fld->GetSpecializedType() == type; });

			if (sameTypeProperty)
			{
				fieldProperty = sameTypeProperty;
				mPropertiesPool[objectPropertyType].Remove(fieldProperty);
			}
			else fieldProperty = mPropertiesPool[objectPropertyType].PopBack();
		}
		else fieldProperty = mnew ObjectProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateObjectPtrField(const Type* type, const String& name,
													 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
													 const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		IPropertyField* fieldProperty = nullptr;

		const Type* objectPropertyType = &TypeOf(ObjectPtrProperty);
		if (mPropertiesPool.ContainsKey(objectPropertyType) && mPropertiesPool[objectPropertyType].Count() > 0)
		{
			IPropertyField* sameTypeProperty = mPropertiesPool[objectPropertyType].
				FindMatch([=](IPropertyField* fld) { return fld->GetSpecializedType() == type; });

			if (sameTypeProperty)
			{
				fieldProperty = sameTypeProperty;
				mPropertiesPool[objectPropertyType].Remove(fieldProperty);
			}
			else fieldProperty = mPropertiesPool[objectPropertyType].PopBack();
		}
		else fieldProperty = mnew ObjectPtrProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IPropertyField* Properties::CreateVectorField(const Type* type, const String& name,
												  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
												  const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		if (!IsFieldTypeSupported(type))
			return nullptr;

		IPropertyField* fieldProperty = nullptr;

		const Type* objectPropertyType = &TypeOf(VectorProperty);
		if (mPropertiesPool.ContainsKey(objectPropertyType) && mPropertiesPool[objectPropertyType].Count() > 0)
		{
			IPropertyField* sameTypeProperty = mPropertiesPool[objectPropertyType].
				FindMatch([=](IPropertyField* fld) { return fld->GetSpecializedType() == type; });

			if (sameTypeProperty)
			{
				fieldProperty = sameTypeProperty;
				mPropertiesPool[objectPropertyType].Remove(fieldProperty);
			}
			else fieldProperty = mPropertiesPool[objectPropertyType].PopBack();
		}
		else fieldProperty = mnew VectorProperty();

		fieldProperty->onChanged = onChanged;
		fieldProperty->onChangeCompleted = onChangeCompleted;
		fieldProperty->SetCaption(name);

		return fieldProperty;
	}

	IObjectPropertiesViewer* Properties::CreateObjectViewer(const Type* type, const String& path,
															const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/,
															const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		auto sample = mAvailableObjectPropertiesViewers.FindMatch([=](IObjectPropertiesViewer* x) {
			return type->IsBasedOn(*x->GetViewingObjectType()); });

		auto& viewerType = sample ? sample->GetType() : TypeOf(DefaultObjectViewer);

		IObjectPropertiesViewer* viewer = nullptr;

		if (mObjectPropertiesViewersPool.ContainsKey(type))
		{
			if (!mObjectPropertiesViewersPool[type].IsEmpty())
				viewer = mObjectPropertiesViewersPool[type].PopBack();
		}

		if (!viewer)
			viewer = (IObjectPropertiesViewer*)(viewerType.CreateSample());

		viewer->path = path;
		viewer->onChanged = onChanged;
		viewer->onChangeCompleted = onChangeCompleted;

		return viewer;
	}

	void Properties::FreeObjectViewer(IObjectPropertiesViewer* viewer, const Type* type)
	{
		if (!mObjectPropertiesViewersPool.ContainsKey(type))
			mObjectPropertiesViewersPool.Add(type, IObjectPropertiesViewersVec());

		mObjectPropertiesViewersPool[type].Add(viewer);
		viewer->GetViewWidget()->SetParent(nullptr);
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
			auto fieldType = field->GetFieldType();
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
