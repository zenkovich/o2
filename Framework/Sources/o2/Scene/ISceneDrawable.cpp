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
		mDrawingDepth(other.mDrawingDepth), mInheritDrawingDepthFromParent(other.mInheritDrawingDepthFromParent), drawDepth(this)
	{
	}

	ISceneDrawable::~ISceneDrawable()
	{
		if (mRegistered)
			Unregister();

		for (auto child : mChildrenInheritedDepth)
		{
			child->mParentRegistry = nullptr;
			child->mRegistered = false;
		}
	}

	ISceneDrawable& ISceneDrawable::operator=(const ISceneDrawable& other)
	{
		if (other.mInheritDrawingDepthFromParent)
			SetDrawingDepthInheritFromParent(true);
		else
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
		mInheritDrawingDepthFromParent = false;

		Reregister();
	}

	float ISceneDrawable::GetDrawingDepth() const
	{
		return mDrawingDepth;
	}

	void ISceneDrawable::SetDrawingDepthInheritFromParent(bool inherit)
	{
		mInheritDrawingDepthFromParent = inherit;

		Reregister();
	}

	bool ISceneDrawable::IsDrawingDepthInheritedFromParent() const
	{
		return mInheritDrawingDepthFromParent;
	}

	void ISceneDrawable::OnDrawbleParentChanged()
	{
		Reregister();
	}

	void ISceneDrawable::OnDrawableLayerChanged()
	{
		Reregister();
	}

	void ISceneDrawable::SortInheritedDrawables()
	{
		mChildrenInheritedDepth.SortBy<int>([](ISceneDrawable* x) { return x->GetIndexInParentDrawable(); });
	}

	void ISceneDrawable::OnEnabled()
	{
		mDrawableEnabled = true;

		if (mRegistered)
			Unregister();

		Register();
	}

	void ISceneDrawable::OnDisabled()
	{
		mDrawableEnabled = false;

		if (mRegistered)
			Unregister();
	}

	void ISceneDrawable::OnAddToScene()
	{
		mIsOnScene = true;
		Reregister();
	}

	void ISceneDrawable::OnRemoveFromScene()
	{
		mIsOnScene = false;

		if (mRegistered)
			Unregister();
	}

	void ISceneDrawable::Reregister()
	{
		if (mRegistered)
			Unregister();

		if (mDrawableEnabled)
			Register();
	}

	void ISceneDrawable::Register()
	{
		if (mInheritDrawingDepthFromParent)
		{
			if (mDrawableEnabled)
			{
				mParentRegistry = GetParentDrawable();
				if (!mParentRegistry && mIsOnScene)
					mParentRegistry = &GetSceneDrawableSceneLayer()->GetRootDrawables();

				if (mParentRegistry)
				{
// 					if (mParentRegistry->mChildrenInheritedDepth.Contains(this))
// 						o2Debug.Log("asd");

					mParentRegistry->mChildrenInheritedDepth.Add(this);
					mParentRegistry->SortInheritedDrawables();

					mRegistered = true;
				}
			}
		}
		else
		{
			if (mDrawableEnabled && mIsOnScene)
			{
				mLayerRegistry = GetSceneDrawableSceneLayer();
				mLayerRegistry->RegisterDrawable(this);

				mRegistered = true;
			}
		}

	}

	void ISceneDrawable::Unregister()
	{
		if (mParentRegistry)
			mParentRegistry->mChildrenInheritedDepth.Remove(this);
		else
			mLayerRegistry->UnregisterDrawable(this);

		mParentRegistry = nullptr;
		mLayerRegistry = nullptr;

		mRegistered = false;
	}

	void ISceneDrawable::SetLastOnCurrentDepth()
	{
		if (!mRegistered)
			return;

		if (mParentRegistry)
		{
			mParentRegistry->mChildrenInheritedDepth.Remove(this);
			mParentRegistry->mChildrenInheritedDepth.Add(this);
		}
		else
			mLayerRegistry->SetLastByDepth(this);
	}

	const Vector<ISceneDrawable*>& ISceneDrawable::GetChildrenInheritedDepth() const
    {
        return mChildrenInheritedDepth;
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
