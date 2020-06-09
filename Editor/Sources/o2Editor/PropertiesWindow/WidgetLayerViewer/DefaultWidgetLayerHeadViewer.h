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
		void Refresh();

		IOBJECT(DefaultWidgetLayerHeaderViewer);

	public:
		Vector<WidgetLayer*> mLayers;

		Widget* mDataView = nullptr;

		BooleanProperty* mEnableProperty = nullptr;
		StringProperty*  mNameProperty = nullptr;
		BooleanProperty* mLockProperty = nullptr;

	protected:
		// It is called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue, 
							   const Vector<DataDocument>& newValue);
	};
}

CLASS_BASES_META(Editor::DefaultWidgetLayerHeaderViewer)
{
	BASE_CLASS(Editor::IWidgetLayerHeaderViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultWidgetLayerHeaderViewer)
{
	PUBLIC_FIELD(mLayers);
	PUBLIC_FIELD(mDataView).DEFAULT_VALUE(nullptr);
	PUBLIC_FIELD(mEnableProperty).DEFAULT_VALUE(nullptr);
	PUBLIC_FIELD(mNameProperty).DEFAULT_VALUE(nullptr);
	PUBLIC_FIELD(mLockProperty).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerHeaderViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
