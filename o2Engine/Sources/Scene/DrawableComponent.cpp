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
		if (mOwner && mOwner->mLayer)
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
		if (oldLayer)
			oldLayer->UnregDrawableComponent(this);

		if (newLayer)
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

			if (mOwner->mIsOnScene)
				mOwner->mLayer->UnregDrawableComponent(this);
		}

		mOwner = actor;

		if (mOwner)
		{
			mOwner->mComponents.Add(this);

			if (mOwner->mIsOnScene)
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

CLASS_META(o2::DrawableComponent)
{
	BASE_CLASS(o2::Component);

	PUBLIC_FIELD(drawDepth);
	PROTECTED_FIELD(mDrawingDepth).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, SetDrawingDepth, float);
	PUBLIC_FUNCTION(float, GetDrawingDepth);
	PROTECTED_FUNCTION(void, OnLayerChanged, Scene::Layer*, Scene::Layer*);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PROTECTED_FUNCTION(void, OnExcludeFromScene);
	PROTECTED_FUNCTION(void, OnIncludeToScene);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
