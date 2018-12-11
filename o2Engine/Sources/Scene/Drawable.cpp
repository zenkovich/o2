#include "stdafx.h"
#include "Drawable.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"

namespace o2
{
	SceneDrawable::SceneDrawable()
	{}

	SceneDrawable::SceneDrawable(const SceneDrawable& other):
		mDrawingDepth(other.mDrawingDepth), drawDepth(this)
	{}

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

	void SceneDrawable::SetLastOnCurrentDepth()
	{
		if (mLayer && mIsOnScene)
			mLayer->SetLastByDepth(this);
	}

	void SceneDrawable::SetLayer(SceneLayer* newLayer)
	{
		if (newLayer == mLayer)
			return;

		if (mLayer && mIsOnScene)
			mLayer->UnregisterDrawable(this);

		mLayer = newLayer;

		if (mLayer && mIsOnScene)
			mLayer->RegisterDrawable(this);
	}

	void SceneDrawable::OnExcludeFromScene()
	{
		if (mLayer)
			mLayer->UnregisterDrawable(this);

		mIsOnScene = false;
	}

	void SceneDrawable::OnIncludeToScene()
	{
		if (mLayer)
			mLayer->RegisterDrawable(this);

		mIsOnScene = true;
	}

#if IS_EDITOR

	o2::Vector<SceneEditableObject*> SceneDrawable::drawnLastFrameEditableObjects;

	SceneEditableObject* SceneDrawable::GetEditableOwner()
	{
		return nullptr;
	}

	void SceneDrawable::OnDrawn()
	{
		if (auto obj = GetEditableOwner())
		{
			if (obj->IsOnScene())
				drawnLastFrameEditableObjects.Add(obj);
		}

		IDrawable::OnDrawn();
	}
#endif
}

DECLARE_CLASS(o2::SceneDrawable);
