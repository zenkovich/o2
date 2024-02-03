#include "o2Editor/stdafx.h"
#include "ObjectProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/NoHeaderAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"

using namespace o2;

namespace Editor
{
	ObjectProperty::ObjectProperty()
	{
		InitializeControls();
	}

	ObjectProperty::ObjectProperty(const ObjectProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	ObjectProperty& ObjectProperty::operator=(const ObjectProperty& other)
	{
		IPropertyField::operator=(other);
		InitializeControls();
		return *this;
	}

	void ObjectProperty::OnFreeProperty()
	{
		if (mObjectViewer)
		{
			RemoveChild(mObjectViewer->GetSpoiler(), false);
			o2EditorProperties.FreeObjectViewer(mObjectViewer);
		}

		mObjectViewer = nullptr;
	}

	void ObjectProperty::InitializeControls()
	{
		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	void ObjectProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		for (auto& pair : mTargetObjects)
		{
			if (pair.first.isCreated)
				delete pair.first.data;

			if (pair.second.isCreated)
				delete pair.second.data;
		}

		mValuesProxies = targets;
		mTargetObjects.Clear();

		for (auto& pair : targets)
			mTargetObjects.Add({ GetObjectFromProxy(pair.first), GetObjectFromProxy(pair.second) });

		Refresh();
	}

	void ObjectProperty::Refresh()
	{
		PushEditorScopeOnStack scope;

		for (auto& pair : mTargetObjects)
		{
			pair.first.Refresh();
			pair.second.Refresh();
		}

		CheckViewer();

		if (mObjectViewer)
		{
			mObjectViewer->Refresh(mTargetObjects.Convert<Pair<IObject*, IObject*>>(
				[&](const Pair<TargetObjectData, TargetObjectData>& x)
				{
					return Pair<IObject*, IObject*>(x.first.data, x.second.data);
				}));
		}

		SetEnabled(mObjectViewer && !mObjectViewer->IsEmpty() || !mNoHeader);
	}

	void ObjectProperty::CheckViewer()
	{
		PushEditorScopeOnStack scope;

		const Type* objectType = nullptr;
		if (!mTargetObjects.IsEmpty())
		{
			auto object = mTargetObjects[0].first.data;
			if (object)
				objectType = &object->GetType();
		}

		const Type* prevObjectType = mObjectViewer ? mObjectViewer->GetViewingObjectType() : nullptr;
		if (objectType != prevObjectType)
		{
			if (mObjectViewer)
				o2EditorProperties.FreeObjectViewer(mObjectViewer);

			if (objectType)
			{
				mObjectViewer = o2EditorProperties.CreateObjectViewer(objectType, mValuesPath,
																	  THIS_FUNC(OnPropertyChanged),
																	  onChanged);

				AddChild(mObjectViewer->GetSpoiler());
				mObjectViewer->SetParentContext(mParentContext);
				mObjectViewer->SetHeaderEnabled(!mNoHeader);
				mObjectViewer->SetExpanded(mExpanded);
				mObjectViewer->GetSpoiler()->SetCaption(mCaption);
			}
		}
	}

	const Type* ObjectProperty::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* ObjectProperty::GetValueTypeStatic()
	{
		return &TypeOf(IObject);
	}

	void ObjectProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);

