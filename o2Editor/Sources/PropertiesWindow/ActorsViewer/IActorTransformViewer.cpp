#include "stdafx.h"
#include "IActorTransformViewer.h"

#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "UI/Button.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	IActorTransformViewer::IActorTransformViewer()
	{
		mDataView = mnew UIVerticalLayout();
		mDataView->spacing = 0.0f;
		mDataView->border = RectF(0, 0, 0, 0);
		mDataView->expandHeight = false;
		mDataView->expandWidth = true;
		mDataView->fitByChildren = true;
		mDataView->baseCorner = BaseCorner::RightTop;
		*mDataView->layout = UIWidgetLayout::BothStretch();

		mSpoiler = mnew UISpoiler();
		mSpoiler->name = "spoiler";
		mSpoiler->SetExpanded(true);

		UIWidget* componentHead = mnew UIWidget();
		componentHead->name = "head";
		*componentHead->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 18);
		componentHead->layout->minHeight = 18;
		componentHead->AddLayer("back", mnew Sprite("ui/UI2_component_head.png"),
								Layout::BothStretch(-2, -2, -2, -2));
		mDataView->AddChild(componentHead);

		auto separatorImg = o2UI.CreateImage("ui/UI_Separator.png");
		*separatorImg->layout = UIWidgetLayout::HorStretch(VerAlign::Top, -1, -1, 5, -2);
		componentHead->AddChild(separatorImg);

		mExpandBtn = o2UI.CreateWidget<UIButton>("expand");
		*mExpandBtn->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(15, 15), Vec2F(0, 0));
		mExpandBtn->onClick = [&]() { if (mSpoiler->IsExpanded()) Collapse(); else Expand(); };
		componentHead->AddChild(mExpandBtn);
		mExpandBtn->SetState("expanded", true);

		auto icon = o2UI.CreateImage("ui/UI2_transform_icon.png");
		*icon->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(12, 0));
		componentHead->AddChild(icon);

		mNameCaption = o2UI.CreateLabel("Unknown");
		*mNameCaption->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, 30, 30, 15, 0);
		mNameCaption->horAlign = HorAlign::Left;
		componentHead->AddChild(mNameCaption);

		mSaveBtn = o2UI.CreateWidget<UIButton>("component save");
		*mSaveBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(-15, 0));
		componentHead->AddChild(mSaveBtn);

		mOptionsBtn = o2UI.CreateWidget<UIButton>("component options");
		*mOptionsBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0));
		componentHead->AddChild(mOptionsBtn);

		mPropertiesLayout = mnew UIWidget();
		mPropertiesLayout->name = "properties";
		mSpoiler->AddChild(mPropertiesLayout);

		mDataView->AddChild(mSpoiler);
	}

	IActorTransformViewer::~IActorTransformViewer()
	{}

	UIWidget* IActorTransformViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorTransformViewer::Expand()
	{
		mExpandBtn->SetState("expanded", true);
		mSpoiler->Expand();
	}

	void IActorTransformViewer::Collapse()
	{
		mExpandBtn->SetState("expanded", false);
		mSpoiler->Collapse();
	}

	void IActorTransformViewer::Refresh()
	{}

}

DECLARE_CLASS(Editor::IActorTransformViewer);
