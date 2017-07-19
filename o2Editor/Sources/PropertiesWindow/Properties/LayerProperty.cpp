#include "LayerProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/DropDown.h"
#include "UI/UIManager.h"

namespace Editor
{

	LayerProperty::LayerProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("enum property");

		mDropDown = mPropertyWidget->FindChild<UIDropDown>();
		if (!mDropDown)
			mDropDown = dynamic_cast<UIDropDown*>(mPropertyWidget);

		mDropDown->onSelectedText = Func(this, &LayerProperty::SelectLayer);
		mDropDown->SetState("undefined", true);

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = Func(this, &LayerProperty::Revert);
	}

	LayerProperty::~LayerProperty()
	{
		delete mPropertyWidget;
	}

	void LayerProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Scene::Layer* value) { *((Property<Scene::Layer*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Scene::Layer*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Scene::Layer* value) { *((Scene::Layer**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Scene::Layer**)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void LayerProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
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
	}

	void LayerProperty::Revert()
	{
		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				mAssignFunc(ptr.first, mGetFunc(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* LayerProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Scene::Layer* LayerProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool LayerProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void LayerProperty::SetValue(Scene::Layer* value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

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
		return &TypeOf(Scene::Layer*);
	}

	void LayerProperty::SetCommonValue(Scene::Layer* value)
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

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second && !Math::Equals(mGetFunc(ptr.first), mGetFunc(ptr.second)))
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
			if (!layers.ContainsPred([&](Scene::Layer* x) { return x->name == itemName; }))
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

	void LayerProperty::SetValueByUser(Scene::Layer* value)
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
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::LayerProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mDropDown);
	PROTECTED_FIELD(mUpdatingValue);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Scene::Layer*, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, Scene::Layer*);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, Scene::Layer*);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, UpdateLayersList);
	PROTECTED_FUNCTION(void, SelectLayer, const WString&);
	PROTECTED_FUNCTION(void, SetValueByUser, Scene::Layer*);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
