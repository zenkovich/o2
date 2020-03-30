#include "o2Editor/stdafx.h"
#include "AnimationViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	AnimationViewer::AnimationViewer() :
		DefaultObjectPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mEditButton = o2UI.CreateButton("Edit", THIS_FUNC(OnEditPressed));
	}

	void AnimationViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		DefaultObjectPropertiesViewer::Refresh(targetObjets);

		mLayout->AddChild(mEditButton);
		mEditButton->SetIndexInSiblings(mLayout->GetChildren().Count() - 1);

		mCurrentAnimation = !targetObjets.IsEmpty() ? dynamic_cast<Animation*>(targetObjets[0].first) : nullptr;
	}

	const Type* AnimationViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* AnimationViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(Animation);
	}

	void AnimationViewer::OnEditPressed()
	{
		o2EditorAnimationWindow.SetAnimation(mCurrentAnimation);

		if (!o2EditorSceneScreen.GetSelectedObjects().IsEmpty())
			o2EditorAnimationWindow.SetTarget(dynamic_cast<Actor*>(o2EditorSceneScreen.GetSelectedObjects().Last()));

		o2EditorAnimationWindow.GetWindow()->Focus();
	}

}

DECLARE_CLASS(Editor::AnimationViewer);
