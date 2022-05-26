#include "o2Editor/stdafx.h"
#include "ScriptValueProperty.h"

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
	ScriptValueProperty::ScriptValueProperty()
	{
		InitializeControls();
	}

	ScriptValueProperty::ScriptValueProperty(const ScriptValueProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	ScriptValueProperty& ScriptValueProperty::operator=(const ScriptValueProperty& other)
	{
		IPropertyField::operator=(other);
		InitializeControls();
		return *this;
	}

	void ScriptValueProperty::OnFreeProperty()
	{
	}

	void ScriptValueProperty::InitializeControls()
	{
		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;

		mSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");
		AddChild(mSpoiler);
	}

	Map<String, Vector<Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>>> ScriptValueProperty::GetCommonProperties(
		const Vector<Pair<ScriptValue, ScriptValue>>& values) const
	{
		Map<String, Vector<Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>>> res;

		for (auto kv : values)
		{
			kv.first.ForEachProperties(
				[&](const ScriptValue& name, const ScriptValue& value)
				{
					Pair<o2::ScriptValueProperty, o2::ScriptValueProperty> elem;
					elem.first = { kv.first, name };
					if (kv.second.IsObject())
						elem.second = { kv.second, name };

					res[name.ToString()].Add(elem);
					return true;
				});
		}

		// Leave only common properties
		res.RemoveAll([&](const String& k, const auto& v)
					  {
						  return v.Count() != values.Count();
					  });

		return res;
	}

	void ScriptValueProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mNeedUpdateProxies = targets != mValuesProxies;
		IPropertyField::SetValueAndPrototypeProxy(targets);
	}

	void ScriptValueProperty::Refresh()
	{
		PushEditorScopeOnStack scope;

		// Get values from proxies
		Vector<Pair<ScriptValue, ScriptValue>> values =
			mValuesProxies.Convert<Pair<ScriptValue, ScriptValue>>(
				[](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
				{
					Pair<ScriptValue, ScriptValue> res;
					x.first->GetValuePtr(&res.first);

					if (x.second)
						x.second->GetValuePtr(&res.second);

					return res;
				}
		);

		// Get common properties
		auto commonProperties = GetCommonProperties(values);

		// Check that last built properties are same
		bool changedProperties = false;
		for (auto kv : commonProperties)
		{
			if (!mPreviousBuiltTypes.ContainsKey(kv.first) ||
				mPreviousBuiltTypes[kv.first] != kv.second[0].first.Get().GetValueType())
			{
				changedProperties = true;
				break;
			}
		}

		// Rebuild properties if needed
		if (changedProperties)
		{
			mPreviousBuiltTypes.Clear();

			for (auto kv : commonProperties)
			{
				auto& name = kv.first;
				auto value = kv.second[0].first.Get();
				auto type = value.GetValueType();
				mPreviousBuiltTypes[name] = type;

				if (type == ScriptValue::ValueType::Number)
					AddProperty(name, &TypeOf(float));
				else if (type == ScriptValue::ValueType::String)
					AddProperty(name, &TypeOf(String));
				else if (type == ScriptValue::ValueType::Bool)
					AddProperty(name, &TypeOf(bool));
				else if (type == ScriptValue::ValueType::Object)
				{
					auto prototype = value.GetPrototype();
					if (prototype == *ScriptValuePrototypes::GetVec2Prototype())
						AddProperty(name, &TypeOf(Vec2F));
					else if (prototype == *ScriptValuePrototypes::GetRectPrototype())
						AddProperty(name, &TypeOf(RectF));
					else if (prototype == *ScriptValuePrototypes::GetBorderPrototype())
						AddProperty(name, &TypeOf(BorderF));
					else if (prototype == *ScriptValuePrototypes::GetColor4Prototype())
						AddProperty(name, &TypeOf(Color4));
					else if (value.IsObjectContainer())
						AddProperty(name, value.GetObjectContainerType());
					else
						AddProperty(name, &TypeOf(ScriptValue));
				}
			}

			mNeedUpdateProxies = true;
		}

		for (auto kv : mBuiltProperties)
		{
			if (mNeedUpdateProxies)
			{
				auto& name = kv.first;
				auto value = commonProperties[kv.first][0].first.Get();
				auto type = value.GetValueType();
				auto field = kv.second;

				if (type == ScriptValue::ValueType::Number)
					SetFieldProxies<float>(commonProperties, name, field);
				else if (type == ScriptValue::ValueType::String)
					SetFieldProxies<String>(commonProperties, name, field);
				else if (type == ScriptValue::ValueType::Bool)
					SetFieldProxies<bool>(commonProperties, name, field);
				else if (type == ScriptValue::ValueType::Object)
				{
					auto prototype = value.GetPrototype();
					if (prototype == *ScriptValuePrototypes::GetVec2Prototype())
						SetFieldProxies<Vec2F>(commonProperties, name, field);
					else if (prototype == *ScriptValuePrototypes::GetRectPrototype())
						SetFieldProxies<RectF>(commonProperties, name, field);
					else if (prototype == *ScriptValuePrototypes::GetBorderPrototype())
						SetFieldProxies<BorderF>(commonProperties, name, field);
					else if (prototype == *ScriptValuePrototypes::GetColor4Prototype())
						SetFieldProxies<Color4>(commonProperties, name, field);
					else if (value.IsObjectContainer())
					{
						auto proxies = commonProperties[name].Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
							[](const Pair<o2::ScriptValueProperty, o2::ScriptValueProperty>& x)
							{
								Pair<IAbstractValueProxy*, IAbstractValueProxy*> res;
								res.first = mnew ScriptValueProxy(x.first);
								if (x.second.object.IsObject())
									res.second = mnew ScriptValueProxy(x.second);

								return res;
							});

						field->SetValueAndPrototypeProxy(proxies);
					}
					else
						SetFieldProxies<ScriptValue>(commonProperties, name, field);
				}
			}
			else
				kv.second->Refresh();
		}

		mNeedUpdateProxies = false;
	}

	void ScriptValueProperty::AddProperty(const String& name, const Type* type)
	{
		auto prop = o2EditorProperties.CreateFieldProperty(type, name);
		mSpoiler->AddChild(prop);
		mBuiltProperties.Add(name, prop);
	}

	const Type* ScriptValueProperty::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* ScriptValueProperty::GetValueTypeStatic()
	{
		return &TypeOf(ScriptValue);
	}

	void ScriptValueProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);

		if (fieldInfo)
		{
			mExpanded = fieldInfo->HasAttribute<ExpandedByDefaultAttribute>();
			SetHeaderEnabled(!fieldInfo->HasAttribute<NoHeaderAttribute>());
		}
	}

	void ScriptValueProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);;
	}

	WString ScriptValueProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
	}

	Button* ScriptValueProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			*mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, 0));
			AddInternalWidget(mRemoveBtn);
		}

		return mRemoveBtn;
	}

	void ScriptValueProperty::Expand()
	{
		SetExpanded(true);
	}

	void ScriptValueProperty::Collapse()
	{
		SetExpanded(false);
	}

	void ScriptValueProperty::SetExpanded(bool expanded)
	{
		mExpanded = expanded;
		mSpoiler->SetExpanded(expanded);
	}

	bool ScriptValueProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	void ScriptValueProperty::SetHeaderEnabled(bool enabled)
	{
		mHeaderEnabled = enabled;

		if (mHeaderEnabled)
		{
			mSpoiler->SetHeadHeight(20);
			mSpoiler->GetLayerDrawable<Text>("caption")->enabled = true;
			mSpoiler->GetInternalWidget("expand")->enabledForcibly = true;
			mSpoiler->borderLeft = 10;
			mSpoiler->borderTop = 2;
		}
		else
		{
			mSpoiler->SetHeadHeight(0);
			mSpoiler->GetLayerDrawable<Text>("caption")->enabled = false;
			mSpoiler->GetInternalWidget("expand")->enabledForcibly = false;
			mSpoiler->borderLeft = 0;
			mSpoiler->borderTop = 0;
			mSpoiler->Expand();
		}
	}

	bool ScriptValueProperty::IsHeaderEnabled() const
	{
		return mHeaderEnabled;
	}

	void ScriptValueProperty::OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
												const Vector<DataDocument>& after)
	{
		onChangeCompleted(path, before, after);
	}
}

DECLARE_CLASS(Editor::ScriptValueProperty);
