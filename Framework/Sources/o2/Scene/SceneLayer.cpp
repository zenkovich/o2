#include "o2/stdafx.h"
#include "SceneLayer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Drawable.h"
#include "o2/Scene/Scene.h"

namespace o2
{
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

	const Vector<SceneDrawable*>& SceneLayer::GetDrawables() const
	{
		return mDrawables;
	}

	const Vector<SceneDrawable*>& SceneLayer::GetEnabledDrawables() const
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

	void SceneLayer::RegisterDrawable(SceneDrawable* drawable)
	{
		mDrawables.Add(drawable);
		OnDrawableEnabled(drawable);
	}

	void SceneLayer::UnregisterDrawable(SceneDrawable* drawable)
	{
		OnDrawableDisabled(drawable);
		mDrawables.Remove(drawable);
	}

	void SceneLayer::OnDrawableDepthChanged(SceneDrawable* drawable)
	{
		OnDrawableDisabled(drawable);
		OnDrawableEnabled(drawable);
	}

	void SceneLayer::OnDrawableEnabled(SceneDrawable* drawable)
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

	void SceneLayer::OnDrawableDisabled(SceneDrawable* drawable)
	{
		mEnabledDrawables.Remove(drawable);
	}

	void SceneLayer::SetLastByDepth(SceneDrawable* drawable)
	{
		OnDrawableDisabled(drawable);

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


// 	void LayerDataValueConverter::ToData(void* object, DataValue& data)
// 	{
// 		if (object)
// 		{
// 			SceneLayer* value = (SceneLayer*)object;
// 			data = value->name;
// 		}
// 	}
// 
// 	void LayerDataValueConverter::FromData(void* object, const DataValue& data)
// 	{
// 		SceneLayer*& value = *(SceneLayer**)object;
// 		value = o2Scene.GetLayer(data);
// 	}
// 
// 	bool LayerDataValueConverter::IsConvertsType(const Type* type) const
// 	{
// 		return type->IsBasedOn(*TypeOf(SceneLayer).GetPointerType());
// 	}
}

DECLARE_CLASS(o2::SceneLayer);
