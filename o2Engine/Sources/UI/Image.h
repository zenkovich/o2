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
		Property<Sprite*>    image;      // Image sprite
		Property<ImageAsset> imageAsset; // Image asset
		Property<String>     imageName;  // Image asset name

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
		void SetImageAsset(const ImageAsset& asset);

		// Returns image asset
		ImageAsset GetImageAsset() const;

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
