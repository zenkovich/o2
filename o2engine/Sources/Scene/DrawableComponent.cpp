#include "DrawableComponent.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace o2
{
	DrawableComponent::DrawableComponent():
		Component(), mDrawingDepth(0)
	{
		if (Scene::IsSingletonInitialzed())
			o2Scene.RegDrawableComponent(this);
		InitializeProperties();
	}

	DrawableComponent::DrawableComponent(const DrawableComponent& other):
		Component(other), mDrawingDepth(other.mDrawingDepth)
	{
		o2Scene.RegDrawableComponent(this);
		InitializeProperties();
	}

	DrawableComponent::~DrawableComponent()
	{
		o2Scene.UnregDrawableComponent(this);
	}

	DrawableComponent& DrawableComponent::operator=(const DrawableComponent& other)
	{
		Component::operator=(other);

		mDrawingDepth = other.mDrawingDepth;
		o2Scene.ComponentDepthChanged(this);

		return *this;
	}

	void DrawableComponent::Draw()
	{}

	void DrawableComponent::SetDrawingDepth(float depth)
	{
		mDrawingDepth = depth;
		o2Scene.ComponentDepthChanged(this);
	}

	float DrawableComponent::GetDrawingDepth() const
	{
		return mDrawingDepth;
	}

	void DrawableComponent::OnLayerChanged(Scene::Layer* oldLayer, Scene::Layer* newLayer)
	{
		oldLayer->drawables.Remove(this);
		newLayer->drawables.Add(this);

		if (mResEnabled)
		{
			oldLayer->enabledDrawables.Remove(this);
			newLayer->enabledDrawables.Add(this);
		}
	}

	void DrawableComponent::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabled;
		else
			mResEnabled = mEnabled;

		if (lastResEnabled != mResEnabled && mOwner)
		{
			if (mResEnabled)
				mOwner->mLayer->enabledDrawables.Add(this);
			else
				mOwner->mLayer->enabledDrawables.Remove(this);
		}
	}

	void DrawableComponent::SetOwnerActor(Actor* actor)
	{
		if (mOwner)
		{
			mOwner->mCompontents.Remove(this);

			mOwner->mLayer->drawables.Remove(this);
			if (mResEnabled)
				mOwner->mLayer->enabledDrawables.Remove(this);
		}

		mOwner = actor;

		if (mOwner)
		{
			mOwner->mCompontents.Add(this);
			OnTransformChanged();

			mOwner->mLayer->drawables.Add(this);
			if (mResEnabled)
				mOwner->mLayer->enabledDrawables.Add(this);
		}
	}

	void DrawableComponent::InitializeProperties()
	{
		INITIALIZE_PROPERTY(DrawableComponent, drawDepth, SetDrawingDepth, GetDrawingDepth);
	}
}