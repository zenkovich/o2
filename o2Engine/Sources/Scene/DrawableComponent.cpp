#include "DrawableComponent.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace o2
{
	DrawableComponent::DrawableComponent():
		Component(), mDrawingDepth(0)
	{
		InitializeProperties();
	}

	DrawableComponent::DrawableComponent(const DrawableComponent& other):
		Component(other), mDrawingDepth(other.mDrawingDepth)
	{
		InitializeProperties();
	}

	DrawableComponent::~DrawableComponent()
	{
		if (mOwner)
			mOwner->mLayer->UnregDrawableComponent(this);
	}

	DrawableComponent& DrawableComponent::operator=(const DrawableComponent& other)
	{
		Component::operator=(other);

		mDrawingDepth = other.mDrawingDepth;

		if (mOwner)
			mOwner->mLayer->ComponentDepthChanged(this);

		return *this;
	}

	void DrawableComponent::Draw()
	{}

	void DrawableComponent::SetDrawingDepth(float depth)
	{
		mDrawingDepth = depth;

		if (mOwner)
		{
			mOwner->mLayer->ComponentDepthChanged(this);
			mOwner->OnChanged();
		}
	}

	float DrawableComponent::GetDrawingDepth() const
	{
		return mDrawingDepth;
	}

	void DrawableComponent::OnLayerChanged(Scene::Layer* oldLayer, Scene::Layer* newLayer)
	{
		oldLayer->UnregDrawableComponent(this);
		newLayer->RegDrawableComponent(this);
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
				mOwner->mLayer->ComponentEnabled(this);
			else
				mOwner->mLayer->ComponentDisabled(this);
		}
	}

	void DrawableComponent::SetOwnerActor(Actor* actor)
	{
		if (mOwner)
		{
			mOwner->mComponents.Remove(this);
			mOwner->mLayer->UnregDrawableComponent(this);
		}

		mOwner = actor;

		if (mOwner)
		{
			mOwner->mComponents.Add(this);
			mOwner->mLayer->RegDrawableComponent(this);

			OnTransformChanged();
		}
	}

	void DrawableComponent::OnExcludeFromScene()
	{
		if (mOwner)
			mOwner->mLayer->UnregDrawableComponent(this);
	}

	void DrawableComponent::OnIncludeToScene()
	{
		if (mOwner)
			mOwner->mLayer->RegDrawableComponent(this);
	}

	void DrawableComponent::InitializeProperties()
	{
		INITIALIZE_PROPERTY(DrawableComponent, drawDepth, SetDrawingDepth, GetDrawingDepth);
	}
}