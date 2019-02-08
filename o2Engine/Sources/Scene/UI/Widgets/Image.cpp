#include "stdafx.h"
#include "Image.h"

#include "Render/Sprite.h"
#include "Scene/UI/WidgetLayer.h"

namespace o2
{
	UIImage::UIImage():
		UIWidget()
	{
		mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());
	}

	UIImage::UIImage(const UIImage& other):
		UIWidget(other), image(this), imageAsset(this), imageName(this)
	{
		mImage = GetLayerDrawable<Sprite>("image");
		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());
	}

	UIImage& UIImage::operator=(const UIImage& other)
	{
		UIWidget::operator=(other);
		return *this;
	}

	void UIImage::SetImage(Sprite* sprite)
	{
		if (mImage)
			delete mImage;

		if (auto layer = FindLayer("image"))
		{
			layer->SetDrawable(sprite);
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
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());

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
		auto imageAsset = GetImageAsset();
		if (imageAsset.IsValid())
			return imageAsset->GetPath();

		return "";
	}

	void UIImage::CopyData(const Actor& otherActor)
	{
		const UIImage& other = dynamic_cast<const UIImage&>(otherActor);

		UIWidget::CopyData(other);

		mImage = GetLayerDrawable<Sprite>("image");
		if (!mImage)
			mImage = dynamic_cast<Sprite*>(AddLayer("image", mnew Sprite())->GetDrawable());
	}
}

DECLARE_CLASS(o2::UIImage);
