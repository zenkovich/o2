#pragma once

#include "o2/Utils/Types/String.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Actor;
	class SceneDrawable;

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
		const Vector<SceneDrawable*>& GetDrawables() const;

		// Returns enabled drawable objects of actors in layer
		const Vector<SceneDrawable*>& GetEnabledDrawables() const;

		SERIALIZABLE(SceneLayer);

	protected:
		String mName; // Name of layer @SERIALIZABLE

		Vector<Actor*>  mActors;        // Actors in layer
		Vector<Actor*>  mEnabledActors; // Enabled actors

		Vector<SceneDrawable*> mDrawables;        // Drawable objects in layer
		Vector<SceneDrawable*> mEnabledDrawables; // Enabled drawable objects in layer

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
		void RegisterDrawable(SceneDrawable* drawable);

		// Unregisters drawable object
		void UnregisterDrawable(SceneDrawable* drawable);

		// It is called when drawable object depth was changed and sorts all drawable component
		void OnDrawableDepthChanged(SceneDrawable* drawable);

		// It is called when object was enabled
		void OnDrawableEnabled(SceneDrawable* drawable);

		// It is called when object was enabled
		void OnDrawableDisabled(SceneDrawable* drawable);

		// Sets drawable order as last of all objects with same depth
		void SetLastByDepth(SceneDrawable* drawable);

		friend class Actor;
		friend class CameraActor;
		friend class DrawableComponent;
		friend class Scene;
		friend class SceneDrawable;
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
	PUBLIC_FIELD(visible).DEFAULT_VALUE(true);
	PROTECTED_FIELD(mName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mActors);
	PROTECTED_FIELD(mEnabledActors);
	PROTECTED_FIELD(mDrawables);
	PROTECTED_FIELD(mEnabledDrawables);
}
END_META;
CLASS_METHODS_META(o2::SceneLayer)
{

	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(const String&, GetName);
	PUBLIC_FUNCTION(const Vector<Actor*>&, GetActors);
	PUBLIC_FUNCTION(const Vector<Actor*>&, GetEnabledActors);
	PUBLIC_FUNCTION(const Vector<SceneDrawable*>&, GetDrawables);
	PUBLIC_FUNCTION(const Vector<SceneDrawable*>&, GetEnabledDrawables);
	PROTECTED_FUNCTION(void, RegisterActor, Actor*);
	PROTECTED_FUNCTION(void, UnregisterActor, Actor*);
	PROTECTED_FUNCTION(void, OnActorEnabled, Actor*);
	PROTECTED_FUNCTION(void, OnActorDisabled, Actor*);
	PROTECTED_FUNCTION(void, RegisterDrawable, SceneDrawable*);
	PROTECTED_FUNCTION(void, UnregisterDrawable, SceneDrawable*);
	PROTECTED_FUNCTION(void, OnDrawableDepthChanged, SceneDrawable*);
	PROTECTED_FUNCTION(void, OnDrawableEnabled, SceneDrawable*);
	PROTECTED_FUNCTION(void, OnDrawableDisabled, SceneDrawable*);
	PROTECTED_FUNCTION(void, SetLastByDepth, SceneDrawable*);
}
END_META;
