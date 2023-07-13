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
			else
			{
				if (mLayer)
				{
					if (mIsEnabled)
						mLayer->OnDrawableDisabled(this, false);

					mLayer->UnregisterDrawable(this);
				}
			}
		}

        mChildrenInheritedDepth.Clear();
	}

	ISceneDrawable& ISceneDrawable::operator=(const ISceneDrawable& other)
	{
		SetDrawingDepth(other.mDrawingDepth);
		return *this;
	}

	void ISceneDrawable::Draw()
	{
		//PROFILE_SAMPLE_FUNC();

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
		mLayer = GetSceneDrawableSceneLayer();
		if (mLayer)
			mLayer->OnDrawableEnabled(this, false);

		mIsEnabled = true;
	}

	void ISceneDrawable::OnDisabled()
	{
		if (mLayer)
			mLayer->OnDrawableDisabled(this, false);

		mLayer = nullptr;
		mIsEnabled = false;
	}

	void ISceneDrawable::OnAddToScene(bool force /*= false*/)
	{
		mLayer = GetSceneDrawableSceneLayer();
		if (mLayer)
		{
			mLayer->RegisterDrawable(this);

			mIsEnabled = IsSceneDrawableEnabled();
			if (mIsEnabled)
				mLayer->OnDrawableEnabled(this, force);
		}
	}

	void ISceneDrawable::OnRemoveFromScene(bool force /*= false*/)
	{
		if (mLayer)
		{
			mIsEnabled = IsSceneDrawableEnabled();
			if (mIsEnabled)
				mLayer->OnDrawableDisabled(this, force);

			mLayer->UnregisterDrawable(this);
		}

		mLayer = nullptr;
	}

	void ISceneDrawable::SetLastOnCurrentDepth()
	{
		if (mInheritDrawingDepthFromParent)
			return;

		if (mLayer)
			mLayer->SetLastByDepth(this);
	}

    const Vector<ISceneDrawable*>& ISceneDrawable::GetChildrenInheritedDepth() const
    {
        return mChildrenInheritedDepth;
    }

	bool ISceneDrawable::IsDrawableEnabled() const
	{
		return mIsEnabled;
	}

#if IS_EDITOR
	SceneEditableObject* ISceneDrawable::GetEditableOwner()
	{
		return nullptr;
	}

	void ISceneDrawable::OnDrawn()
	{
		//PROFILE_SAMPLE_FUNC();

		if (auto obj = GetEditableOwner())
			o2Scene.OnObjectDrawn(obj);

		IDrawable::OnDrawn();

		drawCallIdx = o2Render.GetDrawCallsCount();
	}
#endif
}
// --- META ---

DECLARE_CLASS(o2::ISceneDrawable);
// --- END META ---
