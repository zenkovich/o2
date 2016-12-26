#pragma once

#include "PropertiesWindow/AssetsViewer/IAssetPropertiesViewer.h"
#include "PropertiesWindow/Properties/FieldPropertiesInfo.h"
#include "PropertiesWindow/Properties/IPropertyField.h"

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
		void SetTargetAssets(const Vector<Asset*>& assets);

		// Returns viewing asset type 
		const Type* GetAssetType() const;

		// Returns data widget
		UIWidget* GetWidget() const;

		// Specialize viewing component type. Creates all using properties
		void SpecializeAssetType(const Type* type);

		IOBJECT(DefaultAssetPropertiesViewer);

	protected:
		UIVerticalLayout*   mPropertiesLayout;    // Properties layout
		Vector<Asset*>      mTargetAssets;        // Target assets
		FieldPropertiesInfo mFieldProperties;     // Field properties information
		const Type*         mAssetType = nullptr; // Target asset type
	};
}
