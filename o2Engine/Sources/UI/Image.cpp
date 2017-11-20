#include "Image.h"

#include "Render/Sprite.h"
#include "UI/WidgetLayer.h"

namespace o2
{
	UIImage::UIImage():
		UIWidget()
	{
		mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->drawable);
		InitializeProperties();
	}

	UIImage::UIImage(const UIImage& other):
		UIWidget(other)
	{
		mImage = GetLayerDrawable<Sprite>("image");
		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->drawable);

		InitializeProperties();
	}

	UIImage& UIImage::operator=(const UIImage& other)
	{
		CopyData(other);
		return *this;
	}

	void UIImage::SetImage(Sprite* sprite)
	{
		if (mImage)
			delete mImage;

		if (auto layer = FindLayer("image"))
		{
			layer->drawable = sprite;
			mImage = sprite;
		}
	}

	Sprite* UIImage::GetImage()
	{
		return mImage;
	}

	void UIImage::SetImageAsset(const ImageAssetRef& asset)
	{
		if (!mImage)
			mImage = GetLayerDrawable<Sprite>("image");

		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->drawable);

		mImage->LoadFromImage(asset);
	}

	ImageAssetRef UIImage::GetImageAsset() const
	{
		if (mImage)
			return mImage->GetImageAsset();

		return ImageAssetRef();
	}

	void UIImage::SetImageName(const String& name)
	{
		SetImageAsset(ImageAssetRef(name));
	}

	String UIImage::GetImageName() const
	{
		return GetImageAsset()->GetPath();
	}

	void UIImage::CopyData(const Actor& otherActor)
	{
		const UIImage& other = dynamic_cast<const UIImage&>(otherActor);

		UIWidget::CopyData(other);

		mImage = GetLayerDrawable<Sprite>("image");
		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->drawable);
	}

	void UIImage::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIImage, image, SetImage, GetImage);
		INITIALIZE_PROPERTY(UIImage, imageAsset, SetImageAsset, GetImageAsset);
		INITIALIZE_PROPERTY(UIImage, imageName, SetImageName, GetImageName);
	}
}

DECLARE_CLASS(o2::UIImage);
