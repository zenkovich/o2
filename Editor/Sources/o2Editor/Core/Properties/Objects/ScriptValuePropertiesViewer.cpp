#include "o2Editor/stdafx.h"
#include "ScriptValuePropertiesViewer.h"

#if IS_SCRIPTING_SUPPORTED

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"

namespace Editor
{
	bool ScriptValuePropertiesViewer::CheckBuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		IObjectPropertiesViewer::CheckBuildProperties(targetObjets);

		if (targetObjets.IsEmpty())
			return false;

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

			return true;
		}

		return false;
	}

	void ScriptValuePropertiesViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (!targetObjets.IsEmpty())
			mRealObjectType = &(targetObjets[0].first)->GetType();
	}

	const Type* ScriptValuePropertiesViewer::GetViewingObjectType() const
	{
		return mRealObjectType;
	}
}

#endif

DECLARE_CLASS(Editor::DefaultObjectPropertiesViewer);
