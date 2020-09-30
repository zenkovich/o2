#include "o2/stdafx.h"
#include "ISceneDrawable.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"

namespace o2
{
	ISceneDrawable::ISceneDrawable()
	{}

	ISceneDrawable::ISceneDrawable(const ISceneDrawable& other):
		drawDepth(this)
	{}

	ISceneDrawable::~ISceneDrawable()
	{}

	ISceneDrawable& ISceneDrawable::operator=(const ISceneDrawable& other)
	{
		SetDrawingDepth(other.mDrawingDepth);
		return *this;
	}

	void ISceneDrawable::Draw()
	{
		OnDrawn();
	}

	void ISceneDrawable::SetDrawingDepth(float depth)
	{
		mDrawingDepth = depth;

		if (auto layer = GetSceneDrawableSceneLayer())
			layer->OnDrawableDepthChanged(this);
	}

	float ISceneDrawable::GetSceneDrawableDepth() const
	{
		return mDrawingDepth;
	}

	void ISceneDrawable::OnEnabled()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
			layer->OnDrawableEnabled(this);
	}

	void ISceneDrawable::OnDisabled()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
			layer->OnDrawableEnabled(this);
	}

	void ISceneDrawable::OnAddToScene()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
		{
			layer->RegisterDrawable(this);

			if (IsSceneDrawableEnabled())
				layer->OnDrawableEnabled(this);
		}
	}

	void ISceneDrawable::OnRemoveFromScene()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
		{
			if (IsSceneDrawableEnabled())
				layer->OnDrawableDisabled(this);

			layer->UnregisterDrawable(this);
		}
	}

	void ISceneDrawable::SetLastOnCurrentDepth()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
			layer->SetLastByDepth(this);
	}

#if IS_EDITOR
	SceneEditableObject* ISceneDrawable::GetEditableOwner()
	{
		return nullptr;
	}

	void ISceneDrawable::OnDrawn()
	{
		if (auto obj = GetEditableOwner())
			o2Scene.OnObjectDrawn(obj);

		IDrawable::OnDrawn();
	}
#endif
}

DECLARE_CLASS(o2::ISceneDrawable);
