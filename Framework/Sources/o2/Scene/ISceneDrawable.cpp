#include "o2/stdafx.h"
#include "ISceneDrawable.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"
#include <o2/Utils/Debug/StackTrace.h>

namespace o2
{
	ISceneDrawable::ISceneDrawable()
	{
	}

	ISceneDrawable::ISceneDrawable(const ISceneDrawable& other) :
		drawDepth(this)
	{
	}

	ISceneDrawable::~ISceneDrawable()
	{
		if (mInheritDrawingDepthFromParent)
		{
			if (mParentDrawable)
			{
				mParentDrawable->mChildrenInheritedDepth.Remove(this);
				mParentDrawable->SortInheritedDrawables();
			}
		}
	}

	ISceneDrawable& ISceneDrawable::operator=(const ISceneDrawable& other)
	{
		SetDrawingDepth(other.mDrawingDepth);
		return *this;
	}

	void ISceneDrawable::Draw()
	{
		OnDrawn();

		for (auto child : mChildrenInheritedDepth)
			child->Draw();
	}

	void ISceneDrawable::SetDrawingDepth(float depth)
	{
		mDrawingDepth = depth;

		if (mInheritDrawingDepthFromParent)
			return;

		if (auto layer = GetSceneDrawableSceneLayer())
			layer->OnDrawableDepthChanged(this);
	}

	float ISceneDrawable::GetDrawingDepth() const
	{
		return mDrawingDepth;
	}

	void ISceneDrawable::SetDrawingDepthInheritFromParent(bool inherit)
	{
		mInheritDrawingDepthFromParent = inherit;

		auto parent = GetParentDrawable();
		if (mInheritDrawingDepthFromParent)
		{
			if (parent)
			{
				parent->mChildrenInheritedDepth.Add(this);
				parent->SortInheritedDrawables();
			}

			OnRemoveFromScene(true);
		}
		else
		{
			if (parent)
			{
				parent->mChildrenInheritedDepth.Remove(this);
				parent->SortInheritedDrawables();
			}

			OnAddToScene(true);
		}
	}

	bool ISceneDrawable::IsDrawingDepthInheritedFromParent() const
	{
		return mInheritDrawingDepthFromParent;
	}

	void ISceneDrawable::OnDrawbleParentChanged()
	{
		if (!mInheritDrawingDepthFromParent)
			return;

		if (mParentDrawable)
		{
			mParentDrawable->mChildrenInheritedDepth.Remove(this);
			mParentDrawable->SortInheritedDrawables();
		}

		mParentDrawable = GetParentDrawable();

		if (mParentDrawable)
		{
			mParentDrawable->mChildrenInheritedDepth.Add(this);
			mParentDrawable->SortInheritedDrawables();
		}
	}

	void ISceneDrawable::SortInheritedDrawables()
	{
		mChildrenInheritedDepth.SortBy<int>([](ISceneDrawable* x) { return x->GetIndexInParentDrawable(); });
	}

	void ISceneDrawable::OnEnabled()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
			layer->OnDrawableEnabled(this, false);
	}

	void ISceneDrawable::OnDisabled()
	{
		if (auto layer = GetSceneDrawableSceneLayer())
			layer->OnDrawableDisabled(this, false);
	}

	void ISceneDrawable::OnAddToScene(bool force /*= false*/)
	{
		if (auto layer = GetSceneDrawableSceneLayer())
		{
			layer->RegisterDrawable(this);

			if (IsSceneDrawableEnabled())
				layer->OnDrawableEnabled(this, force);
		}
	}

	void ISceneDrawable::OnRemoveFromScene(bool force /*= false*/)
	{
		if (auto layer = GetSceneDrawableSceneLayer())
		{
			if (IsSceneDrawableEnabled())
				layer->OnDrawableDisabled(this, force);

			layer->UnregisterDrawable(this);
		}
	}

	void ISceneDrawable::SetLastOnCurrentDepth()
	{
		if (mInheritDrawingDepthFromParent)
			return;

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
