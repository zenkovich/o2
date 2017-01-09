#pragma once

#include "PropertiesWindow/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Actor;
	class UIVerticalLayout;
}

namespace Editor
{
	class IActorHeaderViewer;
	class IActorComponentViewer;
	class IActorTransformViewer;
	class IActorAnimationViewer;
	class DefaultActorComponentViewer;

	// ------------------------
	// Actors properties viewer
	// ------------------------
	class ActorPropertiesViewer: IObjectPropertiesViewer
	{
	public:
		ActorPropertiesViewer();

		// Virtual destructor
		~ActorPropertiesViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const;

		// Sets header viewer
		void SetActorHeaderViewer(IActorHeaderViewer* viewer);

		// Sets transform viewer
		void SetActorTransformViewer(IActorTransformViewer* viewer);

		// Sets animation viewer
		void SetActorAnimationViewer(IActorAnimationViewer* viewer);

		// Adds new available component viewer type
		void AddComponentViewerType(IActorComponentViewer* viewer);

		// Updates properties values
		void Refresh();

		IOBJECT(ActorPropertiesViewer);

	protected:
		typedef Dictionary<const Type*, Vector<IActorComponentViewer*>> TypeCompViewersDict;

		Vector<Actor*>                 mTargetActors;               // Current target actors

		IActorHeaderViewer*            mHeaderViewer;               // Actor header viewer
		IActorTransformViewer*         mTransformViewer;            // Actor transform viewer
		IActorAnimationViewer*         mAnimationViewer;            // Actor animation viewer
		Vector<IActorComponentViewer*> mComponentsViewers;          // Components viewers

		DefaultActorComponentViewer*   mDefaultComponentViewer;     // Default component viewer sample
		Vector<IActorComponentViewer*> mAvailableComponentsViewers; // Available components' viewers

		TypeCompViewersDict            mComponentViewersPool;       // Components viewers pool
									   
		UIVerticalLayout*              mViewersLayout;              // Viewers layout

	protected:
		// Sets target objects
		void SetTargets(const Vector<IObject*> targets);

		// Enable viewer event function
		void OnEnabled();

		// Disable viewer event function
		void OnDisabled();

		// Updates viewer
		void Update(float dt);

		// Draws something
		void Draw();
	};

}