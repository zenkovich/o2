#include "o2/stdafx.h"
#include "SceneLayer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	SceneLayer::SceneLayer()
	{
		OnDrawableEnabled(&mRootDrawables, true);
	}

	void SceneLayer::SetName(const String& name)
	{
		String oldName = mName;
		mName = name;

		o2Scene.OnLayerRenamed(this, oldName);

		for (auto actor : mActors)
			actor->mLayerName = name;
	}

	const String& SceneLayer::GetName() const
	{
		return mName;
	}

	const Vector<Actor*>& SceneLayer::GetActors() const
	{
		return mActors;
	}

	const Vector<Actor*>& SceneLayer::GetEnabledActors() const
	{
		return mEnabledActors;
	}

	const Vector<ISceneDrawable*>& SceneLayer::GetDrawables() const
	{
		return mDrawables;
	}

	const Vector<ISceneDrawable*>& SceneLayer::GetEnabledDrawables() const
	{
		return mEnabledDrawables;
	}

	void SceneLayer::RegisterActor(Actor* actor)
	{
		mActors.Add(actor);
	}

	void SceneLayer::UnregisterActor(Actor* actor)
	{
		mActors.Remove(actor);
	}

	void SceneLayer::OnActorEnabled(Actor* actor)
	{
		mEnabledActors.Add(actor);
	}

	void SceneLayer::OnActorDisabled(Actor* actor)
	{
		mEnabledActors.Remove(actor);
	}

	void SceneLayer::RegisterDrawable(ISceneDrawable* drawable)
	{
		mDrawables.Add(drawable);
	}

	void SceneLayer::UnregisterDrawable(ISceneDrawable* drawable)
	{
		mDrawables.Remove(drawable);
	}

	void SceneLayer::OnDrawableDepthChanged(ISceneDrawable* drawable)
	{
		OnDrawableDisabled(drawable, false);
		OnDrawableEnabled(drawable, false);
	}

	void SceneLayer::OnDrawableEnabled(ISceneDrawable* drawable, bool force)
	{
		if (force || !drawable->mInheritDrawingDepthFromParent)
		{
			const int binSearchRangeSizeStop = 5;
			int rangeMin = 0, rangeMax = mEnabledDrawables.Count();
			float targetDepth = drawable->mDrawingDepth;
			int position = 0;
			bool skipLinearSearch = false;

			while (rangeMax - rangeMin > binSearchRangeSizeStop)
			{
				int center = (rangeMin + rangeMax) >> 1;

				float centerValue = mEnabledDrawables[center]->mDrawingDepth;

				if (targetDepth < centerValue)
					rangeMax = center;
				else if (targetDepth > centerValue)
					rangeMin = center;
				else
				{
					position = center;
					skipLinearSearch = true;
					break;
				}
			}

			if (!skipLinearSearch)
			{
				for (position = rangeMin; position < rangeMax; position++)
					if (mEnabledDrawables[position]->mDrawingDepth > targetDepth)
						break;
			}

			mEnabledDrawables.Insert(drawable, position);
		}
		else if (drawable->mInheritDrawingDepthFromParent && drawable->mParentDrawable == nullptr)
		{
			mRootDrawables.drawables.Add(drawable);
			mRootDrawables.drawables.SortBy<int>([](ISceneDrawable* x) { return x->GetIndexInParentDrawable(); });
		}
	}

	void SceneLayer::OnDrawableDisabled(ISceneDrawable* drawable, bool force)
	{
		if (force || !drawable->mInheritDrawingDepthFromParent)
			mEnabledDrawables.Remove(drawable);
		else if (drawable->mInheritDrawingDepthFromParent && drawable->mParentDrawable == nullptr)
			mRootDrawables.drawables.Remove(drawable);
	}

	void SceneLayer::SetLastByDepth(ISceneDrawable* drawable)
	{
		OnDrawableDisabled(drawable, drawable->IsDrawingDepthInheritedFromParent() && drawable->GetParentDrawable() == nullptr);

		for (int position = 0; position < mEnabledDrawables.Count(); position++)
		{
			if (mEnabledDrawables[position]->mDrawingDepth > drawable->mDrawingDepth)
			{
				mEnabledDrawables.Insert(drawable, position);
				return;
			}
		}

		mEnabledDrawables.Add(drawable);
	}

	void SceneLayer::RootDrawablesContainer::Draw()
	{
		for (auto drawable : drawables)
			drawable->Draw();

        ISceneDrawable::OnDrawn();
	}
}
// --- META ---

DECLARE_CLASS(o2::SceneLayer);
// --- END META ---
