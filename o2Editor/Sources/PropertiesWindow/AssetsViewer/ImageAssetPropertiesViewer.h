#pragma once

#include "PropertiesWindow/AssetsViewer/IAssetPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class UIVerticalLayout;
	class UIImage;
}

namespace Editor
{
	class ImageAssetPropertiesViewer: public IAssetPropertiesViewer
	{
	public:
		// Default constructor
		ImageAssetPropertiesViewer();

		// Destructor
		~ImageAssetPropertiesViewer();

		// Sets target actors
		void SetTargetAssets(const Vector<Asset*>& assets);

		// Returns viewing asset type 
		const Type* GetAssetType() const;

		// Returns data widget
		UIWidget* GetWidget() const;

		IOBJECT(ImageAssetPropertiesViewer);

	protected:
		UIVerticalLayout* mContent = nullptr;
	};
}
