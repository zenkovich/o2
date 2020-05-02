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
	const Type* AnimationViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* AnimationViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(AnimationClip);
	}

	void AnimationViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		PushEditorScopeOnStack scope;

		mEditButton = o2UI.CreateButton("Edit", THIS_FUNC(OnEditPressed));

		DefaultObjectPropertiesViewer::RebuildProperties(targetObjets);
	}

	void AnimationViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		mSpoiler->AddChild(mEditButton);
		mEditButton->SetIndexInSiblings(mSpoiler->GetChildren().Count() - 1);

		mCurrentAnimation = !targetObjets.IsEmpty() ? dynamic_cast<AnimationClip*>(targetObjets[0].first) : nullptr;
	}

	void AnimationViewer::OnEditPressed()
	{
		o2EditorAnimationWindow.SetAnimation(mCurrentAnimation);

		if (!o2EditorSceneScreen.GetSelectedObjects().IsEmpty())
			o2EditorAnimationWindow.SetTarget(dynamic_cast<Actor*>(o2EditorSceneScreen.GetSelectedObjects().Last()));

		o2EditorAnimationWindow.GetWindow()->Focus();

		auto contextIt = mPropertiesContext.parent;
		while (contextIt)
		{
			if (!contextIt->targets.IsEmpty())
			{
				if (auto editable = dynamic_cast<IEditableAnimation*>(contextIt->targets[0].first))
				{
					o2EditorAnimationWindow.SetAnimationEditable(editable);
					break;
				}
			}

			contextIt = contextIt->parent;
		}
	}

}

DECLARE_CLASS(Editor::AnimationViewer);
