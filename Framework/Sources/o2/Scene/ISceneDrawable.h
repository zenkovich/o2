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
	class ISceneDrawable: virtual public ISerializable, public IDrawable
	{
	public:
		PROPERTIES(ISceneDrawable);
		PROPERTY(float, drawDepth, SetDrawingDepth, GetSceneDrawableDepth); // Drawing depth property. Objects with higher depth will be drawn later

	public:
		// Default constructor
		ISceneDrawable();

		// Copy-constructor
		ISceneDrawable(const ISceneDrawable& other);

		// Destructor
		~ISceneDrawable();

		// Copy operator
		ISceneDrawable& operator=(const ISceneDrawable& other);

		// Draws content
		void Draw() override;

		// Sets drawing depth. Objects with higher depth will be drawn later
		virtual void SetDrawingDepth(float depth);

		// Returns drawing depth
		float GetSceneDrawableDepth() const;

		// Sets this drawable as last drawing object in layer with same depth
		void SetLastOnCurrentDepth();

		SERIALIZABLE(ISceneDrawable);

	protected:
		float mDrawingDepth = 0.0f; // Drawing depth. Objects with higher depth will be drawn later @SERIALIZABLE

	protected:
		// Returns current scene layer
		virtual SceneLayer* GetSceneDrawableSceneLayer() const = 0;

		// Returns is drawable enabled
		virtual bool IsSceneDrawableEnabled() const = 0;

		// Is is called when drawable has enabled
		void OnEnabled();

		// Is is called when drawable has enabled
		void OnDisabled();

		// It is called when actor was included to scene
		void OnAddToScene();

		// It is called when actor was excluded from scene
		void OnRemoveFromScene();

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

CLASS_BASES_META(o2::ISceneDrawable)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::IDrawable);
}
END_META;
CLASS_FIELDS_META(o2::ISceneDrawable)
{
	FIELD().NAME(drawDepth).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mDrawingDepth).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::ISceneDrawable)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetDrawingDepth, float);
	PUBLIC_FUNCTION(float, GetSceneDrawableDepth);
	PUBLIC_FUNCTION(void, SetLastOnCurrentDepth);
	PROTECTED_FUNCTION(SceneLayer*, GetSceneDrawableSceneLayer);
	PROTECTED_FUNCTION(bool, IsSceneDrawableEnabled);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableOwner);
	PUBLIC_FUNCTION(void, OnDrawn);
}
END_META;
