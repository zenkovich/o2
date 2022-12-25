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
		PROPERTY(float, drawDepth, SetDrawingDepth, GetDrawingDepth); // Drawing depth property. Objects with higher depth will be drawn later @EDITOR_IGNORE

	public:
		// Default constructor
		ISceneDrawable();

		// Copy-constructor
		ISceneDrawable(const ISceneDrawable& other);

		// Destructor
		~ISceneDrawable() override;

		// Copy operator
		ISceneDrawable& operator=(const ISceneDrawable& other);

		// Draws content
		void Draw() override;

		// Sets drawing depth. Objects with higher depth will be drawn later
		virtual void SetDrawingDepth(float depth);

		// Returns drawing depth
		float GetDrawingDepth() const;

		// Sets that depth is inherited from the parent
		void SetDrawingDepthInheritFromParent(bool inherit);

		// Returns whether the depth is inherited from the parent
		bool IsDrawingDepthInheritedFromParent() const;

		// Sets this drawable as last drawing object in layer with same depth
		void SetLastOnCurrentDepth();

		SERIALIZABLE(ISceneDrawable);

	protected:
		ISceneDrawable* mParentDrawable = nullptr; // Parent drawable

		float mDrawingDepth = 0.0f;                  // Drawing depth. Objects with higher depth will be drawn later @SERIALIZABLE
		bool  mInheritDrawingDepthFromParent = true; // If parent depth is used @SERIALIZABLE

		Vector<ISceneDrawable*> mChildrenInheritedDepth; // List of children who inherited depth

	protected:
		// Returns current scene layer
		virtual SceneLayer* GetSceneDrawableSceneLayer() const { return nullptr; }

		// Returns is drawable enabled
		virtual bool IsSceneDrawableEnabled() const { return false; }

		// Returns parent scene drawable
		virtual ISceneDrawable* GetParentDrawable() { return nullptr; }

		// Returns the index in the parent's list of children, used to sort the rendering
		virtual int GetIndexInParentDrawable() const { return 0; }

		// Called when the parent changes
		virtual void OnDrawbleParentChanged();

		// Sorts depth-inheriting drawables
		void SortInheritedDrawables();

		// Called when drawable has enabled
		void OnEnabled();

		// Called when drawable has enabled
		void OnDisabled();

		// Called when actor was included to scene
		void OnAddToScene(bool force = false);

		// Called when actor was excluded from scene
		void OnRemoveFromScene(bool force = false);

		friend class Scene;
		friend class SceneLayer;

#if IS_EDITOR
	public:
		// Returns pointer to owner editable object
		virtual SceneEditableObject* GetEditableOwner();

		// Called when drawable was drawn. Storing render scissor rect, calling onDraw callback, adding in drawnEditableObjects
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
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(drawDepth);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mParentDrawable);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mDrawingDepth);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mInheritDrawingDepthFromParent);
	FIELD().PROTECTED().NAME(mChildrenInheritedDepth);
}
END_META;
CLASS_METHODS_META(o2::ISceneDrawable)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ISceneDrawable&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDrawingDepth, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetDrawingDepth);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDrawingDepthInheritFromParent, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsDrawingDepthInheritedFromParent);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLastOnCurrentDepth);
	FUNCTION().PROTECTED().SIGNATURE(SceneLayer*, GetSceneDrawableSceneLayer);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsSceneDrawableEnabled);
	FUNCTION().PROTECTED().SIGNATURE(ISceneDrawable*, GetParentDrawable);
	FUNCTION().PROTECTED().SIGNATURE(int, GetIndexInParentDrawable);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDrawbleParentChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, SortInheritedDrawables);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene, bool);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableOwner);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDrawn);
}
END_META;
