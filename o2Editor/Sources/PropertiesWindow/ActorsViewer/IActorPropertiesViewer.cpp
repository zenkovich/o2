#include "stdafx.h"
#include "IActorPropertiesViewer.h"

#include "Render/Sprite.h"
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
	IActorPropertiesViewer::IActorPropertiesViewer()
	{
		mDataView = mnew UIVerticalLayout();
		mDataView->spacing = 0.0f;
		mDataView->border = BorderF(0, 0, 0, 0);
		mDataView->expandHeight = false;
		mDataView->expandWidth = true;
		mDataView->fitByChildren = true;
		mDataView->baseCorner = BaseCorner::RightTop;
		*mDataView->layout = UIWidgetLayout::BothStretch();

		mSpoiler = mnew UISpoiler();
		mSpoiler->name = "spoiler";
		mSpoiler->SetExpanded(true);

		UIWidget* spoilerHead = mnew UIWidget();
		spoilerHead->name = "head";
		*spoilerHead->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 18);
		spoilerHead->layout->minHeight = 18;
		spoilerHead->AddLayer("back", mnew Sprite("ui/UI2_component_head.png"),
								Layout::BothStretch(-2, -2, -2, -2));
		mDataView->AddChild(spoilerHead);

		auto separatorImg = o2UI.CreateImage("ui/UI_Separator.png");
		*separatorImg->layout = UIWidgetLayout::HorStretch(VerAlign::Top, -1, -1, 5, -2);
		spoilerHead->AddChild(separatorImg);

		mExpandBtn = o2UI.CreateWidget<UIButton>("expand");
		*mExpandBtn->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(15, 15), Vec2F(0, 0));
		mExpandBtn->onClick = [&]() { if (mSpoiler->IsExpanded()) Collapse(); else Expand(); };
		spoilerHead->AddChild(mExpandBtn);
		mExpandBtn->SetState("expanded", true);

		auto icon = o2UI.CreateImage("ui/UI2_big_file_icon.png");
		*icon->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(12, 0));
		spoilerHead->AddChild(icon);

		mNameCaption = o2UI.CreateLabel("Unknown");
		*mNameCaption->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, 30, 30, 15, 0);
		mNameCaption->horAlign = HorAlign::Left;
		spoilerHead->AddChild(mNameCaption);

		mSaveBtn = o2UI.CreateWidget<UIButton>("component save");
		*mSaveBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(-15, 0));
		spoilerHead->AddChild(mSaveBtn);

		mOptionsBtn = o2UI.CreateWidget<UIButton>("component options");
		*mOptionsBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0));
		spoilerHead->AddChild(mOptionsBtn);

		mPropertiesLayout = mnew UIVerticalLayout();
		mPropertiesLayout->name = "properties";
		mPropertiesLayout->spacing = 5.0f;
		mPropertiesLayout->border = BorderF(5, 5, 5, 5);
		mPropertiesLayout->expandHeight = false;
		mPropertiesLayout->expandWidth = true;
		mPropertiesLayout->fitByChildren = true;
		mPropertiesLayout->baseCorner = BaseCorner::Top;
		*mPropertiesLayout->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 0);
		mSpoiler->AddChild(mPropertiesLayout);

		mDataView->AddChild(mSpoiler);
	}

	IActorPropertiesViewer::~IActorPropertiesViewer()
	{
		if (mDataView)
			delete mDataView;
	}

	UIWidget* IActorPropertiesViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorPropertiesViewer::Expand()
	{
		mExpandBtn->SetState("expanded", true);
		mSpoiler->Expand();
	}

	void IActorPropertiesViewer::Collapse()
	{
		mExpandBtn->SetState("expanded", false);
		mSpoiler->Collapse();
	}

	void IActorPropertiesViewer::Refresh()
	{}
}

DECLARE_CLASS(Editor::IActorPropertiesViewer);
