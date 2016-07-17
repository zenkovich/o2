#include "ComponentProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Assets/Assets.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"

namespace Editor
{
	ComponentProperty::ComponentProperty()
	{
		InitializeWidget();
		mComponentType = &TypeOf(Component);
	}

	ComponentProperty::ComponentProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		mComponentType = &TypeOf(Component);
		Setup(targets, isProperty);
	}

	ComponentProperty::~ComponentProperty()
	{
		SetEventHandleDrawable(nullptr);
		delete mBox;
	}

	void ComponentProperty::InitializeWidget()
	{
		mBox = mnew UIWidget();

		mBox->SetFocusable(true);

		auto backLayer = mBox->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto selectLayer = mBox->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = mBox->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		mBox->AddState("focused", Animation::EaseInOut(mBox, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		mBox->AddState("select", Animation::EaseInOut(mBox, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		mNameText = mnew Text("stdFont.ttf");
		mNameText->text = "--";
		mNameText->horAlign = HorAlign::Left;
		mNameText->verAlign = VerAlign::Middle;
		mNameText->dotsEngings = true;
		mBox->AddLayer("caption", mNameText, Layout::BothStretch(2, 2, 2, 2));

		auto linkBtn = o2UI.CreateWidget<UIButton>("asset link");
		linkBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		mBox->AddChild(linkBtn);

		SetEventHandleDrawable(mBox);
	}

	void ComponentProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Component* value) { *((Property<Component*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Component*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Component* value) { *((Component**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Component**)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void ComponentProperty::Update()
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

		if (mValuesDifferent)
		{
			mNameText->text = "--";
			mBox->layer["caption"]->transparency = 1.0f;
		}
		else
		{
			if (!mCommonValue)
			{
				mNameText->text = "Null:" + mComponentType->Name();
				mBox->layer["caption"]->transparency = 0.5f;
			}
			else
			{
				mNameText->text = mCommonValue->GetOwnerActor()->GetName();
				mBox->layer["caption"]->transparency = 1.0f;
			}
		}
	}

	UIWidget* ComponentProperty::GetWidget() const
	{
		return mBox;
	}

	Component* ComponentProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ComponentProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* ComponentProperty::GetFieldType() const
	{
		return &TypeOf(Component*);
	}

	void ComponentProperty::SpecializeType(const Type* type)
	{
		mComponentType = type;
	}

	const Type* ComponentProperty::GetSpecializedType() const
	{
		return mComponentType;
	}

	void ComponentProperty::SetValue(Component* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		Update();
	}

	void ComponentProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("select", true);
	}

	void ComponentProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("select", false);
	}

	void ComponentProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);

		if (mCommonValue)
		{
			auto ownerActor = mCommonValue->GetOwnerActor();

			if (ownerActor)
			{
				if (ownerActor->IsAsset())
				{
					o2EditorAssets.ShowAssetIcon(o2Assets.GetAssetPath(ownerActor->GetAssetId()));
				}
				else if (ownerActor->IsOnScene())
				{
					o2EditorTree.HightlightActorsTreeNode(ownerActor);
					o2EditorSceneScreen.SelectActor(ownerActor, false);
				}
			}
		}
	}

	void ComponentProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox->IsFocused() && key == VK_DELETE || key == VK_BACK)
			SetValue(nullptr);
	}

}