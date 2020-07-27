#pragma once

#include "o2/Render/IDrawable.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"

#if IS_EDITOR
#include "o2/Utils/Editor/SceneEditableObject.h"
#endif

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
		PROPERTIES(SceneDrawable);
		PROPERTY(float, drawDepth, SetDrawingDepth, GetDrawingDepth); // Drawing depth property. Objects with higher depth will be drawn later

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

		// Sets this drawable as last drawing object in layer with same depth
		void SetLastOnCurrentDepth();

		// Sets layer
		void SetLayer(SceneLayer* layer);

		SERIALIZABLE(SceneDrawable);

	protected:
		SceneLayer* mLayer = nullptr;     // Scene layer, where this will be drawn 
		bool        mIsOnScene = false;   // Is drawable on scene
		float       mDrawingDepth = 0.0f; // Drawing depth. Objects with higher depth will be drawn later @SERIALIZABLE

	protected:
		// It is called when actor was excluded from scene
		void OnExcludeFromScene();

		// It is called when actor was included to scene
		void OnIncludeToScene();

		friend class Scene;
		friend class SceneLayer;

#if IS_EDITOR
	public:
		// Returns pointer to owner editable object
		virtual SceneEditableObject* GetEditableOwner();

		// It is called when drawable was drawn. Storing render scissor rect, calling onDraw callback, adding in drawnEditableObjects
		void OnDrawn() override;
#endif
	};
}

CLASS_BASES_META(o2::SceneDrawable)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::IDrawable);
}
END_META;
CLASS_FIELDS_META(o2::SceneDrawable)
{
	PUBLIC_FIELD(drawDepth);
	PROTECTED_FIELD(mLayer).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mIsOnScene).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mDrawingDepth).DEFAULT_VALUE(0.0f).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::SceneDrawable)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetDrawingDepth, float);
	PUBLIC_FUNCTION(float, GetDrawingDepth);
	PUBLIC_FUNCTION(void, SetLastOnCurrentDepth);
	PUBLIC_FUNCTION(void, SetLayer, SceneLayer*);
	PROTECTED_FUNCTION(void, OnExcludeFromScene);
	PROTECTED_FUNCTION(void, OnIncludeToScene);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableOwner);
	PUBLIC_FUNCTION(void, OnDrawn);
}
END_META;
