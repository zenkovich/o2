#pragma once

#include "o2Editor/PropertiesWindow/IPropertiesViewer.h"
#include "Ref.h"
#include "WeakRef.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
	class VerticalLayout;
	class WidgetLayer;
}

namespace Editor
{
	class IWidgetLayerHeaderViewer;
	class IWidgetLayerLayoutViewer;
	class IWidgetLayerPropertiesViewer;

	// ------------------------------
	// Widget layer properties viewer
	// ------------------------------
	class WidgetLayerViewer : public IPropertiesViewer
	{
	public:
		WidgetLayerViewer();

		// Virtual destructor
		~WidgetLayerViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const override;

		// Sets header viewer
		void SetHeaderViewer(const Ref<IWidgetLayerHeaderViewer>& viewer);

		// Sets transform viewer
		void SetLayoutViewer(const Ref<IWidgetLayerLayoutViewer>& viewer);

		// Adds new available actor properties viewer type
		void SetActorPropertiesViewer(const Ref<IWidgetLayerPropertiesViewer>& viewer);

		// Updates properties values
		void Refresh() override;

		IOBJECT(WidgetLayerViewer);

	protected:
		Vector<Ref<WidgetLayer>> mTargetLayers; // Current target layers

		Ref<IWidgetLayerHeaderViewer>     mHeaderViewer;     // Layer header viewer
		Ref<IWidgetLayerLayoutViewer>     mLayoutViewer;     // Layer layout viewer
		Ref<IWidgetLayerPropertiesViewer> mPropertiesViewer; // Layer properties viewer

		WeakRef<VerticalLayout> mViewersLayout; // Viewers layout

	protected:
		// Called when some actors on scene were changed
		void OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects);

		// Sets target objects
		void SetTargets(const Vector<Ref<IObject>>& targets) override;

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;

		// Updates viewer
		void Update(float dt) override;

		// Draws something
		void Draw() override;
	};

}

// --- META ---

CLASS_BASES_META(Editor::WidgetLayerViewer)
{
    BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::WidgetLayerViewer)
{
    FIELD().PROTECTED().NAME(mTargetLayers);
    FIELD().PROTECTED().NAME(mHeaderViewer);
    FIELD().PROTECTED().NAME(mLayoutViewer);
    FIELD().PROTECTED().NAME(mPropertiesViewer);
    FIELD().PROTECTED().NAME(mViewersLayout);
}
END_META;
CLASS_METHODS_META(Editor::WidgetLayerViewer)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE(void, SetHeaderViewer, const Ref<IWidgetLayerHeaderViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLayoutViewer, const Ref<IWidgetLayerLayoutViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetActorPropertiesViewer, const Ref<IWidgetLayerPropertiesViewer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSceneObjectsChanged, const Vector<SceneEditableObject*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetTargets, const Vector<Ref<IObject>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, Update, float);
    FUNCTION().PROTECTED().SIGNATURE(void, Draw);
}
END_META;
// --- END META ---

Note: I replaced raw pointers with Ref<> and used const Ref<>& for function arguments with pointer types. I removed the default nullptr value for class members with type Ref<>. I used WeakRef<> for class variables that are some kind of "parent". I used mmake<>() to create Ref<>, similar to make_shared<>(). I replaced dynamic_cast<type*> with DynamicCast<type> to cast Ref<>. However, I couldn't find any void* pointers in the given code, so I couldn't make any changes related to that.