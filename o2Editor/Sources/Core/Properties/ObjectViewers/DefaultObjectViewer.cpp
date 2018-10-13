#include "stdafx.h"
#include "DefaultObjectViewer.h"

#include "Core/Properties/Properties.h"

namespace Editor
{

	UIWidget* DefaultObjectViewer::InitializeControls(const String& path, const OnChangeCompletedFunc& onChangeCompleted,
													  const OnChangedFunc& onChanged)
	{
		mFieldsPath = path;
		mOnFieldChanged = onChanged;
		mOnFieldChangeCompleted = onChangeCompleted;
	}

	void DefaultObjectViewer::Refresh(const TargetsVec& targetObjets)
	{
		if (targetObjets.IsEmpty())
			return;

		const Type* objectsType = (GetProxy<IObject*>(targetObjets[0]->first))->GetType();

		if (mRealObjectType == &objectsType)
			return;

		mRealObjectType = &objectsType;

		if (mRealObjectType)
			o2EditorProperties.FreeProperties(mFieldProperties);

		if (mRealObjectType)
		{
			o2EditorProperties.BuildObjectProperties(layout, &mRealObjectType, mFieldProperties, mFieldsPath,
													 mOnFieldChangeCompleted, mOnFieldChanged);

			mFieldProperties.Set(targetObjets.Select<Pair<IObject*, IObject*>>(
				[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
			{
				return Pair<IObject*, IObject*>(GetProxy<IObject*>(x.first),
												x.second ? GetProxy<IObject*>(x.second) : nullptr);
			}));
		}
	}

	const Type* DefaultObjectViewer::GetViewingObjectType() const
	{
		return &TypeOf(IObject);
	}
}

DECLARE_CLASS(Editor::DefaultObjectViewer);
