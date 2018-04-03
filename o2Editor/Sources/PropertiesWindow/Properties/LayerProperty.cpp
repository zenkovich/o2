#include "stdafx.h"
#include "LayerProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/DropDown.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{

	LayerProperty::LayerProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("enum property");

		mDropDown = mPropertyWidget->GetChildByType<UIDropDown>();
		if (!mDropDown)
			mDropDown = dynamic_cast<UIDropDown*>(mPropertyWidget);

		mDropDown->onSelectedText = THIS_FUNC(SelectLayer);
		mDropDown->SetState("undefined", true);

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	LayerProperty::~LayerProperty()
	{
		delete mPropertyWidget;
	}

	void LayerProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		Refresh();
	}

	void LayerProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = GetProxy<SceneLayer*>(mValuesProxies[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			if (newCommonValue != GetProxy<SceneLayer*>(mValuesProxies[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue();
		}
		else if (newCommonValue != lastCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	void LayerProperty::Revert()
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				SetProxy<SceneLayer*>(ptr.first, GetProxy<SceneLayer*>(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* LayerProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	SceneLayer* LayerProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool LayerProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void LayerProperty::SetValue(SceneLayer* value)
	{
		for (auto ptr : mValuesProxies)
			SetProxy<SceneLayer*>(ptr.first, value);

		SetCommonValue(value);
	}

	void LayerProperty::SetUnknownValue()
	{
		mCommonValue = nullptr;
		mValuesDifferent = true;

		mUpdatingValue = true;
		mDropDown->SelectItemAt(-1);
		mUpdatingValue = false;

		mDropDown->SetState("undefined", true);

		OnChanged();
	}

	const Type* LayerProperty::GetFieldType() const
	{
		return &TypeOf(SceneLayer*);
	}

	void LayerProperty::SetCommonValue(SceneLayer* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		mUpdatingValue = true;
		if (!mCommonValue)
		{
			mDropDown->SelectItemAt(-1);
			mDropDown->SetState("undefined", false);
		}
		else
		{
			mDropDown->value = mCommonValue->name;
			mDropDown->SetState("undefined", false);
		}
		mUpdatingValue = false;

		OnChanged();
	}

	void LayerProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy<SceneLayer*>(ptr.first), GetProxy<SceneLayer*>(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void LayerProperty::UpdateLayersList()
	{
		auto layers = o2Scene.GetLayers();
		auto dropdownLayers = mDropDown->GetAllItemsText();
		for (auto itemName : dropdownLayers)
		{
			if (!layers.ContainsPred([&](SceneLayer* x) { return x->name == itemName; }))
				mDropDown->RemoveItem(itemName);
		}

		for (auto layer : layers)
		{
			if (!dropdownLayers.Contains(layer->name))
				mDropDown->AddItem(layer->name);
		}
	}

	void LayerProperty::SelectLayer(const WString& name)
	{
		if (mUpdatingValue)
			return;

		SetValueByUser(o2Scene.GetLayer(name));
	}

	void LayerProperty::SetValueByUser(SceneLayer* value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void LayerProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void LayerProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataNode());
			data.Last() = GetProxy<SceneLayer*>(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::LayerProperty);
