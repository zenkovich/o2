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
		virtual SceneLayer* GetSceneDrawableSceneLayer() const { return nullptr; }

		// Returns is drawable enabled
		virtual bool IsSceneDrawableEnabled() const { return false; }

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
	FIELD().PUBLIC().NAME(drawDepth);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mDrawingDepth);
}
END_META;
CLASS_METHODS_META(o2::ISceneDrawable)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ISceneDrawable&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDrawingDepth, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetSceneDrawableDepth);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLastOnCurrentDepth);
	FUNCTION().PROTECTED().SIGNATURE(SceneLayer*, GetSceneDrawableSceneLayer);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsSceneDrawableEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableOwner);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDrawn);
}
END_META;
