#include "LayerProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Render/Sprite.h"
#include "UI/DropDown.h"
#include "UI/UIManager.h"

namespace Editor
{

	LayerProperty::LayerProperty()
	{
		InitializeWidget();
	}

	LayerProperty::LayerProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	LayerProperty::~LayerProperty()
	{
		delete mDropDown;
	}

	void LayerProperty::InitializeWidget()
	{
		mDropDown = o2UI.CreateWidget<UIDropDown>("editor property");
		mDropDown->onSelectedText = Function<void(const WString&)>(this, &LayerProperty::SelectLayer);
	}

	void LayerProperty::Setup(const Vector<void*>& targets, bool isProperty)
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

		mDropDown->RemoveAllItems();

		for (auto layer : o2Scene.GetLayers())
			mDropDown->AddItem(layer->name);

		Update();
	}

	void LayerProperty::Update()
	{
		if (mValuesPointers.IsEmpty())
			return;

		mCommonValue = mGetFunc(mValuesPointers[0]);
		mValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (mCommonValue != mGetFunc(mValuesPointers[i]))
			{
				mValuesDifferent = true;
				break;
			}
		}

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

	UIWidget* LayerProperty::GetWidget() const
	{
		return mDropDown;
	}

	Scene::Layer* LayerProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool LayerProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* LayerProperty::GetFieldType() const
	{
		return &TypeOf(Scene::Layer*);
	}

	void LayerProperty::SelectLayer(const WString& name)
	{
		if (mUpdatingValue)
			return;

		mCommonValue = o2Scene.GetLayer(name);
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		Update();
	}

}