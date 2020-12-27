#include "o2/stdafx.h"
#include "DrawableComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"

namespace o2
{
	DrawableComponent::DrawableComponent():
		Component(), ISceneDrawable()
	{}

	DrawableComponent::DrawableComponent(const DrawableComponent& other) :
		Component(other), ISceneDrawable(other)
	{}

	DrawableComponent& DrawableComponent::operator=(const DrawableComponent& other)
	{
		Component::operator=(other);
		ISceneDrawable::operator=(other);

		return *this;
	}

	void DrawableComponent::SetDrawingDepth(float depth)
	{
		ISceneDrawable::SetDrawingDepth(depth);

		if (mOwner)
			mOwner->OnChanged();
	}

	void DrawableComponent::OnSerialize(DataValue& node) const
	{
		Component::OnSerialize(node);
	}

	void DrawableComponent::OnDeserialized(const DataValue& node)
	{
		Component::OnDeserialized(node);
	}

	void DrawableComponent::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabledInHierarchy;
		else
			mResEnabled = mEnabled;

		if (lastResEnabled != mResEnabled)
		{
			if (mResEnabled)
				ISceneDrawable::OnEnabled();
			else
				ISceneDrawable::OnDisabled();
		}
	}

	SceneLayer* DrawableComponent::GetSceneDrawableSceneLayer() const
	{
		return mOwner->mLayer;
	}

	bool DrawableComponent::IsSceneDrawableEnabled() const
	{
		return mResEnabled;
	}

	void DrawableComponent::OnAddToScene()
	{
		ISceneDrawable::OnAddToScene();
	}

	void DrawableComponent::OnRemoveFromScene()
	{
		ISceneDrawable::OnRemoveFromScene();
	}

#if IS_EDITOR
	SceneEditableObject* DrawableComponent::GetEditableOwner()
	{
		return mOwner;
	}
#endif
}

DECLARE_CLASS(o2::DrawableComponent);
