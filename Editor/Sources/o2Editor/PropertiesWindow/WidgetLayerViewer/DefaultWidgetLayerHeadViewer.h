#pragma once

#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerHeadViewer.h"

namespace o2
{
	class Toggle;
	class EditBox;
	class Button;
}

namespace Editor
{
	using namespace o2;

	class BooleanProperty;
	class StringProperty;

	// ----------------------------------
	// Default widget layer header viewer
	// ----------------------------------
	class DefaultWidgetLayerHeaderViewer : public IWidgetLayerHeaderViewer
	{
	public:
		// Default constructor
		DefaultWidgetLayerHeaderViewer();

		// Virtual destructor
		~DefaultWidgetLayerHeaderViewer();

		// Sets target actors
		void SetTargetLayers(const Vector<WidgetLayer*>& layers) override;

		// Returns data widget
		Widget* GetWidget() const override;

		// Updates properties values
		void Refresh() override;

		IOBJECT(DefaultWidgetLayerHeaderViewer);

	public:
		Vector<WidgetLayer*> mLayers;

		Widget* mDataView = nullptr;

		BooleanProperty* mEnableProperty = nullptr;
		StringProperty*  mNameProperty = nullptr;
		BooleanProperty* mLockProperty = nullptr;

	protected:
		// Called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue, 
							   const Vector<DataDocument>& newValue);
	};
}
// --- META ---

CLASS_BASES_META(Editor::DefaultWidgetLayerHeaderViewer)
{
	BASE_CLASS(Editor::IWidgetLayerHeaderViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultWidgetLayerHeaderViewer)
{
	FIELD().PUBLIC().NAME(mLayers);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mDataView);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mEnableProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mNameProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mLockProperty);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerHeaderViewer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, GetWidget);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
// --- END META ---
