#pragma once

#include "o2/Scene/ISceneDrawable.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Actor;
	class ISceneDrawable;

	// --------------------------------------------------------------------------------
	// Scene layer. It contains Actors and their Drawable parts, managing sorting order
	// --------------------------------------------------------------------------------
	class SceneLayer: public ISerializable
	{
	public:
#if IS_EDITOR
		bool visible = true; // Is layer visible in editor
#endif

	public:
		// Default constructor
		SceneLayer();

		// Sets layer name
		void SetName(const String& name);

		// Returns layer name
		const String& GetName() const;

		// Returns all actors in layer
		const Vector<Actor*>& GetActors() const;

		// Returns enabled actors in layer
		const Vector<Actor*>& GetEnabledActors() const;

		// Returns all drawable objects of actors in layer
		const Vector<ISceneDrawable*>& GetDrawables() const;

		// Returns enabled drawable objects of actors in layer
		const Vector<ISceneDrawable*>& GetEnabledDrawables() const;

		SERIALIZABLE(SceneLayer);

	public:
		struct RootDrawablesContainer: public ISceneDrawable
		{
			Vector<ISceneDrawable*> drawables;

			void Draw() override;
		};

	protected:
		String mName; // Name of layer @SERIALIZABLE

		Vector<Actor*>  mActors;        // Actors in layer
		Vector<Actor*>  mEnabledActors; // Enabled actors

		Vector<ISceneDrawable*> mDrawables;        // Drawable objects in layer
		Vector<ISceneDrawable*> mEnabledDrawables; // Enabled drawable objects in layer

		RootDrawablesContainer mRootDrawables; // Root drawables with inherited depth. Draws at 0 priority

	protected:
		// Registers actor in list
		void RegisterActor(Actor* actor);

		// Unregisters actor in list
		void UnregisterActor(Actor* actor);

		// Called when actor has enabled
		void OnActorEnabled(Actor* actor);

		// Called when actor has disabled
		void OnActorDisabled(Actor* actor);

		// Registers drawable object
		void RegisterDrawable(ISceneDrawable* drawable);

		// Unregisters drawable object
		void UnregisterDrawable(ISceneDrawable* drawable);

		// Called when drawable object depth was changed and sorts all drawable component
		void OnDrawableDepthChanged(ISceneDrawable* drawable);

		// Called when object was enabled. If force is true, it is added to the scene
		void OnDrawableEnabled(ISceneDrawable* drawable, bool force);

		// Called when object was enabled. If force is true, it is removed from the scene
		void OnDrawableDisabled(ISceneDrawable* drawable, bool force);

		// Sets drawable order as last of all objects with same depth
		void SetLastByDepth(ISceneDrawable* drawable);

		friend class Actor;
		friend class CameraActor;
		friend class DrawableComponent;
		friend class Scene;
		friend class ISceneDrawable;
		friend class Widget;
	};

	// -------------------------
	// Layer data node converter
	// -------------------------
// 	class LayerDataValueConverter : public IDataValueTypeConverter
// 	{
// 	public:
// 		// Converts layer pointer to data 
// 		void ToData(void* object, DataValue& data);
// 
// 		// Gets layer pointer from data
// 		void FromData(void* object, const DataValue& data);
// 
// 		// Checks that type is layer's type
// 		bool IsConvertsType(const Type* type) const;
// 	};
}
// --- META ---

CLASS_BASES_META(o2::SceneLayer)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::SceneLayer)
{
#if  IS_EDITOR
	FIELD().PUBLIC().DEFAULT_VALUE(true).NAME(visible);
#endif
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mName);
	FIELD().PROTECTED().NAME(mActors);
	FIELD().PROTECTED().NAME(mEnabledActors);
	FIELD().PROTECTED().NAME(mDrawables);
	FIELD().PROTECTED().NAME(mEnabledDrawables);
	FIELD().PROTECTED().NAME(mRootDrawables);
}
END_META;
CLASS_METHODS_META(o2::SceneLayer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<Actor*>&, GetActors);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<Actor*>&, GetEnabledActors);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<ISceneDrawable*>&, GetDrawables);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<ISceneDrawable*>&, GetEnabledDrawables);
	FUNCTION().PROTECTED().SIGNATURE(void, RegisterActor, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, UnregisterActor, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnActorEnabled, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnActorDisabled, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, RegisterDrawable, ISceneDrawable*);
	FUNCTION().PROTECTED().SIGNATURE(void, UnregisterDrawable, ISceneDrawable*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDrawableDepthChanged, ISceneDrawable*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDrawableEnabled, ISceneDrawable*, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDrawableDisabled, ISceneDrawable*, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, SetLastByDepth, ISceneDrawable*);
}
END_META;
// --- END META ---
