#include "SceneLayer.h"

#include "Scene/Actor.h"
#include "Scene/Drawable.h"

namespace o2
{

	const SceneLayer::ActorsVec& SceneLayer::GetActors() const
	{
		return mActors;
	}

	const SceneLayer::ActorsVec& SceneLayer::GetEnabledActors() const
	{
		return mEnabledActors;
	}

	const SceneLayer::DrawablesVec& SceneLayer::GetDrawables() const
	{
		return mDrawables;
	}

	const SceneLayer::DrawablesVec& SceneLayer::GetEnabledDrawables() const
	{
		return mEnabledDrawables;
	}

	void SceneLayer::RegisterDrawable(SceneDrawable* drawable)
	{
		mDrawables.Add(drawable);

		if (drawable->mOwner && drawable->mOwner->mIsOnScene)
			DrawableEnabled(drawable);
	}

	void SceneLayer::UnregisterDrawable(SceneDrawable* drawable)
	{
		DrawableDisabled(drawable);
		mDrawables.Remove(drawable);
	}

	void SceneLayer::DrawableDepthChanged(SceneDrawable* drawable)
	{
		DrawableDisabled(drawable);
		DrawableEnabled(drawable);
	}

	void SceneLayer::DrawableEnabled(SceneDrawable* drawable)
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

	void SceneLayer::DrawableDisabled(SceneDrawable* drawable)
	{
		mEnabledDrawables.Remove(drawable);
	}
}