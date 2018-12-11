#pragma once

#include "Scene/Component.h"
#include "Scene/Drawable.h"

namespace o2
{
	// ------------------
	// Drawable component
	// ------------------
	class DrawableComponent: public Component, virtual public SceneDrawable
	{
	public:
		// Default constructor. Registers in scene as drawable object
		DrawableComponent();

		// Copy-constructor. Registers in scene as drawable object
		DrawableComponent(const DrawableComponent& other);

		// Copy operator
		DrawableComponent& operator=(const DrawableComponent& other);

		// Sets drawing depth. Components with higher depth will be drawn later
		void SetDrawingDepth(float depth) override;

		SERIALIZABLE(DrawableComponent);

	protected:
		using Component::mOwner;

		// Updates component enable
		void UpdateEnabled();

		// Sets owner actor
		void SetOwnerActor(Actor* actor) override;

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
	BASE_CLASS(o2::SceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::DrawableComponent)
{
}
END_META;
CLASS_METHODS_META(o2::DrawableComponent)
{

	PUBLIC_FUNCTION(void, SetDrawingDepth, float);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableOwner);
}
END_META;
