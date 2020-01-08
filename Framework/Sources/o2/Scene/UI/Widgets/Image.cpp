#include "o2/stdafx.h"
#include "Image.h"

#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/WidgetLayer.h"

namespace o2
{
	Image::Image():
		Widget()
	{
		mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());
	}

	Image::Image(const Image& other):
		Widget(other), image(this), imageAsset(this), imageName(this)
	{
		mImage = GetLayerDrawable<Sprite>("image");
		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());
	}

	Image& Image::operator=(const Image& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void Image::SetImage(Sprite* sprite)
	{
		if (mImage)
			delete mImage;

		if (auto layer = FindLayer("image"))
		{
			layer->SetDrawable(sprite);
			mImage = sprite;
		}
	}

	Sprite* Image::GetImage()
	{
		return mImage;
	}

	void Image::SetImageAsset(const ImageAssetRef& asset)
	{
		if (!mImage)
			mImage = GetLayerDrawable<Sprite>("image");

		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());

		mImage->LoadFromImage(asset);
	}

	ImageAssetRef Image::GetImageAsset() const
	{
		if (mImage)
			return mImage->GetImageAsset();

		return ImageAssetRef();
	}

	void Image::SetImageName(const String& name)
	{
		SetImageAsset(ImageAssetRef(name));
	}

	String Image::GetImageName() const
	{
		auto imageAsset = GetImageAsset();
		if (imageAsset.IsValid())
			return imageAsset->GetPath();

		return "";
	}

	void Image::CopyData(const Actor& otherActor)
	{
		const Image& other = dynamic_cast<const Image&>(otherActor);

		Widget::CopyData(other);

		mImage = GetLayerDrawable<Sprite>("image");
		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());
	}
}

DECLARE_CLASS(o2::Image);
