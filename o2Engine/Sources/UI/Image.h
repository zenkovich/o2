#pragma once

#include "Assets/ImageAsset.h"
#include "UI/Widget.h"

namespace o2
{
	class Sprite;

	// ------------
	// Image widget
	// ------------
	class UIImage: public UIWidget
	{
	public:
		Property<Sprite*>       image;      // Image sprite
		Property<ImageAssetRef> imageAsset; // Image asset
		Property<String>        imageName;  // Image asset name

		// Default constructor
		UIImage();

		// Copy-constructor
		UIImage(const UIImage& other);

		// Assign operator
		UIImage& operator=(const UIImage& other);

		// Sets image
		void SetImage(Sprite* sprite);

		// Returns image sprite
		Sprite* GetImage();

		// Sets image asset
		void SetImageAsset(const ImageAssetRef& asset);

		// Returns image asset
		ImageAssetRef GetImageAsset() const;

		// Sets image asset name
		void SetImageName(const String& name);

		// Returns asset image name
		String GetImageName() const;

		SERIALIZABLE(UIImage);

	protected:
		Sprite* mImage = nullptr; // Image layer drawable

	protected:
		// Initializes properties
		void InitializeProperties();

		friend class UIButtonGroup;
	};
}

CLASS_BASES_META(o2::UIImage)
{
	BASE_CLASS(o2::UIWidget);
}
END_META;
CLASS_FIELDS_META(o2::UIImage)
{
	PUBLIC_FIELD(image);
	PUBLIC_FIELD(imageAsset);
	PUBLIC_FIELD(imageName);
	PROTECTED_FIELD(mImage);
}
END_META;
CLASS_METHODS_META(o2::UIImage)
{

	PUBLIC_FUNCTION(void, SetImage, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetImage);
	PUBLIC_FUNCTION(void, SetImageAsset, const ImageAssetRef&);
	PUBLIC_FUNCTION(ImageAssetRef, GetImageAsset);
	PUBLIC_FUNCTION(void, SetImageName, const String&);
	PUBLIC_FUNCTION(String, GetImageName);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
