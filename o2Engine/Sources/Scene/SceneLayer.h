#pragma once

#include "Utils/String.h"
#include "Utils/Serializable.h"

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

		friend class Actor;
		friend class DrawableComponent;
		friend class Scene;
		friend class SceneDrawable;
		friend class UIWidget;
	};

	typedef Vector<SceneLayer*> LayersVec;
}
