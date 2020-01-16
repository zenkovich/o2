#include "o2Editor/stdafx.h"
#include "WidgetLayerViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/DefaultWidgetLayerHeadViewer.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/DefaultWidgetLayerLayoutViewer.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/DefaultWidgetLayerPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerHeadViewer.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerLayoutViewer.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerPropertiesViewer.h"

namespace Editor
{
	WidgetLayerViewer::WidgetLayerViewer()
	{
		PushEditorScopeOnStack scope;

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
		*mViewersLayout->layout = WidgetLayout::BothStretch();
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
		return &TypeOf(WidgetLayer);
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
		if (mTargetLayers.IsEmpty())
			return;

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
		PushEditorScopeOnStack scope;

		mTargetLayers = targets.Select<WidgetLayer*>([](auto x) { return dynamic_cast<WidgetLayer*>(x); });

		// rebuild
		mHeaderViewer->SetTargetLayers(mTargetLayers);
		mLayoutViewer->SetTargetLayers(mTargetLayers);
		mPropertiesViewer->SetTargetLayers(mTargetLayers);
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
