#include "stdafx.h"
#include "DefaultObjectViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widgets/VerticalLayout.h"

namespace Editor
{
	DefaultObjectViewer::DefaultObjectViewer()
	{
		PushEditorScopeOnStack scope;

		auto layout = mnew VerticalLayout();
		layout->spacing = 5;
		layout->borderTop = 5;
		layout->expandHeight = false;
		layout->expandWidth = true;
		layout->fitByChildren = true;

		mViewWidget = layout;
	}

	void DefaultObjectViewer::Refresh(const TargetsVec& targetObjets)
	{
		PushEditorScopeOnStack scope;

		if (targetObjets.IsEmpty())
			return;

		const Type* objectsType = &(targetObjets[0].first)->GetType();

		if (mRealObjectType == objectsType && mBuiltWithHiddenProperties == o2EditorProperties.IsPrivateFieldsVisible())
			return;

		mRealObjectType = objectsType;

		if (mRealObjectType)
			o2EditorProperties.FreeProperties(mFieldProperties);

		if (mRealObjectType)
		{
			o2EditorProperties.BuildObjectProperties(dynamic_cast<VerticalLayout*>(mViewWidget), mRealObjectType, 
													 mFieldProperties, "", mOnChildFieldChangeCompleted, onChanged);

			mFieldProperties.Set(targetObjets);
			mBuiltWithHiddenProperties = o2EditorProperties.IsPrivateFieldsVisible();

			for (auto kv : mFieldProperties.mProperties)
			{
			}
		}
	}

	const Type* DefaultObjectViewer::GetViewingObjectType() const
	{
		return &TypeOf(IObject);
	}
}

DECLARE_CLASS(Editor::DefaultObjectViewer);
