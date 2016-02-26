#include "PropertiesWindow.h"

#include "PropertiesWindow/IObjectPropertiesViewer.h"
#include "UI/Widget.h"

DECLARE_SINGLETON(PropertiesWindow);

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

void PropertiesWindow::InitializeWindow()
{
	mWindow->caption = "Properties";
	mWindow->name = "properties window";
	if (auto iconLayer = mWindow->GetLayer("icon"))
	{
		*((Sprite*)iconLayer->drawable) = Sprite("ui/UI2_gear_icon.png");
		iconLayer->layout = Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2));
	}

	mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
	mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));
}

void PropertiesWindow::InitializeViewers()
{
	auto viewersTypes = IObjectPropertiesViewer::type.DerivedTypes();

	for (auto type : viewersTypes)
		mViewers.Add((IObjectPropertiesViewer*)type->CreateSample());
}

void PropertiesWindow::SetTarget(IObject* target)
{
	SetTargets({ target });
}

void PropertiesWindow::SetTargets(const Vector<IObject*> targets)
{
	IObjectPropertiesViewer* objectViewer = nullptr;
	if (!targets.IsEmpty())
	{
		auto type = &targets[0]->GetType();
		objectViewer = mViewers.FindMatch([&](auto x) { return x->GetViewingObjectType() == type; });
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
			mCurrentViewer->mContentWidget->layout = UIWidgetLayout::BothStretch();
			mCurrentViewer->mContentWidget->Show(true);
			mCurrentViewer->OnEnabled();
		}
	}

	mTargets = targets;

	if (mCurrentViewer)
		mCurrentViewer->SetTargets(mTargets);
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
