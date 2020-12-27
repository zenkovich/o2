#pragma once

#include "o2/Scene/Component.h"
#include "o2/Scene/ISceneDrawable.h"

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

	protected:
		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Updates component enable
		void UpdateEnabled();

		// Returns current scene layer
		SceneLayer* GetSceneDrawableSceneLayer() const override;

		// Returns is drawable enabled
		bool IsSceneDrawableEnabled() const override;

		// It is called when actor was included to scene
		void OnAddToScene();

		// It is called when actor was excluded from scene
		void OnRemoveFromScene();

		friend class Scene;

#if IS_EDITOR
	public:
		// Returns pointer to owner editable object
		SceneEditableObject* GetEditableOwner() override;
#endif
	};
}

CLASS_BASES_META(o2::DrawableComponent)
{
	BASE_CLASS(o2::Component);
	BASE_CLASS(o2::ISceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::DrawableComponent)
{
}
END_META;
CLASS_METHODS_META(o2::DrawableComponent)
{

	PUBLIC_FUNCTION(void, SetDrawingDepth, float);
	PROTECTED_FUNCTION(void, OnSerialize, DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(SceneLayer*, GetSceneDrawableSceneLayer);
	PROTECTED_FUNCTION(bool, IsSceneDrawableEnabled);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableOwner);
}
END_META;
