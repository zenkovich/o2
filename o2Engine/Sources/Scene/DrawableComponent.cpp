#include "stdafx.h"
#include "DrawableComponent.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"

namespace o2
{
	DrawableComponent::DrawableComponent():
		Component(), SceneDrawable()
	{}

	DrawableComponent::DrawableComponent(const DrawableComponent& other) :
		Component(other), SceneDrawable(other)
	{}

	DrawableComponent& DrawableComponent::operator=(const DrawableComponent& other)
	{
		Component::operator=(other);
		SceneDrawable::operator=(other);

		return *this;
	}

	void DrawableComponent::SetDrawingDepth(float depth)
	{
		SceneDrawable::SetDrawingDepth(depth);

		if (mOwner)
			mOwner->OnChanged();
	}

	void DrawableComponent::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabledInHierarchy;
		else
			mResEnabled = mEnabled;

		if (lastResEnabled != mResEnabled && mLayer)
		{
			if (mResEnabled)
				mLayer->DrawableEnabled(this);
			else
				mLayer->DrawableDisabled(this);
		}
	}

	void DrawableComponent::SetOwnerActor(Actor* actor)
	{
		SceneDrawable::SetLayer(actor->mLayer);

		if (mOwner)
		{
			mOwner->mComponents.Remove(this);

			if (mOwner->mIsOnScene)
				mOwner->mLayer->UnregisterDrawable(this);
		}

		mOwner = actor;

		if (mOwner)
		{
			mOwner->mComponents.Add(this);

			if (mOwner->mIsOnScene)
				mOwner->mLayer->RegisterDrawable(this);

			OnTransformUpdated();
		}
	}

#if IS_EDITOR
	SceneEditableObject* DrawableComponent::GetEditableOwner()
	{
		return mOwner;
	}
#endif
}

DECLARE_CLASS(o2::DrawableComponent);
