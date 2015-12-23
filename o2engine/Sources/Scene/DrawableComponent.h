#pragma once

#include "Scene/Component.h"

namespace o2
{
	// ------------------
	// Drawable component
	// ------------------
	class DrawableComponent: public Component
	{
	public:
		Property<float> drawDepth; // Drawing depth property. Components with higher depth will be drawn later

		// Default constructor. Registers in scene as drawable
		DrawableComponent();

		// Copy-constructor. Registers in scene as drawable
		DrawableComponent(const DrawableComponent& other);

		// Destructor. Removes itself from scene drawables list
		~DrawableComponent();

		// Copy operator
		DrawableComponent& operator=(const DrawableComponent& other);

		// Draws component content
		virtual void Draw();

		// Sets drawing depth. Components with higher depth will be drawn later
		void SetDrawingDepth(float depth);

		// Returns drawing depth
		float GetDrawingDepth() const;

		SERIALIZABLE(DrawableComponent);

	protected:
		float mDrawingDepth; // Drawing depth. Components with higher depth will be drawn later

	protected:
		// Initializes property
		void InitializeProperties();

		friend class Scene;
	};
}