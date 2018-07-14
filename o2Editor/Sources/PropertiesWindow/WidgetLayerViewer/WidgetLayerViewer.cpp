#include "stdafx.h"
#include "WidgetLayerViewer.h"

#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerHeadViewer.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerLayoutViewer.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerPropertiesViewer.h"

namespace Editor
{

	WidgetLayerViewer::WidgetLayerViewer()
	{

	}

	WidgetLayerViewer::~WidgetLayerViewer()
	{

	}

	const Type* WidgetLayerViewer::GetViewingObjectType() const
	{
		return &TypeOf(UIWidgetLayer);
	}

	void WidgetLayerViewer::SetHeaderViewer(IWidgetLayerHeaderViewer* viewer)
	{

	}

	void WidgetLayerViewer::SetLayoutViewer(IWidgetLayerLayoutViewer* viewer)
	{

	}

	void WidgetLayerViewer::SetActorPropertiesViewer(IWidgetLayerPropertiesViewer* viewer)
	{

	}

	void WidgetLayerViewer::Refresh()
	{

	}

	void WidgetLayerViewer::OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects)
	{

	}

	void WidgetLayerViewer::SetTargets(const Vector<IObject*> targets)
	{

	}

	void WidgetLayerViewer::OnEnabled()
	{

	}

	void WidgetLayerViewer::OnDisabled()
	{

	}

	void WidgetLayerViewer::Update(float dt)
	{

	}

	void WidgetLayerViewer::Draw()
	{

	}
}

DECLARE_CLASS(Editor::WidgetLayerViewer);
