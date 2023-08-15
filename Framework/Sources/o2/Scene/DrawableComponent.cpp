#include "o2/stdafx.h"
#include "DrawableComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"

namespace o2
{
	DrawableComponent::DrawableComponent() :
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

#if IS_EDITOR
		if (mOwner)
			mOwner->OnChanged();
#endif
	}

	void DrawableComponent::OnSerialize(DataValue& node) const
	{
		Component::OnSerialize(node);
		ISceneDrawable::OnSerialize(node);
	}

	void DrawableComponent::OnDeserialized(const DataValue& node)
	{
		Component::OnDeserialized(node);
		ISceneDrawable::OnDeserialized(node);
	}

	void DrawableComponent::OnSerializeDelta(DataValue& node, const IObject& origin) const
	{
		Component::OnSerializeDelta(node, origin);
		ISceneDrawable::OnSerializeDelta(node, origin);
	}

	void DrawableComponent::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		Component::OnDeserializedDelta(node, origin);
		ISceneDrawable::OnDeserializedDelta(node, origin);
	}

	void DrawableComponent::OnEnabled()
	{
		ISceneDrawable::OnEnabled();
	}

	void DrawableComponent::OnDisabled()
	{
		ISceneDrawable::OnDisabled();
	}

	void DrawableComponent::OnParentChanged(Actor* oldParent)
	{
		OnDrawbleParentChanged();
	}

	void DrawableComponent::OnAddToScene()
	{
		Component::OnAddToScene();
		ISceneDrawable::OnAddToScene();
	}

	void DrawableComponent::OnRemoveFromScene()
	{
		Component::OnRemoveFromScene();
		ISceneDrawable::OnRemoveFromScene();
	}

	void DrawableComponent::OnChildrenChanged()
	{
		ISceneDrawable::SortInheritedDrawables();
	}

	ISceneDrawable* DrawableComponent::GetParentDrawable()
	{
		if (!mOwner)
			return nullptr;

		if (mOwner->mParent)
		{
			auto itParent = mOwner->mParent;
			while (itParent)
			{
				if (auto drawable = dynamic_cast<ISceneDrawable*>(itParent))
					return drawable;

				auto comp = itParent->GetComponent<DrawableComponent>();
				if (comp)
					return (ISceneDrawable*)comp;

				itParent = itParent->mParent;
			}
		}

		return nullptr;
	}

	int DrawableComponent::GetIndexInParentDrawable() const
	{
		if (mOwner)
		{
			if (mOwner->mParent)
				return mOwner->mParent->mChildren.IndexOf(mOwner);
			else
				return o2Scene.mRootActors.IndexOf(mOwner);
		}

		return 0;
	}

	SceneLayer* DrawableComponent::GetSceneDrawableSceneLayer() const
	{
		return const_cast<SceneLayer*>(&mLayer.Get());
	}

#if IS_EDITOR
	SceneEditableObject* DrawableComponent::GetEditableOwner()
	{
		return mOwner;
	}
#endif
}
// --- META ---

DECLARE_CLASS(o2::DrawableComponent);
// --- END META ---
