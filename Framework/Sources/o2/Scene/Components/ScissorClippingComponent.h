#pragma once

#include "o2/Scene/DrawableComponent.h"

namespace o2
{
	// -----------------------------------------------------------------------------------------
	// Scissor clipping component. Clips children and inherited drawables by parent actor bounds
	// -----------------------------------------------------------------------------------------
	class ScissorClippingComponent : public DrawableComponent
	{
	public:
		bool enableClipping = true; // Is clipping enabled @SERIALIZABLE

	public:
		// Default constructor
		ScissorClippingComponent();

		// Destructor
		~ScissorClippingComponent();

		// Draws content of scene
		void Draw() override;

		SERIALIZABLE(ScissorClippingComponent);
	};
}
// --- META ---

CLASS_BASES_META(o2::ScissorClippingComponent)
{
	BASE_CLASS(o2::DrawableComponent);
}
END_META;
CLASS_FIELDS_META(o2::ScissorClippingComponent)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(enableClipping);
}
END_META;
CLASS_METHODS_META(o2::ScissorClippingComponent)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
}
END_META;
// --- END META ---
