#pragma once

#include "o2Editor/PropertiesWindow/IPropertiesViewer.h"

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
		const Type* GetViewingObjectType() const;

		// Sets header viewer
		void SetHeaderViewer(IWidgetLayerHeaderViewer* viewer);

		// Sets transform viewer
		void SetLayoutViewer(IWidgetLayerLayoutViewer* viewer);

		// Adds new available actor properties viewer type
		void SetActorPropertiesViewer(IWidgetLayerPropertiesViewer* viewer);

		// Updates properties values
		void Refresh() override;

		IOBJECT(WidgetLayerViewer);

	protected:
		Vector<WidgetLayer*> mTargetLayers; // Current target layers

		IWidgetLayerHeaderViewer*     mHeaderViewer = nullptr;     // Layer header viewer
		IWidgetLayerLayoutViewer*     mLayoutViewer = nullptr;     // Layer layout viewer
		IWidgetLayerPropertiesViewer* mPropertiesViewer = nullptr; // Layer properties viewer

		VerticalLayout* mViewersLayout = nullptr; // Viewers layout

	protected:
		// It is called when some actors on scene were changed
		void OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects);

		// Sets target objects
		void SetTargets(const Vector<IObject*> targets) override;

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

CLASS_BASES_META(Editor::WidgetLayerViewer)
{
	BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::WidgetLayerViewer)
{
	FIELD().PROTECTED().NAME(mTargetLayers);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHeaderViewer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayoutViewer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPropertiesViewer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mViewersLayout);
}
END_META;
CLASS_METHODS_META(Editor::WidgetLayerViewer)
{

	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHeaderViewer, IWidgetLayerHeaderViewer*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayoutViewer, IWidgetLayerLayoutViewer*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetActorPropertiesViewer, IWidgetLayerPropertiesViewer*);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSceneObjectsChanged, const Vector<SceneEditableObject*>&);
	FUNCTION().PROTECTED().SIGNATURE(void, SetTargets, const Vector<IObject*>);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, Update, float);
	FUNCTION().PROTECTED().SIGNATURE(void, Draw);
}
END_META;
