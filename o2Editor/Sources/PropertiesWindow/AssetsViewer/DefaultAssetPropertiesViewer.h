#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "PropertiesWindow/AssetsViewer/IAssetPropertiesViewer.h"

namespace o2
{
	class UIVerticalLayout;
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
		void SetTargetAssets(const Vector<AssetRef*>& assets);

		// Returns viewing asset type 
		const Type* GetAssetType() const;

		// Returns data widget
		UIWidget* GetWidget() const;

		// Specialize viewing component type. Creates all using properties
		void SpecializeAssetType(const Type* type);

		IOBJECT(DefaultAssetPropertiesViewer);

	protected:
		UIVerticalLayout*   mPropertiesLayout;    // Properties layout
		Vector<AssetRef*>   mTargetAssets;        // Target assets
		FieldPropertiesInfo mFieldProperties;     // Field properties information
		const Type*         mAssetType = nullptr; // Target asset type
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
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mAssetType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultAssetPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetAssets, const Vector<AssetRef*>&);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SpecializeAssetType, const Type*);
}
END_META;
