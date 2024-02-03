replace raw pointers with Ref<>, except void* pointers.

#include "o2Editor/PropertiesWindow/IPropertiesViewer.h"
#include <type_traits>

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

	class ActorViewer : public IPropertiesViewer
	{
	public:
		ActorViewer();

		// Virtual destructor
		~ActorViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const override;

		// Sets header viewer
		void SetActorHeaderViewer(const Ref<IActorHeaderViewer>& viewer);

		// Sets transform viewer
		void SetActorTransformViewer(const Ref<IActorTransformViewer>& viewer);

		// Adds new available component viewer type
		void AddComponentViewerType(const Ref<IActorComponentViewer>& viewer);

		// Adds new available actor properties viewer type
		void AddActorPropertiesViewerType(const Ref<IActorPropertiesViewer>& viewer);

		// Updates properties values
		void Refresh() override;

		IOBJECT(ActorViewer);

	protected:
		typedef Map<const Type*, Vector<Ref<IActorComponentViewer>>> TypeCompViewersMap;
		typedef Map<const Type*, Ref<IActorPropertiesViewer>> TypeActorViewersmap;

		Vector<Ref<Actor>> mTargetActors; // Current target actors

		Ref<IActorHeaderViewer> mHeaderViewer; // Actor header viewer
		Ref<IActorTransformViewer> mTransformViewer; // Actor transform viewer

		Ref<IActorPropertiesViewer> mActorPropertiesViewer; // Actor properties viewer
		Ref<DefaultActorPropertiesViewer> mDefaultActorPropertiesViewer; // Default actor properties viewer sample
		Vector<Ref<IActorPropertiesViewer>> mAvailableActorPropertiesViewers; // Available actor properties viewers
		TypeActorViewersmap mActorPropertiesViewersPool; // Actor properties viewers pool

		Vector<Pair<const Type*, Vector<Component*>>> mComponentGroupsTypes; // List of components group types for targets
		Vector<Ref<IActorComponentViewer>> mComponentsViewers; // Components viewers
		Ref<DefaultActorComponentViewer> mDefaultComponentViewer; // Default component viewer sample
		Vector<Ref<IActorComponentViewer>> mAvailableComponentsViewers; // Available components' viewers
		TypeCompViewersMap mComponentViewersPool; // Components viewers pool

		Ref<AddComponentPanel> mAddComponentPanel; // Add component panel. Shown by clicking on filter field

		Ref<VerticalLayout> mViewersLayout; // Viewers layout

	protected:
		// Called when some actors on scene were changed
		void OnSceneObjectsChanged(const Vector<Ref<SceneEditableObject>>& objects);

		// Sets target objects
		void SetTargets(const Vector<Ref<IObject>>& targets) override;

		// Sets target actor properties
		void SetTargetsActorProperties(const Vector<Ref<IObject>>& targets, Vector<Ref<Widget>>& viewersWidgets);

		// Sets target components: gets common components and initializes them
		void SetTargetsComponents(const Vector<Ref<IObject>>& targets, Vector<Ref<Widget>>& viewersWidgets);

		// Returns list of grouped by types components
		Vector<Pair<const Type*, Vector<Component*>>> GetGroupedComponents() const;

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;

		friend class AddComponentPanel;
	};
}

// --- META ---

CLASS_BASES_META(Editor::ActorViewer)
{
    BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;

CLASS_FIELDS_META(Editor::ActorViewer)
{
    FIELD().PROTECTED().NAME(mTargetActors),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHeaderViewer),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTransformViewer),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mActorPropertiesViewer),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDefaultActorPropertiesViewer),
    FIELD().PROTECTED().NAME(mAvailableActorPropertiesViewers),
    FIELD().PROTECTED().NAME(mActorPropertiesViewersPool),
    FIELD().PROTECTED().NAME(mComponentGroupsTypes),
    FIELD().PROTECTED().NAME(mComponentsViewers),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDefaultComponentViewer),
    FIELD().PROTECTED().NAME(mAvailableComponentsViewers),
    FIELD().PROTECTED().NAME(mComponentViewersPool),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAddComponentPanel),
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mViewersLayout),
}
END_META;

CLASS_METHODS_META(Editor::ActorViewer)
{
    typedef Vector<Pair<const Type*, Vector<Component*>>> _tmp1;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargets, const Vector<Ref<IObject>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetActorHeaderViewer, const Ref<IActorHeaderViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetActorTransformViewer, const Ref<IActorTransformViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddComponentViewerType, const Ref<IActorComponentViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddActorPropertiesViewerType, const Ref<IActorPropertiesViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSceneObjectsChanged, const Vector<Ref<SceneEditableObject>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetTargetsActorProperties, const Vector<Ref<IObject>>&, Vector<Ref<Widget>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetTargetsComponents, const Vector<Ref<IObject>>&, Vector<Ref<Widget>>&);
    FUNCTION().PROTECTED().SIGNATURE(Vector<Pair<const Type*, Vector<Component*>>>, GetGroupedComponents) const;
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
}
END_META;

template <class T>
Ref<T> DynamicCast(const Ref<IObject>& object)
{
    return Ref<T>(static_cast<T*>(object.GetPtr()), false);
}Ref<ActorHeaderViewer> m_headerViewer;
Ref<IActorTransformViewer> m_transformViewer;
Vector<Ref<IActorComponentViewer>> m_componentViewers;
Vector<Ref<IActorPropertiesViewer>> m_propertiesViewers;
Vector<SceneEditableObject*>* m_sceneObjects;
Vector<Ref<IObject>>* m_targets;
Vector<Widget*>* m_targetsActorProperties;
Vector<Widget*>* m_targetsComponents;
Ref<_tmp1> m_groupedComponents;
bool m_enabled;

void SetActorHeaderViewer(const Ref<ActorHeaderViewer>& viewer);
void SetActorTransformViewer(const Ref<IActorTransformViewer>& viewer);
void AddComponentViewerType(const Ref<IActorComponentViewer>& type);
void AddActorPropertiesViewerType(const Ref<IActorPropertiesViewer>& type);
void Refresh();
void OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects);
void SetTargets(const Vector<Ref<IObject>>& targets);
void SetTargetsActorProperties(const Vector<Ref<IObject>>& targets, Vector<Widget*>& widgets);
void SetTargetsComponents(const Vector<Ref<IObject>>& targets, Vector<Widget*>& widgets);
_tmp1 GetGroupedComponents();
void OnEnabled();
void OnDisabled();