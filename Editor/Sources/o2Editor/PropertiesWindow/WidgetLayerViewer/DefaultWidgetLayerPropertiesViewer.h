#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2Editor/PropertiesWindow/WidgetLayerViewer/IWidgetLayerPropertiesViewer.h"

namespace o2
{
	class Button;
}

namespace Editor
{
	class IObjectPropertiesViewer;

	// --------------------------------------
	// Default widget layer properties viewer
	// -------------------------------
	class DefaultWidgetLayerPropertiesViewer : public IWidgetLayerPropertiesViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultWidgetLayerPropertiesViewer();

		// Virtual destructor
		~DefaultWidgetLayerPropertiesViewer();

		// Sets target actors
		void SetTargetLayers(const Vector<WidgetLayer*>& layers) override;

		// Returns viewing layer drawable type 
		const Type* GetDrawableType() const override;

		// Updates all actor values
		void Refresh() override;

		// Returns is there no properties
		bool IsEmpty() const override;

		IOBJECT(DefaultWidgetLayerPropertiesViewer);

	protected:
		Vector<WidgetLayer*>     mLayers;                  // Target widget layers
		IObjectPropertiesViewer* mViewer = nullptr;        // Properties viewer
		Button*                  mFitSizeButton = nullptr; // Fit size of layer by drawable size
		const Type*              mDrawableType = nullptr;  // Target actor type

	protected:
		// Fits layer size by drawable size, Called when mFitSizeButton were pressed
		void FitLayerByDrawable();
	};
}

CLASS_BASES_META(Editor::DefaultWidgetLayerPropertiesViewer)
{
	BASE_CLASS(Editor::IWidgetLayerPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultWidgetLayerPropertiesViewer)
{
	FIELD().PROTECTED().NAME(mLayers);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mViewer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mFitSizeButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDrawableType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerPropertiesViewer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetDrawableType);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEmpty);
	FUNCTION().PROTECTED().SIGNATURE(void, FitLayerByDrawable);
}
END_META;
