#include "stdafx.h"
#include "AnimationComponentViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Scene/Components/AnimationComponent.h"

namespace Editor
{
	AnimationComponentViewer::AnimationComponentViewer()
	{
		PushEditorScopeOnStack scope;

		o2EditorProperties.BuildField(mLayout, TypeOf(AnimationComponent), "mStates", "", mPropertiesContext, 
									  mOnChildFieldChangeCompleted, onChanged);
	}

	void AnimationComponentViewer::Refresh(const TargetsVec& targetObjets)
	{
		mPropertiesContext.Set(targetObjets);
	}

	const Type* AnimationComponentViewer::GetViewingObjectType() const
	{
		return &TypeOf(AnimationComponent);
	}

}

DECLARE_CLASS(Editor::AnimationComponentViewer);
