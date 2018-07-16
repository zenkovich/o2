#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "PropertiesWindow/WidgetLayerViewer/IWidgetLayerPropertiesViewer.h"

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
		void SetTargetLayers(const Vector<UIWidgetLayer*>& layers) override;

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
		Vector<UIWidgetLayer*> mLayers;                 // Target widget layers
		FieldPropertiesInfo    mFieldProperties;        // Field properties information
		bool                   mBuiltWithHidden;        // True when properties was built with hidden fields
		const Type*            mDrawableType = nullptr; // Target actor type
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
	PROTECTED_FIELD(mBuiltWithHidden);
	PROTECTED_FIELD(mDrawableType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultWidgetLayerPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetLayers, const Vector<UIWidgetLayer*>&);
	PUBLIC_FUNCTION(const Type*, GetDrawableType);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(bool, IsBuiltWithHiddenFields);
}
END_META;
