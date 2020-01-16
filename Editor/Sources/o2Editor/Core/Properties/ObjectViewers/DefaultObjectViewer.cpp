#include "o2Editor/stdafx.h"
#include "DefaultObjectViewer.h"

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"

namespace Editor
{
	DefaultObjectViewer::DefaultObjectViewer()
	{}

	void DefaultObjectViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		PushEditorScopeOnStack scope;

		if (targetObjets.IsEmpty())
			return;

		const Type* objectsType = &(targetObjets[0].first)->GetType();

		if (mRealObjectType != objectsType || mBuiltWithHiddenProperties != o2EditorProperties.IsPrivateFieldsVisible())
		{
			mRealObjectType = objectsType;

			if (mRealObjectType)
				o2EditorProperties.FreeProperties(mPropertiesContext);

			if (mRealObjectType)
			{
				o2EditorProperties.BuildObjectProperties(dynamic_cast<VerticalLayout*>(mLayout), mRealObjectType,
														 mPropertiesContext, "", mOnChildFieldChangeCompleted, onChanged);

				mBuiltWithHiddenProperties = o2EditorProperties.IsPrivateFieldsVisible();
			}
		}

		mPropertiesContext.Set(targetObjets);
	}

	const Type* DefaultObjectViewer::GetViewingObjectType() const
	{
		return mRealObjectType;
	}
}

DECLARE_CLASS(Editor::DefaultObjectViewer);
