#include "stdafx.h"
#include "DefaultObjectViewer.h"

#include "Core/Properties/Properties.h"

namespace Editor
{

	void DefaultObjectViewer::InitializeControls(UIVerticalLayout* layout, const Type& objectType,
												 const OnChangeCompletedFunc& onChangeCompleted,
												 const OnChangedFunc& onChanged)
	{
		mSpecializedType = &objectType;
		o2EditorProperties.BuildObjectProperties(layout, &objectType, mFieldProperties, "", onChangeCompleted, onChanged);
	}

	void DefaultObjectViewer::Refresh(const TargetsVec& targetObjets)
	{
		if (mSpecializedType)
		{
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
