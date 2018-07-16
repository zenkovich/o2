#pragma once

#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerHeadViewer.h"

namespace o2
{
	class UIToggle;
	class UIEditBox;
	class UIButton;
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
		void SetTargetLayers(const Vector<UIWidgetLayer*>& layers) override;

		// Returns data widget
		UIWidget* GetWidget() const override;

		// Updates properties values
		void Refresh();

		IOBJECT(DefaultWidgetLayerHeaderViewer);

	public:
		Vector<UIWidgetLayer*> mLayers;

		UIWidget*              mDataView = nullptr;

		BooleanProperty*       mEnableProperty = nullptr;
		StringProperty*        mNameProperty = nullptr;
		BooleanProperty*       mLockProperty = nullptr;

	protected:
		// It is called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue);
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
	PUBLIC_FIELD(mDataView);
	PUBLIC_FIELD(mEnableProperty);
	PUBLIC_FIELD(mNameProperty);
	PUBLIC_FIELD(mLockProperty);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerHeaderViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<UIWidgetLayer*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