		if (fieldInfo)
		{
			mExpanded = fieldInfo->HasAttribute<ExpandedByDefaultAttribute>();
			mNoHeader = fieldInfo->HasAttribute<NoHeaderAttribute>();
		}
	}

	void ObjectProperty::SetCaption(const WString& text)
	{
		mCaption = text;

		if (mObjectViewer)
			mObjectViewer->GetSpoiler()->SetCaption(mCaption);;
	}

	WString ObjectProperty::GetCaption() const
	{
		return mCaption;
	}

	Button* ObjectProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			*mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, 0));
			AddInternalWidget(mRemoveBtn);
		}

		return mRemoveBtn;
	}

	void ObjectProperty::Expand()
	{
		SetExpanded(true);
	}

	void ObjectProperty::Collapse()
	{
		SetExpanded(false);
	}

	void ObjectProperty::SetExpanded(bool expanded)
	{
		mExpanded = expanded;

		if (mObjectViewer)
			mObjectViewer->GetSpoiler()->SetExpanded(expanded);
	}

	bool ObjectProperty::IsExpanded() const
	{
		if (mObjectViewer)
			return mObjectViewer->GetSpoiler()->IsExpanded();

		return mExpanded;
	}

	ObjectProperty::TargetObjectData ObjectProperty::GetObjectFromProxy(IAbstractValueProxy* proxy)
	{
		TargetObjectData res;

		if (!proxy)
			return res;

		res.proxy = proxy;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());

		bool usedRawPointer = false;
		if (auto pointerProxy = dynamic_cast<IPointerValueProxy*>(proxy))
		{
			Ref<IObject> object = objectType.DynamicCastToIObject(pointerProxy->GetValueVoidPointer());
			res.data = object;
			res.isCreated = false;
			usedRawPointer = true;
		}

#if IS_SCRIPTING_SUPPORTED
		if (auto scriptProxy = dynamic_cast<ScriptValueProxy*>(proxy))
		{
			ScriptValue value = scriptProxy->scriptProperty->Get();
			if (value.IsObject())
			{
				res.data = m_make<IObject>(value.ToIObject());
				res.isCreated = true;
			}
		}
#endif

		if (!usedRawPointer)
		{
			res.data = m_make<IObject>(nullptr);
			res.isCreated = false;
		}

		return res;
	}

	void ObjectProperty::OnPropertyChanged()
	{
		if (mPropertyDelegate && mPropertyInfo)
		{
			PushEditorScopeOnStack scope;

			for (auto& pair : mTargetObjects)
			{
				mPropertyDelegate->Invoke(mPropertyInfo, GetValueFromObject(pair.first.data), GetValueFromObject(pair.second.data));
			}
		}
	}

	void ObjectProperty::Bind(PropertyChangedCallback callback)
	{
		onChanged = callback;

		if (mObjectViewer)
			mObjectViewer->Bind([this](const Pair<IObject*, IObject*>& values)
			{
				PushEditorScopeOnStack scope;

				Pair<Ref<IObject>, Ref<IObject>> objectsToSet = values.Convert<Pair<Ref<IObject>, Ref<IObject>>>(
					[](const Pair<IObject*, IObject*>& x)
					{
						return Pair<Ref<IObject>, Ref<IObject>>(x.first, x.second);
					});

				SetValueAndPrototypeProxy({ objectsToSet });
			});

		IPropertyField::Bind(callback);
	}

	void ObjectProperty::DisableObjectViewer()
	{
		if (mObjectViewer)
		{
			mObjectViewer->SetEnabled(false);
			mObjectViewer->SetEditEnabled(false);
		}
	}

	void ObjectProperty::EnableObjectViewer()
	{
		if (mObjectViewer)
		{
			mObjectViewer->SetEnabled(true);
			mObjectViewer->SetEditEnabled(true);
		}
	}

	void ObjectProperty::AddProperty(IPropertyField* field)
	{
		if (mObjectViewer)
			mObjectViewer->AddProperty(field);
	}

	bool ObjectProperty::RemoveProperty(IPropertyField* field)
	{
		if (mObjectViewer)
			return mObjectViewer->RemoveProperty(field);

		return false;
	}

	void ObjectProperty::RemoveAllProperties()
	{
		if (mObjectViewer)
			mObjectViewer->RemoveAllProperties();
	}

	const IObjectPropertiesViewer* ObjectProperty::GetObjectViewer() const
	{
		return mObjectViewer;
	}

	void ObjectProperty::SetValuesPath(const WString& path)
	{
		mValuesPath = path;

		if (mObjectViewer)
			mObjectViewer->SetValuesPath(path);
	}

	const WString& ObjectProperty::GetValuesPath() const
	{
		return mValuesPath;
	}

	void ObjectProperty::HideMobjectViewerHeader()
	{
		mNoHeader = true;

		if (mObjectViewer)
			mObjectViewer->SetHeaderEnabled(false);
	}

	void ObjectProperty::ShowMobjectViewerHeader()
	{
		mNoHeader = false;

		if (mObjectViewer)
			mObjectViewer->SetHeaderEnabled(true);
	}

	void ObjectProperty::SetParentContext(Context parent)
	{
		mParentContext = parent;

		if (mObjectViewer)
			mObjectViewer->SetParentContext(parent);
	}

	Context ObjectProperty::GetParentContext() const
	{
		return mParentContext;
	}

	void ObjectProperty::AddInternalWidget(Widget* widget)
	{
		AddChild(widget);
	}

	void ObjectProperty::RemoveInternalWidget(Widget* widget)
	{
		RemoveChild(widget);
	}

	void ObjectProperty::SetInternalWidgetsVisible(bool visible)
	{
		for (auto widget : GetChildren())
		{
			if (widget != mRemoveBtn)
			{
				widget->SetVisible(visible);
			}
		}
	}
}#include <Ref.h>

