#include "o2Editor/stdafx.h"
#include "AnimationStateViewer.h"

#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"

namespace Editor
{
	AnimationStateViewer::AnimationStateViewer()
	{
		PushEditorScopeOnStack scope; 
		
		mPlayPause = o2UI.CreateWidget<Toggle>("animation state play-stop");
		mPlayPause->name = "play-stop";
		*mPlayPause->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(20, 0));
		mPlayPause->onClick = THIS_FUNC(OnPlayPauseToggled);

		mLooped = o2UI.CreateWidget<Toggle>("animation state loop");
		mLooped->name = "loop";
		*mLooped->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-20, 0));

		mTimeProgress = o2UI.CreateWidget<HorizontalProgress>("animation state bar");
		mTimeProgress->name = "bar";
		*mTimeProgress->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 2, 20);
	}

	void AnimationStateViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		DefaultObjectPropertiesViewer::Refresh(targetObjets);
	}

	void AnimationStateViewer::Prepare()
	{
		if (auto parentSpoiler = dynamic_cast<Spoiler*>(mLayout->GetParentWidget()))
		{
			if (auto parentHeader = parentSpoiler->FindInternalWidgetByType<HorizontalLayout>())
				parentHeader->AddInternalWidget(mLooped);

			parentSpoiler->AddInternalWidget(mPlayPause);
			parentSpoiler->AddInternalWidget(mTimeProgress);
		}
	}

	const Type* AnimationStateViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* AnimationStateViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(AnimationState);
	}

	void AnimationStateViewer::OnFree()
	{
		mLooped->SetParent(nullptr);
		mPlayPause->SetParent(nullptr);
		mTimeProgress->SetParent(nullptr);
	}

	void AnimationStateViewer::OnPlayPauseToggled(bool value)
	{

	}

	void AnimationStateViewer::OnLoopToggled(bool value)
	{

	}

	void AnimationStateViewer::OnAnimationUpdate(float time)
	{

	}

}

DECLARE_CLASS(Editor::AnimationStateViewer);
