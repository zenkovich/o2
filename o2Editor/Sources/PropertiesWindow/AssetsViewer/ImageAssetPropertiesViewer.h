#pragma once

#include "Assets/AtlasAsset.h"
#include "PropertiesWindow/AssetsViewer/IAssetPropertiesViewer.h"
#include "PropertiesWindow/Properties/AssetProperty.h"
#include "Utils/CursorEventsArea.h"

using namespace o2;

namespace o2
{
	class UIVerticalLayout;
	class UIImage;
	class ImageAsset;
}

namespace Editor
{
	class BorderIProperty;
	class EnumProperty;
	class ObjectProperty;

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
		Vector<ImageAsset*> mTargetAssets;

		UIVerticalLayout*          mContent = nullptr;

		UIWidget*                  mPreviewImageContent;
		UIImage*                   mPreviewImage = nullptr;
		UIImage*                   mPreviewImageBack = nullptr;

		UIImage*                   mBorderLeftHandleWidget = nullptr;
		UIImage*                   mBorderRightHandleWidget = nullptr;
		UIImage*                   mBorderTopHandleWidget = nullptr;
		UIImage*                   mBorderBottomHandleWidget = nullptr;

		CursorEventsArea           mBorderLeftHandle;
		CursorEventsArea           mBorderRightHandle;
		CursorEventsArea           mBorderTopHandle;
		CursorEventsArea           mBorderBottomHandle;
		BorderF                    mBordersSmoothValue;

		BorderIProperty*           mBorderProperty = nullptr;
		EnumProperty*              mDefaultTypeProperty = nullptr;
		AssetProperty<AtlasAsset>* mAtlasProperty = nullptr;
		ObjectProperty*            mWindowsProperties = nullptr;
		ObjectProperty*            mOSXProperties = nullptr;
		ObjectProperty*            mAndroidProperties = nullptr;
		ObjectProperty*            mIOSProperties = nullptr;

	protected:
		// Initializes image preview widgets and border handles
		void InitializeImagePreview();

		// Initializes left border handle
		void InitializeLeftHandle();

		// Initializes right border handle
		void InitializeRightHandle();

		// Initializes top border handle
		void InitializeTopHandle();

		// Initializes bottom border handle
		void InitializeBottomHandle();

		// Initializes other properties
		void InitializeProperties();

		// Fits image by size
		void FitImage();

		// Updates borders anchors layouts
		void UpdateBordersAnchors();

		// Updates targets assets borders values by floored mBordersSmoothValue
		void UpdateBordersValue();

		// Sets common value for atlas property
		void SetupAtlasProperty();

		// Calls when atlas property was changed
		void OnAtlasPropertyChanged();
	};
}
