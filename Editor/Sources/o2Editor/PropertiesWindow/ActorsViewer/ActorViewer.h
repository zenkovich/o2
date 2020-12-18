#pragma once

#include "o2Editor/PropertiesWindow/IPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Button;
	class VerticalLayout;
	class Widget;
}

namespace Editor
{
	class AddComponentPanel;
	class DefaultActorComponentViewer;
	class DefaultActorPropertiesViewer;
	class IActorComponentViewer;
	class IActorHeaderViewer;
	class IActorPropertiesViewer;
	class IActorTransformViewer;

	// ------------------------
	// Actors properties viewer
	// ------------------------
	class ActorViewer: public IPropertiesViewer
	{
	public:
		ActorViewer();

		// Virtual destructor
		~ActorViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const;

		// Sets header viewer
		void SetActorHeaderViewer(IActorHeaderViewer* viewer);

		// Sets transform viewer
		void SetActorTransformViewer(IActorTransformViewer* viewer);

		// Adds new available component viewer type
		void AddComponentViewerType(IActorComponentViewer* viewer);

		// Adds new available actor properties viewer type
		void AddActorPropertiesViewerType(IActorPropertiesViewer* viewer);

		// Updates properties values
		void Refresh() override;

		IOBJECT(ActorViewer);

	protected:
		typedef Map<const Type*, Vector<IActorComponentViewer*>> TypeCompViewersMap;
		typedef Map<const Type*, IActorPropertiesViewer*> TypeActorViewersmap;

		Vector<Actor*> mTargetActors; // Current target actors
									    
		IActorHeaderViewer*    mHeaderViewer = nullptr;    // Actor header viewer
		IActorTransformViewer* mTransformViewer = nullptr; // Actor transform viewer

		IActorPropertiesViewer*         mActorPropertiesViewer = nullptr;        // Actor properties viewer
		DefaultActorPropertiesViewer*   mDefaultActorPropertiesViewer = nullptr; // Default actor properties viewer sample
		Vector<IActorPropertiesViewer*> mAvailableActorPropertiesViewers;        // Available actor properties viewers										 							      
		TypeActorViewersmap             mActorPropertiesViewersPool;             // Actor properties viewers pool

		Vector<const Type*>            mCommonComponentsTypes;            // List of common components types for targets
		Vector<IActorComponentViewer*> mComponentsViewers;                // Components viewers
		DefaultActorComponentViewer*   mDefaultComponentViewer = nullptr; // Default component viewer sample
		Vector<IActorComponentViewer*> mAvailableComponentsViewers;       // Available components' viewers										 							      
		TypeCompViewersMap             mComponentViewersPool;             // Components viewers pool

		AddComponentPanel* mAddComponentPanel = nullptr; // Add component panel. Shown by clicking on filter field
									    
		VerticalLayout* mViewersLayout = nullptr; // Viewers layout

	protected:
		// It is called when some actors on scene were changed
		void OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects);

		// Sets target objects
		void SetTargets(const Vector<IObject*> targets) override;

		// Sets target actor properties
		void SetTargetsActorProperties(const Vector<IObject*> targets, Vector<Widget*>& viewersWidgets);

		// Sets target components: gets common components and initializes them
		void SetTargetsComponents(const Vector<IObject*> targets, Vector<Widget*>& viewersWidgets);

		// Returns list of common components types for targets
		Vector<const Type*> GetCommonComponentsTypes(const Vector<IObject*> targets) const;

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;

		friend class AddComponentPanel;
	};

}

CLASS_BASES_META(Editor::ActorViewer)
{
	BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::ActorViewer)
{
	FIELD().NAME(mTargetActors).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mHeaderViewer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTransformViewer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mActorPropertiesViewer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDefaultActorPropertiesViewer).PROTECTED();
	FIELD().NAME(mAvailableActorPropertiesViewers).PROTECTED();
	FIELD().NAME(mActorPropertiesViewersPool).PROTECTED();
	FIELD().NAME(mCommonComponentsTypes).PROTECTED();
	FIELD().NAME(mComponentsViewers).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDefaultComponentViewer).PROTECTED();
	FIELD().NAME(mAvailableComponentsViewers).PROTECTED();
	FIELD().NAME(mComponentViewersPool).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAddComponentPanel).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mViewersLayout).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::ActorViewer)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(void, SetActorHeaderViewer, IActorHeaderViewer*);
	PUBLIC_FUNCTION(void, SetActorTransformViewer, IActorTransformViewer*);
	PUBLIC_FUNCTION(void, AddComponentViewerType, IActorComponentViewer*);
	PUBLIC_FUNCTION(void, AddActorPropertiesViewerType, IActorPropertiesViewer*);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnSceneObjectsChanged, const Vector<SceneEditableObject*>&);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, SetTargetsActorProperties, const Vector<IObject*>, Vector<Widget*>&);
	PROTECTED_FUNCTION(void, SetTargetsComponents, const Vector<IObject*>, Vector<Widget*>&);
	PROTECTED_FUNCTION(Vector<const Type*>, GetCommonComponentsTypes, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
