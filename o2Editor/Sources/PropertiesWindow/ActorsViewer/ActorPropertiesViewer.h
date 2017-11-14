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

		Vector<Actor*>                 mTargetActors;                     // Current target actors

		IActorHeaderViewer*            mHeaderViewer = nullptr;           // Actor header viewer
		IActorTransformViewer*         mTransformViewer = nullptr;        // Actor transform viewer
		IActorAnimationViewer*         mAnimationViewer = nullptr;        // Actor animation viewer
		Vector<IActorComponentViewer*> mComponentsViewers;                // Components viewers

		DefaultActorComponentViewer*   mDefaultComponentViewer = nullptr; // Default component viewer sample
		Vector<IActorComponentViewer*> mAvailableComponentsViewers;       // Available components' viewers
																	      
		TypeCompViewersDict            mComponentViewersPool;             // Components viewers pool
									   
		UIVerticalLayout*              mViewersLayout = nullptr;          // Viewers layout

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

CLASS_BASES_META(Editor::ActorPropertiesViewer)
{
	BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::ActorPropertiesViewer)
{
	PROTECTED_FIELD(mTargetActors);
	PROTECTED_FIELD(mHeaderViewer);
	PROTECTED_FIELD(mTransformViewer);
	PROTECTED_FIELD(mAnimationViewer);
	PROTECTED_FIELD(mComponentsViewers);
	PROTECTED_FIELD(mDefaultComponentViewer);
	PROTECTED_FIELD(mAvailableComponentsViewers);
	PROTECTED_FIELD(mComponentViewersPool);
	PROTECTED_FIELD(mViewersLayout);
}
END_META;
CLASS_METHODS_META(Editor::ActorPropertiesViewer)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(void, SetActorHeaderViewer, IActorHeaderViewer*);
	PUBLIC_FUNCTION(void, SetActorTransformViewer, IActorTransformViewer*);
	PUBLIC_FUNCTION(void, SetActorAnimationViewer, IActorAnimationViewer*);
	PUBLIC_FUNCTION(void, AddComponentViewerType, IActorComponentViewer*);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, Draw);
}
END_META;
