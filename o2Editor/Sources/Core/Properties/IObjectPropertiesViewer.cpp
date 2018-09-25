#include "stdafx.h"
#include "IObjectPropertiesViewer.h"

namespace Editor
{
	void IObjectPropertiesViewer::InitializeControls(UIVerticalLayout* layout, const Type& objectType,
													 const OnChangeCompletedFunc& onChangeCompleted,
													 const OnChangedFunc& onChanged)
	{
		mSpecializedType = &objectType;
	}

	void IObjectPropertiesViewer::Refresh(const TargetsVec& targetObjets)
	{}

	const Type& IObjectPropertiesViewer::GetSpecializedObjectType() const
	{
		return *mSpecializedType;
	}

	const Type* IObjectPropertiesViewer::GetViewingObjectType() const
	{
		return nullptr;
	}

}

DECLARE_CLASS(Editor::IObjectPropertiesViewer);
