#include "DrawableComponent.h"

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

	void DrawableComponent::InitializeProperties()
	{
		INITIALIZE_PROPERTY(DrawableComponent, drawDepth, SetDrawingDepth, GetDrawingDepth);
	}
}