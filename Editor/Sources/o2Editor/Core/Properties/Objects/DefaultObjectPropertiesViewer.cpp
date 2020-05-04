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

		if (mBuiltObjectType != objectsType || mBuiltWithHiddenProperties != o2EditorProperties.IsPrivateFieldsVisible())
		{
			mBuiltObjectType = objectsType;

			if (mBuiltObjectType)
				o2EditorProperties.FreeProperties(mPropertiesContext);

			if (mBuiltObjectType)
			{
				o2EditorProperties.BuildObjectProperties(mSpoiler, mBuiltObjectType, mPropertiesContext, "",
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
