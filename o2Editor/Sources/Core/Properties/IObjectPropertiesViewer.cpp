#include "stdafx.h"
#include "IObjectPropertiesViewer.h"

namespace Editor
{
	void IObjectPropertiesViewer::Refresh(const TargetsVec& targetObjets, const String& path, 
										  const OnChangeCompletedFunc& onChangeCompleted, const OnChangedFunc& onChanged)
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
