#pragma once

#include "Core/Properties/PropertiesContext.h"
#include "Core/Properties/IPropertyField.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerPropertiesViewer.h"

namespace o2
{
	class Button;
}

namespace Editor
{
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

		// Rebuilds properties layout
		void Rebuild() override;

		// Returns is there no properties
		bool IsEmpty() const override;

		// Returns true when properties was built with hidden fields
		bool IsBuiltWithHiddenFields() const override;

		IOBJECT(DefaultWidgetLayerPropertiesViewer);

	protected:
		Vector<WidgetLayer*>   mLayers;                  // Target widget layers
		PropertiesContext mFieldProperties;         // Field properties information
		Button*                mFitSizeButton = nullptr; // Fit size of layer by drawable size
		const Type*            mDrawableType = nullptr;  // Target actor type

	protected:
		// Fits layer size by drawable size, it is called when mFitSizeButton were pressed
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
	PROTECTED_FIELD(mLayers);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mFitSizeButton);
	PROTECTED_FIELD(mDrawableType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	PUBLIC_FUNCTION(const Type*, GetDrawableType);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(bool, IsBuiltWithHiddenFields);
	PROTECTED_FUNCTION(void, FitLayerByDrawable);
}
END_META;
