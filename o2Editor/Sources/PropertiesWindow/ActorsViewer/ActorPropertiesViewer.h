#pragma once

#include "PropertiesWindow/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Actor;
	class UIVerticalLayout;
}

class IEditorActorHeaderViewer;
class IEditorActorComponentViewer;
class IEditorActorTransformViewer;
class IEditorActorAnimationViewer;
class DefaultEditorActorComponentViewer;

// ------------------------
// Actors properties viewer
// ------------------------
class EditorActorPropertiesViewer: IObjectPropertiesViewer
{
public:
	EditorActorPropertiesViewer();

	// Virtual destructor
	~EditorActorPropertiesViewer();

	// Returns viewing object type
	const Type* GetViewingObjectType() const;

	// Sets header viewer
	void SetActorHeaderViewer(IEditorActorHeaderViewer* viewer);

	// Sets transform viewer
	void SetActorTransformViewer(IEditorActorTransformViewer* viewer);

	// Sets animation viewer
	void SetActorAnimationViewer(IEditorActorAnimationViewer* viewer);

	// Adds new available component viewer type
	void AddComponentViewerType(IEditorActorComponentViewer* viewer);

	IOBJECT(EditorActorPropertiesViewer);

protected:
	typedef Dictionary<const Type*, Vector<IEditorActorComponentViewer*>> TypeCompViewersDict;

	Vector<Actor*>                       mTargetActors;               // Current target actors
										 
	IEditorActorHeaderViewer*            mHeaderViewer;               // Actor header viewer
	IEditorActorTransformViewer*         mTransformViewer;            // Actor transform viewer
	IEditorActorAnimationViewer*         mAnimationViewer;            // Actor animation viewer
	Vector<IEditorActorComponentViewer*> mComponentsViewers;          // Components viewers
										 
	DefaultEditorActorComponentViewer*   mDefaultComponentViewer;     // Default component viewer sample
	Vector<IEditorActorComponentViewer*> mAvailableComponentsViewers; // Available components' viewers

	TypeCompViewersDict                  mComponentViewersPool;       // Components viewers pool

	UIVerticalLayout*                    mViewersLayout;              // Viewers layout

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
