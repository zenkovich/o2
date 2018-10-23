#include "stdafx.h"
#include "DefaultObjectViewer.h"

#include "Core/Properties/Properties.h"
#include "UI/VerticalLayout.h"

namespace Editor
{
	DefaultObjectViewer::DefaultObjectViewer()
	{
		auto layout = mnew UIVerticalLayout();
		layout->spacing = 5;
		layout->borderTop = 5;
		layout->fitByChildren = true;

		mViewWidget = layout;
	}

	void DefaultObjectViewer::Refresh(const TargetsVec& targetObjets)
	{
		if (targetObjets.IsEmpty())
			return;

		const Type* objectsType = &(targetObjets[0].first)->GetType();

		if (mRealObjectType == objectsType)
			return;

		mRealObjectType = objectsType;

		if (mRealObjectType)
			o2EditorProperties.FreeProperties(mFieldProperties);

		if (mRealObjectType)
		{
			o2EditorProperties.BuildObjectProperties(dynamic_cast<UIVerticalLayout*>(mViewWidget), mRealObjectType, 
													 mFieldProperties, "", mOnChildFieldChangeCompleted, onChanged);

			mFieldProperties.Set(targetObjets);
		}
	}

	const Type* DefaultObjectViewer::GetViewingObjectType() const
	{
		return &TypeOf(IObject);
	}
}

DECLARE_CLASS(Editor::DefaultObjectViewer);
