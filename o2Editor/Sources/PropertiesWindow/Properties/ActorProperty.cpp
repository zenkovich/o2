#include "ActorProperty.h"

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
	ActorProperty::ActorProperty()
	{
		InitializeWidget();
	}

	ActorProperty::ActorProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	ActorProperty::~ActorProperty()
	{
		SetEventHandleDrawable(nullptr);
		delete mBox;
	}

	void ActorProperty::InitializeWidget()
	{
		mBox = mnew UIWidget();

		mBox->SetSelectable(true);

		auto backLayer = mBox->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto selectLayer = mBox->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = mBox->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		mBox->AddState("selected", Animation::EaseInOut(mBox, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
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

	void ActorProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Actor* value) { *((Property<Actor*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Actor*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Actor* value) { *((Actor**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Actor**)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void ActorProperty::Update()
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
				mNameText->text = "Null:Actor";
				mBox->layer["caption"]->transparency = 0.5f;
			}
			else
			{
				mNameText->text = mCommonValue->GetName();
				mBox->layer["caption"]->transparency = 1.0f;
			}
		}
	}

	UIWidget* ActorProperty::GetWidget() const
	{
		return mBox;
	}

	Actor* ActorProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ActorProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* ActorProperty::GetFieldType() const
	{
		return &TypeOf(Actor);
	}

	void ActorProperty::SetValue(Actor* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		Update();
	}

	void ActorProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("select", true);
	}

	void ActorProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("select", false);
	}

	void ActorProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.SelectWidget(mBox);

		if (mCommonValue)
		{
			if (mCommonValue->IsAsset())
			{				
				o2EditorAssets.ShowAssetIcon(o2Assets.GetAssetPath(mCommonValue->GetAssetId()));
			}
			else if (mCommonValue->IsOnScene())
			{
				o2EditorTree.ExpandActorsTreeNode(mCommonValue);
				o2EditorSceneScreen.SelectActor(mCommonValue, false);
			}
		}
	}

	void ActorProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox->IsSelected() && key == VK_DELETE || key == VK_BACK)
			SetValue(nullptr);
	}

}