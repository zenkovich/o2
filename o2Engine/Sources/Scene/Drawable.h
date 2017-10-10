#pragma once

#include "Render/IDrawable.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	class SceneLayer;

	// ------------------------------------------------------------------
	// Scene drawable object. Has virtual draw function and sorting depth
	// Depth shows how later object will be drawn
	// ------------------------------------------------------------------
	class SceneDrawable: virtual public ISerializable, public IDrawable
	{
	public:
		Property<float> drawDepth; // Drawing depth property. Objects with higher depth will be drawn later

	public:
		// Default constructor. Registers in scene as drawable
		SceneDrawable();

		// Copy-constructor. Registers in scene as drawable
		SceneDrawable(const SceneDrawable& other);

		// Destructor. Removes itself from scene drawable objects list
		~SceneDrawable();

		// Copy operator
		SceneDrawable& operator=(const SceneDrawable& other);

		// Draws content
		void Draw() override;

		// Sets drawing depth. Objects with higher depth will be drawn later
		virtual void SetDrawingDepth(float depth);

		// Returns drawing depth
		float GetDrawingDepth() const;

		// Sets layer
		void SetLayer(SceneLayer* layer);

		SERIALIZABLE(SceneDrawable);

	protected:
		SceneLayer* mLayer = nullptr;     // Scene layer, where this will be drawn
		float       mDrawingDepth = 0.0f; // Drawing depth. Objects with higher depth will be drawn later @SERIALIZABLE

	protected:
		// It is called when actor was excluded from scene
		void OnExcludeFromScene();

		// It is called when actor was included to scene
		void OnIncludeToScene();

		// Initializes property
		void InitializeProperties();

		friend class Scene;
		friend class SceneLayer;
	};
}
