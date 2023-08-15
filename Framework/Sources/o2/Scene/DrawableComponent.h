#pragma once

#include "o2/Scene/Component.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Scene/SceneLayerRef.h"

namespace o2
{
	// ------------------
	// Drawable component
	// ------------------
	class DrawableComponent: public Component, public ISceneDrawable
	{
	public:
		// Default constructor. Registers in scene as drawable object
		DrawableComponent();

		// Copy-constructor. Registers in scene as drawable object
		DrawableComponent(const DrawableComponent& other);

		// Copy operator
		DrawableComponent& operator=(const DrawableComponent& other);

		// Sets drawing depth. Objects with higher depth will be drawn later
		void SetDrawingDepth(float depth) override;

		SERIALIZABLE(DrawableComponent);

	protected:
		using Component::mOwner;

		Ref<SceneLayer> mLayer; // Layer reference @SERIALIZABLE

	protected:
		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Called when actor enabled in hierarchy
		void OnEnabled() override;

		// Called when actor disabled in hierarchy
		void OnDisabled() override;

		// Called when parent changed
		void OnParentChanged(Actor* oldParent) override;

		// Called when children list changed; updates inherited depth drawables list
		void OnChildrenChanged() override;

		// Called when actor was included to scene
		void OnAddToScene() override;

		// Called when actor was excluded from scene
		void OnRemoveFromScene() override;

		// Returns parent scene drawable
		ISceneDrawable* GetParentDrawable() override;

		// Returns the index in the parent's list of children, used to sort the rendering
		int GetIndexInParentDrawable() const override;

		// Returns current scene layer
		SceneLayer* GetSceneDrawableSceneLayer() const override;

		friend class Scene;

#if IS_EDITOR
	public:
		// Returns pointer to owner editable object
		SceneEditableObject* GetEditableOwner() override;
#endif
	};
}
// --- META ---

CLASS_BASES_META(o2::DrawableComponent)
{
	BASE_CLASS(o2::Component);
	BASE_CLASS(o2::ISceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::DrawableComponent)
{
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mLayer);
}
END_META;
CLASS_METHODS_META(o2::DrawableComponent)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const DrawableComponent&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDrawingDepth, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnParentChanged, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildrenChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(ISceneDrawable*, GetParentDrawable);
	FUNCTION().PROTECTED().SIGNATURE(int, GetIndexInParentDrawable);
	FUNCTION().PROTECTED().SIGNATURE(SceneLayer*, GetSceneDrawableSceneLayer);
#if  IS_EDITOR
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableOwner);
#endif
}
END_META;
// --- END META ---