namespace Editor {
	template<typename T>
	using Ref = SmartPtr<T>;

	template<typename T>
	using WeakRef = SmartPtr<T>;

	class ObjectProperty
	{
	private:
		struct TargetObjectData
		{
			Ref<IObjectProxy> proxy;
			Ref<IObject> data;
			bool isCreated;
			void Refresh();
			void SetValue();
		};
		
		Vector< std::pair< Ref<IObjectProperty>, TargetObjectData > > mTargetObjects;

	public:
		Ref<IObjectValue> ObjectProperty::GetValue(const IObject& obj)
		{
			Ref<IObjectValue> res;

			bool usedRawPointer = false;

#ifdef SMART_PTR
			if (!mTargetObjects.empty())
			{
				const ObjectType& objectType = dynamic_cast<const ObjectType&>(obj.GetType());

				for (const auto& pair : mTargetObjects)
				{
					const Ref<IObject>& targetObject = pair.second.data;
					Ref<IObjectProxy> proxy = pair.second.proxy;

					Ref<IValue> value = proxy->GetValuePtr(targetObject);
					if (!value)
						continue;

					res = objectType.CreateValue();
					ObjectValue& objectValue = dynamic_cast<ObjectValue&>(*res);

					Ref<IObject> valueObject = objectType.CreateObject();

					if (const IObject* valueRawObject = dynamic_cast<const IObject*>(value.get()))
					{
						*valueObject = *valueRawObject;
					}
					else if (auto valueContainer = dynamic_cast<const IObjectContainer*>(value.get()))
					{
						*valueObject = *valueContainer->GetObject();
					}
					else
					{
						valueObject.reset(value->AddRefRawObject());
					}

					objectValue.SetContainer(valueObject);

					if (!objectValue.GetContainer() || (objectValue.GetContainer() && objectValue.GetContainer()->IsEmpty()))
					{
						objectValue.PutPrimitiveValue(value);
					}

					break;
				}

				if (!res)
				{
					// If no target objects, take first non-empty object as default
					res = objectType.CreateValue();
					ObjectValue& objectValue = dynamic_cast<ObjectValue&>(*res);

					Ref<IObject> valueObject = objectType.CreateObject();
					objectValue.SetContainer(valueObject);

					Ref<IValue> value = objectValue.GetContainer()->GetObject();
					if (value)
					{
						objectValue.PutPrimitiveValue(value);
					}
				}
			}
#endif
			if (!usedRawPointer)
			{
				void* sample = objectType.CreateSample();
				proxy->GetValuePtr(sample);

				res.data = objectType.DynamicCastToIObject(sample);
				res.isCreated = true;
			}

			return res;
		}

		void ObjectProperty::OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
											   const Vector<DataDocument>& after)
		{
			for (const auto& pair : mTargetObjects)
				pair.first.SetValue();

			onChangeCompleted(path, before, after);
		}
	};
}

// --- META ---

DECLARE_CLASS(Editor::ObjectProperty, Editor__ObjectProperty);

// --- END META ---