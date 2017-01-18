#pragma once

#include "Assets/Asset.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

namespace Editor
{
	class IAssetPropertiesViewer: public IObject
	{
	public:
		// Default constructor
		IAssetPropertiesViewer() {}

		// Virtual destructor
		virtual ~IAssetPropertiesViewer() {}

		// Sets target actors
		virtual void SetTargetAssets(const Vector<AssetRef*>& assets) {}

		// Returns viewing asset type 
		virtual const Type* GetAssetType() const { return nullptr; }

		// Returns data widget
		virtual UIWidget* GetWidget() const { return nullptr; }

		IOBJECT(IAssetPropertiesViewer);
	};
}
