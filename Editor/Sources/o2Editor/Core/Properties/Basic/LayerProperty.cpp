#include "o2Editor/stdafx.h"
#include "LayerProperty.h"

#include "o2/Scene/UI/Widgets/DropDown.h"


namespace Editor
{
	LayerProperty::LayerProperty()
	{
		mCommonValue = nullptr;
	}

	LayerProperty::LayerProperty(const LayerProperty& other) :
		TPropertyField<SceneLayer*>(other)
	{
		InitializeControls();
	}

	LayerProperty& LayerProperty::operator=(const LayerProperty& other)
	{
		TPropertyField<SceneLayer*>::operator=(other);
		InitializeControls();
		return *this;
	}

	void LayerProperty::InitializeControls()
	{
		mCommonValue = nullptr;

		mDropDown = FindChildByType<DropDown>();
		if (mDropDown)
		{
			mDropDown->onSelectedText = THIS_FUNC(SelectLayer);
			mDropDown->onBeforeExpand = THIS_FUNC(UpdateLayersList);
			mDropDown->SetState("undefined", true);
		}
	}

	void LayerProperty::UpdateValueView()
	{
		mUpdatingValue = true;

		UpdateLayersList();

		if (mValuesDifferent)
		{
			mDropDown->SelectItemAt(-1);
			mDropDown->SetState("undefined", true);
		}
		else
		{
			if (!mCommonValue)
			{
				mDropDown->SelectItemAt(-1);
				mDropDown->SetState("undefined", false);
			}
			else
			{
				mDropDown->value = mCommonValue->GetName();
				mDropDown->SetState("undefined", false);
			}
		}

		mUpdatingValue = false;
	}

	void LayerProperty::UpdateLayersList()
	{
		auto layers = o2Scene.GetLayers();
		auto dropdownLayers = mDropDown->GetAllItemsText();
		for (auto itemName : dropdownLayers)
		{
			if (!layers.Contains([&](SceneLayer* x) { return x->GetName() == itemName; }))
				mDropDown->RemoveItem(itemName);
		}

		for (auto layer : layers)
		{
			if (!dropdownLayers.Contains(layer->GetName()))
				mDropDown->AddItem(layer->GetName());
		}
	}

	void LayerProperty::SelectLayer(const WString& name)
	{
		if (mUpdatingValue)
			return;

		SetValueByUser(o2Scene.GetLayer(name));
	}

	bool LayerProperty::IsAlwaysRefresh() const
	{
		if (mCommonValue && !mValuesDifferent && mCommonValue->GetName() != mDropDown->GetSelectedItemText())
			return true;

		return false;
	}

}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::SceneLayer*>);

DECLARE_CLASS(Editor::LayerProperty);
