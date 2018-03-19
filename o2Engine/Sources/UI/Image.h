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
		PROPERTIES(UIImage);
		PROPERTY(Sprite*, image, SetImage, GetImage);                      // Image sprite
		PROPERTY(ImageAssetRef, imageAsset, SetImageAsset, GetImageAsset); // Image asset
		PROPERTY(String, imageName, SetImageName, GetImageName);           // Image asset name

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
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

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

	PUBLIC_FUNCTION(void, PROPERTIES, UIImage);
	PUBLIC_FUNCTION(void, SetImage, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetImage);
	PUBLIC_FUNCTION(void, SetImageAsset, const ImageAssetRef&);
	PUBLIC_FUNCTION(ImageAssetRef, GetImageAsset);
	PUBLIC_FUNCTION(void, SetImageName, const String&);
	PUBLIC_FUNCTION(String, GetImageName);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
}
END_META;
