#include "stdafx.h"
#include "AnimationComponentViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"

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

}

DECLARE_CLASS_MANUAL(Editor::TObjectPropertiesViewer<o2::AnimationComponent>);

DECLARE_CLASS(Editor::AnimationComponentViewer);
