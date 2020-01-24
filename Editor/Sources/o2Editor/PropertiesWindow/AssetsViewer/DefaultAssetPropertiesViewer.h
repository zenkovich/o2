#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2Editor/PropertiesWindow/AssetsViewer/IAssetPropertiesViewer.h"

namespace o2
{
	class VerticalLayout;
}

namespace Editor
{
	// -------------------------------------------------------------
	// Default asset properties viewer. Shows properties as raw list
	// -------------------------------------------------------------
	class DefaultAssetPropertiesViewer: public IAssetPropertiesViewer
	{
	public:
		// Default constructor
		DefaultAssetPropertiesViewer();

		// Virtual destructor
		~DefaultAssetPropertiesViewer();

		// Sets target actors
		void SetTargetAssets(const Vector<IAssetRef*>& assets);

		// Returns viewing asset type 
		const Type* GetAssetType() const;

		// Returns data widget
		Widget* GetWidget() const;

		// Specialize viewing component type. Creates all using properties
		void SpecializeAssetType(const Type* type);

		IOBJECT(DefaultAssetPropertiesViewer);

	protected:
		VerticalLayout*   mPropertiesLayout;    // Properties layout
		Vector<IAssetRef*> mTargetAssets;        // Target assets
		PropertiesContext mPropertiesContext;   // Field properties information
		const Type*       mAssetType = nullptr; // Target asset type
	};
}

CLASS_BASES_META(Editor::DefaultAssetPropertiesViewer)
{
	BASE_CLASS(Editor::IAssetPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultAssetPropertiesViewer)
{
	PROTECTED_FIELD(mPropertiesLayout);
	PROTECTED_FIELD(mTargetAssets);
	PROTECTED_FIELD(mPropertiesContext);
	PROTECTED_FIELD(mAssetType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultAssetPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetAssets, const Vector<IAssetRef*>&);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, SpecializeAssetType, const Type*);
}
END_META;
