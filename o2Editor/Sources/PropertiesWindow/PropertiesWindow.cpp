#include "stdafx.h"
#include "PropertiesWindow.h"

#include "Core/Properties/Properties.h"
#include "IPropertiesViewer.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/WidgetLayout.h"


DECLARE_SINGLETON(Editor::PropertiesWindow);

namespace Editor
{
	PropertiesWindow::PropertiesWindow():
		mCurrentViewer(nullptr)
	{
		InitializeWindow();
		InitializeViewers();
	}

	PropertiesWindow::~PropertiesWindow()
	{
		for (auto viewer : mViewers)
			delete viewer;
	}

	void PropertiesWindow::ResetTargets()
	{
		SetTarget(nullptr);
	}

	void PropertiesWindow::InitializeWindow()
	{
		mWindow->caption = "Properties";
		mWindow->name = "properties window";
		mWindow->SetIcon(mnew Sprite("ui/UI2_gear_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 15));

		InitializeWindowContext();
	}

	void PropertiesWindow::InitializeWindowContext()
	{
		auto context = mWindow->GetOptionsMenu();
		if (!context)
		{
			o2Debug.LogError("Failed to initialize properties window context menu: not found menu on window");
			return;
		}

		context->AddItem(UIContextMenu::Item::Separator());
		context->AddItem(UIContextMenu::Item("Private visible", false, THIS_FUNC(OnPrivateFieldsVisibleChanged)));
	}

	void PropertiesWindow::InitializeViewers()
	{
		auto viewersTypes = TypeOf(IPropertiesViewer).GetDerivedTypes();

		for (auto type : viewersTypes)
			mViewers.Add((IPropertiesViewer*)type->CreateSample());
	}

	void PropertiesWindow::OnPrivateFieldsVisibleChanged(bool visible)
	{
		o2EditorProperties.SetPrivateFieldsVisible(visible);

		if (mCurrentViewer)
			mCurrentViewer->SetTargets(mTargets);
	}

	void PropertiesWindow::OnPropertyChanged(IPropertyField* field)
	{
		mTargetsChanged = true;
	}

	void PropertiesWindow::SetTarget(IObject* target)
	{
		if (target == nullptr)
			SetTargets(Vector<IObject*>());
		else
			SetTargets({ target });
	}

	void PropertiesWindow::SetTargets(const Vector<IObject*> targets, const Function<void()>& targetsChangedDelegate /*= Function<void()>()*/)
	{
		if (mTargetsChanged)
			mOnTargetsChangedDelegate();

		IPropertiesViewer* objectViewer = nullptr;
		if (!targets.IsEmpty())
		{
			auto type = &targets[0]->GetType();
			objectViewer = mViewers.FindMatch([&](auto x) { return type->IsBasedOn(*x->GetViewingObjectType()); });
		}

		if (objectViewer != mCurrentViewer)
		{
			if (mCurrentViewer)
			{
				mCurrentViewer->mContentWidget->Hide(true);
				mCurrentViewer->OnDisabled();
			}

			mCurrentViewer = objectViewer;

			if (mCurrentViewer)
			{
				mCurrentViewer->mContentWidget->SetParent(mWindow);
				*mCurrentViewer->mContentWidget->layout = WidgetLayout::BothStretch();
				mCurrentViewer->mContentWidget->Show(true);
				mCurrentViewer->OnEnabled();
			}
		}

		mTargets = targets;

		if (mCurrentViewer)
			mCurrentViewer->SetTargets(mTargets);

		mOnTargetsChangedDelegate = targetsChangedDelegate;
		mTargetsChanged = false;
	}

	Vector<IObject*> PropertiesWindow::GetTargets() const
	{
		return mTargets;
	}

	void PropertiesWindow::Update(float dt)
	{
		if (mCurrentViewer)
			mCurrentViewer->Update(dt);
	}

	void PropertiesWindow::Draw()
	{
		if (mCurrentViewer)
			mCurrentViewer->Draw();
	}

	bool PropertiesWindow::IsTargetsChanged() const
	{
		return mTargetsChanged;
	}
}

DECLARE_CLASS(Editor::PropertiesWindow);
