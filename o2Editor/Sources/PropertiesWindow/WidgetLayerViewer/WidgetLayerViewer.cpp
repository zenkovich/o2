#include "stdafx.h"
#include "WidgetLayerViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "DefaultWidgetLayerHeadViewer.h"
#include "DefaultWidgetLayerLayoutViewer.h"
#include "DefaultWidgetLayerPropertiesViewer.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerHeadViewer.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerLayoutViewer.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerPropertiesViewer.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	WidgetLayerViewer::WidgetLayerViewer()
	{
		PushScopeEnterOnStack scope;

		mHeaderViewer = mnew DefaultWidgetLayerHeaderViewer();
		mLayoutViewer = mnew DefaultWidgetLayerLayoutViewer();
		mPropertiesViewer = mnew DefaultWidgetLayerPropertiesViewer();

		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch());
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->name = "widget layers scroll area";
		mContentWidget = scrollArea;

		mViewersLayout = o2UI.CreateVerLayout();
		mViewersLayout->name = "viewers layout";
		mViewersLayout->spacing = 0.0f;
		mViewersLayout->border = BorderF();
		mViewersLayout->expandHeight = false;
		mViewersLayout->expandWidth = true;
		mViewersLayout->fitByChildren = true;
		mViewersLayout->baseCorner = BaseCorner::Top;
		*mViewersLayout->layout = UIWidgetLayout::BothStretch();
		mContentWidget->AddChild(mViewersLayout);

		mViewersLayout->AddChild(mHeaderViewer->GetWidget());
		mViewersLayout->AddChild(mLayoutViewer->GetWidget());
		mViewersLayout->AddChild(mPropertiesViewer->GetWidget());

		o2Scene.onObjectsChanged += THIS_FUNC(OnSceneObjectsChanged);
	}

	WidgetLayerViewer::~WidgetLayerViewer()
	{
		o2Scene.onObjectsChanged -= THIS_FUNC(OnSceneObjectsChanged);

		if (mPropertiesViewer)
			delete mPropertiesViewer;

		if (mHeaderViewer)
			delete mHeaderViewer;

		if (mLayoutViewer)
			delete mLayoutViewer;
	}

	const Type* WidgetLayerViewer::GetViewingObjectType() const
	{
		return &TypeOf(UIWidgetLayer);
	}

	void WidgetLayerViewer::SetHeaderViewer(IWidgetLayerHeaderViewer* viewer)
	{
		delete mHeaderViewer;
		mHeaderViewer = viewer;
	}

	void WidgetLayerViewer::SetLayoutViewer(IWidgetLayerLayoutViewer* viewer)
	{
		delete mLayoutViewer;
		mLayoutViewer = viewer;
	}

	void WidgetLayerViewer::SetActorPropertiesViewer(IWidgetLayerPropertiesViewer* viewer)
	{
		delete mPropertiesViewer;
		mPropertiesViewer = viewer;
	}

	void WidgetLayerViewer::Refresh()
	{
		mHeaderViewer->Refresh();
		mLayoutViewer->Refresh();
		mPropertiesViewer->Refresh();
	}

	void WidgetLayerViewer::OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects)
	{
		Refresh();
	}

	void WidgetLayerViewer::SetTargets(const Vector<IObject*> targets)
	{
		PushScopeEnterOnStack scope;

		mTargetLayers = targets.Select<UIWidgetLayer*>([](auto x) { return dynamic_cast<UIWidgetLayer*>(x); });

		// rebuild
		mHeaderViewer->SetTargetLayers(mTargetLayers);
		mLayoutViewer->SetTargetLayers(mTargetLayers);
		mPropertiesViewer->SetTargetLayers(mTargetLayers);
		mPropertiesViewer->Rebuild();
	}

	void WidgetLayerViewer::OnEnabled()
	{}

	void WidgetLayerViewer::OnDisabled()
	{
		mTargetLayers.Clear();
	}

	void WidgetLayerViewer::Update(float dt)
	{}

	void WidgetLayerViewer::Draw()
	{}
}

DECLARE_CLASS(Editor::WidgetLayerViewer);
