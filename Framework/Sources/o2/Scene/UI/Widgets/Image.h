#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/UI/Widget.h"

namespace o2
{
	class Sprite;

	// ------------
	// Image widget
	// ------------
	class Image: public Widget
	{
	public:
		PROPERTIES(Image);
		PROPERTY(Sprite*, image, SetImage, GetImage);                      // Image sprite
		PROPERTY(ImageAssetRef, imageAsset, SetImageAsset, GetImageAsset); // Image asset
		PROPERTY(String, imageName, SetImageName, GetImageName);           // Image asset name

	public:
		// Default constructor
		Image();

		// Copy-constructor
		Image(const Image& other);

		// Assign operator
		Image& operator=(const Image& other);

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

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(Image);

	protected:
		Sprite* mImage = nullptr; // Image layer drawable

		friend class UIButtonGroup;
	};
}

CLASS_BASES_META(o2::Image)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::Image)
{
	FIELD().PUBLIC().NAME(image);
	FIELD().PUBLIC().NAME(imageAsset);
	FIELD().PUBLIC().NAME(imageName);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mImage);
}
END_META;
CLASS_METHODS_META(o2::Image)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Image&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetImage, Sprite*);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetImage);
	FUNCTION().PUBLIC().SIGNATURE(void, SetImageAsset, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(ImageAssetRef, GetImageAsset);
	FUNCTION().PUBLIC().SIGNATURE(void, SetImageName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(String, GetImageName);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
}
END_META;
