#pragma once

#include "Utils/Types/String.h"
#include "Utils/Serialization/Serializable.h"

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
		typedef Vector<Actor*> ActorsVec;
		typedef Vector<SceneDrawable*> DrawablesVec;

	public:
		String name; // Name of layer @SERIALIZABLE

	public:
		// Returns all actors in layer
		const ActorsVec& GetActors() const;

		// Returns enabled actors in layer
		const ActorsVec& GetEnabledActors() const;

		// Returns all drawable objects of actors in layer
		const DrawablesVec& GetDrawables() const;

		// Returns enabled drawable objects of actors in layer
		const DrawablesVec& GetEnabledDrawables() const;

		SERIALIZABLE(SceneLayer);

	protected:
		ActorsVec    mActors;           // Actors in layer
		ActorsVec    mEnabledActors;    // Enabled actors
		DrawablesVec mDrawables;        // Drawable objects in layer
		DrawablesVec mEnabledDrawables; // Enabled drawable objects in layer

	protected:
		// Registers drawable object
		void RegisterDrawable(SceneDrawable* drawable);

		// Unregisters drawable object
		void UnregisterDrawable(SceneDrawable* drawable);

		// It is called when drawable object depth was changed and sorts all drawable component
		void DrawableDepthChanged(SceneDrawable* drawable);

		// It is called when object was enabled
		void DrawableEnabled(SceneDrawable* drawable);

		// It is called when object was enabled
		void DrawableDisabled(SceneDrawable* drawable);

		// Sets drawable order as last of all objects with same depth
		void SetLastByDepth(SceneDrawable* drawable);

		friend class Actor;
		friend class DrawableComponent;
		friend class Scene;
		friend class SceneDrawable;
		friend class Widget;
	};

	typedef Vector<SceneLayer*> SceneLayersVec;

	// -------------------------
	// Layer data node converter
	// -------------------------
// 	class LayerDataNodeConverter : public IDataNodeTypeConverter
// 	{
// 	public:
// 		// Converts layer pointer to data 
// 		void ToData(void* object, DataNode& data);
// 
// 		// Gets layer pointer from data
// 		void FromData(void* object, const DataNode& data);
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
	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mActors);
	PROTECTED_FIELD(mEnabledActors);
	PROTECTED_FIELD(mDrawables);
	PROTECTED_FIELD(mEnabledDrawables);
}
END_META;
CLASS_METHODS_META(o2::SceneLayer)
{

	PUBLIC_FUNCTION(const ActorsVec&, GetActors);
	PUBLIC_FUNCTION(const ActorsVec&, GetEnabledActors);
	PUBLIC_FUNCTION(const DrawablesVec&, GetDrawables);
	PUBLIC_FUNCTION(const DrawablesVec&, GetEnabledDrawables);
	PROTECTED_FUNCTION(void, RegisterDrawable, SceneDrawable*);
	PROTECTED_FUNCTION(void, UnregisterDrawable, SceneDrawable*);
	PROTECTED_FUNCTION(void, DrawableDepthChanged, SceneDrawable*);
	PROTECTED_FUNCTION(void, DrawableEnabled, SceneDrawable*);
	PROTECTED_FUNCTION(void, DrawableDisabled, SceneDrawable*);
	PROTECTED_FUNCTION(void, SetLastByDepth, SceneDrawable*);
}
END_META;
