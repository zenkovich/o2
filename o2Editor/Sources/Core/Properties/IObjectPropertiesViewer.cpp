#include "stdafx.h"
#include "IObjectPropertiesViewer.h"

namespace Editor
{
	UIWidget* IObjectPropertiesViewer::InitializeControls(const String& path, const OnChangeCompletedFunc& onChangeCompleted,
														  const OnChangedFunc& onChanged)
	{
		return mViewWidget;
	}

	void IObjectPropertiesViewer::Refresh(const TargetsVec& targetObjets)
	{}

	const Type* IObjectPropertiesViewer::GetViewingObjectType() const
	{
		return nullptr;
	}

	UIWidget* IObjectPropertiesViewer::GetViewWidget() const
	{
		return mViewWidget;
	}

}

DECLARE_CLASS(Editor::IObjectPropertiesViewer);
