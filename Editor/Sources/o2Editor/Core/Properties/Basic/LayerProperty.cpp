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
		return *this;
	}

	void LayerProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<SceneLayer*>::CopyData(otherActor);
		InitializeControls();
	}

	void LayerProperty::InitializeControls()
	{
		mCommonValue = nullptr;

		mDropDown = FindChildByType<DropDown>();
		if (mDropDown)
		{
			mDropDown->onSelectedText = THIS_FUNC(SelectLayer);
			mDropDown->SetState("undefined", true);
		}
	}

	void LayerProperty::UpdateValueView()
	{
		mUpdatingValue = true;

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
				mDropDown->value = mCommonValue->name;
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
			if (!layers.Contains([&](SceneLayer* x) { return x->name == itemName; }))
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
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::SceneLayer*>);

DECLARE_CLASS(Editor::LayerProperty);
