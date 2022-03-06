#pragma once

#include "o2/Utils/Types/String.h"
#include "o2/Utils/Serialization/Serializable.h"

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

	protected:
		String mName; // Name of layer @SERIALIZABLE

		Vector<Actor*>  mActors;        // Actors in layer
		Vector<Actor*>  mEnabledActors; // Enabled actors

		Vector<ISceneDrawable*> mDrawables;        // Drawable objects in layer
		Vector<ISceneDrawable*> mEnabledDrawables; // Enabled drawable objects in layer

	protected:
		// Registers actor in list
		void RegisterActor(Actor* actor);

		// Unregisters actor in list
		void UnregisterActor(Actor* actor);

		// Is is called when actor has enabled
		void OnActorEnabled(Actor* actor);

		// Is is called when actor has disabled
		void OnActorDisabled(Actor* actor);

		// Registers drawable object
		void RegisterDrawable(ISceneDrawable* drawable);

		// Unregisters drawable object
		void UnregisterDrawable(ISceneDrawable* drawable);

		// It is called when drawable object depth was changed and sorts all drawable component
		void OnDrawableDepthChanged(ISceneDrawable* drawable);

		// It is called when object was enabled
		void OnDrawableEnabled(ISceneDrawable* drawable);

		// It is called when object was enabled
		void OnDrawableDisabled(ISceneDrawable* drawable);

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

CLASS_BASES_META(o2::SceneLayer)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::SceneLayer)
{
	FIELD().PUBLIC().DEFAULT_VALUE(true).NAME(visible);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mName);
	FIELD().PROTECTED().NAME(mActors);
	FIELD().PROTECTED().NAME(mEnabledActors);
	FIELD().PROTECTED().NAME(mDrawables);
	FIELD().PROTECTED().NAME(mEnabledDrawables);
}
END_META;
CLASS_METHODS_META(o2::SceneLayer)
{

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
	FUNCTION().PROTECTED().SIGNATURE(void, OnDrawableEnabled, ISceneDrawable*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDrawableDisabled, ISceneDrawable*);
	FUNCTION().PROTECTED().SIGNATURE(void, SetLastByDepth, ISceneDrawable*);
}
END_META;
