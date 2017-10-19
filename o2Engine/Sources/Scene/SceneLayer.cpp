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

CLASS_META(o2::SceneLayer)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mActors);
	PROTECTED_FIELD(mEnabledActors);
	PROTECTED_FIELD(mDrawables);
	PROTECTED_FIELD(mEnabledDrawables);

	PUBLIC_FUNCTION(const ActorsVec&, GetActors);
	PUBLIC_FUNCTION(const ActorsVec&, GetEnabledActors);
	PUBLIC_FUNCTION(const DrawablesVec&, GetDrawables);
	PUBLIC_FUNCTION(const DrawablesVec&, GetEnabledDrawables);
	PROTECTED_FUNCTION(void, RegisterDrawable, SceneDrawable*);
	PROTECTED_FUNCTION(void, UnregisterDrawable, SceneDrawable*);
	PROTECTED_FUNCTION(void, DrawableDepthChanged, SceneDrawable*);
	PROTECTED_FUNCTION(void, DrawableEnabled, SceneDrawable*);
	PROTECTED_FUNCTION(void, DrawableDisabled, SceneDrawable*);
}
END_META;
