#pragma once

#include "o2/Scene/Component.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Scene/SceneLayerRef.h"

namespace o2
{
	// ------------------
	// Drawable component
	// ------------------
	class DrawableComponent: public Component
	{
	public:
		// Draws component
		virtual void Draw() {}

		SERIALIZABLE(DrawableComponent);

	protected:
		using Component::mOwner;

		friend class Scene;
	};
}
// --- META ---

CLASS_BASES_META(o2::DrawableComponent)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::DrawableComponent)
{
}
END_META;
CLASS_METHODS_META(o2::DrawableComponent)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
}
END_META;
// --- END META ---
