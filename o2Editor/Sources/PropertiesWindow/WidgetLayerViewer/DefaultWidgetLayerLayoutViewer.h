#pragma once

#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerLayoutViewer.h"

namespace Editor
{
	class Vec2FProperty;
	class FloatProperty;

	// --------------------------------------------
	// Default editor widget layer transform viewer
	// --------------------------------------------
	class DefaultWidgetLayerLayoutViewer : public IWidgetLayerLayoutViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultWidgetLayerLayoutViewer();

		// Virtual destructor
		~DefaultWidgetLayerLayoutViewer();

		// Sets target actors
		void SetTargetLayers(const Vector<UIWidgetLayer*>& layers) override;

		// Updates properties values
		void Refresh();

		IOBJECT(DefaultWidgetLayerLayoutViewer);

	protected:
		Vector<UIWidgetLayer*> mLayers;

		Vec2FProperty* mAnchorRightTopProperty = nullptr;
		Vec2FProperty* mAnchorLeftBottomProperty = nullptr;
		Vec2FProperty* mOffsetRightTopProperty = nullptr;
		Vec2FProperty* mOffsetLeftBottomProperty = nullptr;

	protected:
		// It is called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue);
	};
}

CLASS_BASES_META(Editor::DefaultWidgetLayerLayoutViewer)
{
	BASE_CLASS(Editor::IWidgetLayerLayoutViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultWidgetLayerLayoutViewer)
{
	PROTECTED_FIELD(mLayers);
	PROTECTED_FIELD(mAnchorRightTopProperty);
	PROTECTED_FIELD(mAnchorLeftBottomProperty);
	PROTECTED_FIELD(mOffsetRightTopProperty);
	PROTECTED_FIELD(mOffsetLeftBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerLayoutViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<UIWidgetLayer*>&);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
