#include "o2Editor/stdafx.h"
#include "DefaultObjectPropertiesViewer.h"

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"

namespace Editor
{
	void DefaultObjectPropertiesViewer::CheckBuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		IObjectPropertiesViewer::CheckBuildProperties(targetObjets);

		if (targetObjets.IsEmpty())
			return;

		PushEditorScopeOnStack scope;

		const Type* objectsType = &(targetObjets[0].first)->GetType();

		if (mRealObjectType != objectsType || mBuiltWithHiddenProperties != o2EditorProperties.IsPrivateFieldsVisible())
		{
			mRealObjectType = objectsType;

			if (mRealObjectType)
				o2EditorProperties.FreeProperties(mPropertiesContext);

			if (mRealObjectType)
			{
				o2EditorProperties.BuildObjectProperties(mSpoiler, mRealObjectType, mPropertiesContext, "",
														 mOnChildFieldChangeCompleted, onChanged);

				mBuiltWithHiddenProperties = o2EditorProperties.IsPrivateFieldsVisible();
			}
		}
	}

	void DefaultObjectPropertiesViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (!targetObjets.IsEmpty())
			mRealObjectType = &(targetObjets[0].first)->GetType();
	}

	const Type* DefaultObjectPropertiesViewer::GetViewingObjectType() const
	{
		return mRealObjectType;
	}
}

DECLARE_CLASS(Editor::DefaultObjectPropertiesViewer);
