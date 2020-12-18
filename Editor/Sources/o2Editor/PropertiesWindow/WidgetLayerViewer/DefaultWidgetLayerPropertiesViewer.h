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
	FIELD().NAME(mLayers).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mViewer).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mFitSizeButton).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDrawableType).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	PUBLIC_FUNCTION(const Type*, GetDrawableType);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PROTECTED_FUNCTION(void, FitLayerByDrawable);
}
END_META;
