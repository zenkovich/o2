#include "o2Editor/stdafx.h"
#include "AnimationStateViewer.h"

#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{

	void AnimationStateViewer::SetCaption(const WString& caption)
	{
		DefaultObjectPropertiesViewer::SetCaption(caption);

		Text* spoilerCaptionLayer = GetSpoiler()->GetLayerDrawable<Text>("caption");
		if (spoilerCaptionLayer)
		{
			Vec2F captionSize = Text::GetTextSize(caption, spoilerCaptionLayer->GetFont().Get(), spoilerCaptionLayer->GetHeight());
			*mPlayPause->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(11 + captionSize.x, 1));
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

	Spoiler* AnimationStateViewer::CreateSpoiler()
	{
		mSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");

		mPlayPause = o2UI.CreateWidget<Toggle>("animation state play-stop");
		mPlayPause->name = "play-stop";
		*mPlayPause->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(20, 0));
		mPlayPause->onToggle = THIS_FUNC(OnPlayPauseToggled);
		mSpoiler->AddInternalWidget(mPlayPause);

		mLooped = o2UI.CreateWidget<Toggle>("animation state loop");
		mLooped->name = "loop";
		*mLooped->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-20, 1));
		mLooped->onToggle = THIS_FUNC(OnLoopToggled);
		mSpoiler->AddInternalWidget(mLooped);

		mTimeProgress = o2UI.CreateWidget<HorizontalProgress>("animation state bar");
		mTimeProgress->name = "bar";
		*mTimeProgress->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 2, 20);
		mTimeProgress->onChangeByUser = THIS_FUNC(OnTimeProgressChanged);
		mSpoiler->AddInternalWidget(mTimeProgress);

		return mSpoiler;
	}

	void AnimationStateViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer->onUpdate -= THIS_FUNC(OnAnimationUpdated);

		mSubscribedPlayer = nullptr;

		if (!targetObjets.IsEmpty())
		{
			mSubscribedPlayer = &dynamic_cast<AnimationState*>(targetObjets.Last().first)->player;
			mSubscribedPlayer->onUpdate += THIS_FUNC(OnAnimationUpdated);
		}
	}

	void AnimationStateViewer::OnFree()
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer->onUpdate -= THIS_FUNC(OnAnimationUpdated);

		mSubscribedPlayer = nullptr;
	}

	void AnimationStateViewer::OnPlayPauseToggled(bool play)
	{
		if (mSubscribedPlayer)
		{
			if (play && mSubscribedPlayer->GetRelTime() >= 1.0f - FLT_EPSILON)
				mSubscribedPlayer->SetRelTime(0.0f);

			mSubscribedPlayer->SetPlaying(play);
		}

		o2Scene.OnObjectChanged(o2EditorSceneScreen.GetSelectedObjects().First());
	}

	void AnimationStateViewer::OnLoopToggled(bool looped)
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer->SetLoop(looped ? Loop::Repeat : Loop::None);

		o2Scene.OnObjectChanged(o2EditorSceneScreen.GetSelectedObjects().First());
	}

	void AnimationStateViewer::OnTimeProgressChanged(float value)
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer->SetRelTime(value);
	}

	void AnimationStateViewer::OnAnimationUpdated(float time)
	{
		mTimeProgress->value = mSubscribedPlayer->GetRelTime();
	}

}

DECLARE_CLASS(Editor::AnimationStateViewer);
