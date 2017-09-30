#include "Drawable.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"

namespace o2
{
	SceneDrawable::SceneDrawable()
	{
		InitializeProperties();
	}

	SceneDrawable::SceneDrawable(const SceneDrawable& other):
		mDrawingDepth(other.mDrawingDepth)
	{
		InitializeProperties();
	}

	SceneDrawable::~SceneDrawable()
	{
		if (mLayer)
			mLayer->UnregisterDrawable(this);
	}

	SceneDrawable& SceneDrawable::operator=(const SceneDrawable& other)
	{
		mDrawingDepth = other.mDrawingDepth;

		if (mLayer)
			mLayer->DrawableDepthChanged(this);

		return *this;
	}

	void SceneDrawable::Draw()
	{
		OnDrawn();
	}

	void SceneDrawable::SetDrawingDepth(float depth)
	{
		mDrawingDepth = depth;

		if (mLayer)
			mLayer->DrawableDepthChanged(this);
	}

	float SceneDrawable::GetDrawingDepth() const
	{
		return mDrawingDepth;
	}

	void SceneDrawable::SetLayer(SceneLayer* newLayer)
	{
		if (mLayer)
			mLayer->UnregisterDrawable(this);

		mLayer = newLayer;

		if (mLayer)
			mLayer->RegisterDrawable(this);
	}

	void SceneDrawable::OnExcludeFromScene()
	{
		if (mLayer)
			mLayer->UnregisterDrawable(this);
	}

	void SceneDrawable::OnIncludeToScene()
	{
		if (mLayer)
			mLayer->RegisterDrawable(this);
	}

	void SceneDrawable::InitializeProperties()
	{
		INITIALIZE_PROPERTY(SceneDrawable, drawDepth, SetDrawingDepth, GetDrawingDepth);
	}
}