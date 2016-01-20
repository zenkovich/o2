#include "ImageComponent.h"

#include "Scene/Actor.h"

namespace o2
{
	ImageComponent::ImageComponent():
		DrawableComponent(), mSprite()
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(ImageAsset* image):
		DrawableComponent(), mSprite(image)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(const String& imagePath):
		DrawableComponent(), mSprite(imagePath)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(AssetId imageId):
		DrawableComponent(), mSprite(imageId)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(TextureRef texture, const RectI& srcRect):
		DrawableComponent(), mSprite(texture, srcRect)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(const Color4& color):
		DrawableComponent(), mSprite(color)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(Bitmap* bitmap):
		DrawableComponent(), mSprite(bitmap)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(const Sprite& sprite):
		DrawableComponent(), mSprite(sprite)
	{
		InitializeProperties();
	}

	ImageComponent::ImageComponent(const ImageComponent& other):
		DrawableComponent(other), mSprite(other.mSprite)
	{
		InitializeProperties();
	}

	ImageComponent::~ImageComponent()
	{
	}

	ImageComponent& ImageComponent::operator=(const ImageComponent& other)
	{
		DrawableComponent::operator=(other);
		mSprite = other.mSprite;
		return *this;
	}

	void ImageComponent::Draw()
	{
		mSprite.Draw();
	}

	void ImageComponent::SetTexture(TextureRef texture)
	{
		mSprite.SetTexture(texture);
	}

	TextureRef ImageComponent::GetTexture() const
	{
		return mSprite.GetTexture();
	}

	void ImageComponent::SetTextureSrcRect(const RectI& rect)
	{
		mSprite.SetTextureSrcRect(rect);
	}

	RectI ImageComponent::GetTextureSrcRect() const
	{
		return mSprite.GetTextureSrcRect();
	}

	void ImageComponent::SetCornerColor(Corner corner, const Color4& color)
	{
		mSprite.SetCornerColor(corner, color);
	}

	Color4 ImageComponent::GetCornerColor(Corner corner) const
	{
		return mSprite.GetCornerColor(corner);
	}

	void ImageComponent::SetLeftTopColor(const Color4& color)
	{
		mSprite.SetLeftTopColor(color);
	}

	Color4 ImageComponent::GetLeftTopCorner() const
	{
		return mSprite.GetLeftTopCorner();
	}

	void ImageComponent::SetRightTopColor(const Color4& color)
	{
		mSprite.SetRightTopColor(color);
	}

	Color4 ImageComponent::GetRightTopCorner() const
	{
		return mSprite.GetRightTopCorner();
	}

	void ImageComponent::SetRightBottomColor(const Color4& color)
	{
		mSprite.SetRightBottomColor(color);
	}

	Color4 ImageComponent::GetRightBottomCorner() const
	{
		return mSprite.GetRightBottomCorner();
	}

	void ImageComponent::SetLeftBottomColor(const Color4& color)
	{
		mSprite.SetLeftBottomColor(color);
	}

	Color4 ImageComponent::GetLeftBottomCorner() const
	{
		return mSprite.GetLeftBottomCorner();
	}

	void ImageComponent::SetFill(float fill)
	{
		mSprite.SetFill(fill);
	}

	float ImageComponent::GetFill() const
	{
		return mSprite.GetFill();
	}

	void ImageComponent::SetMode(SpriteMode mode)
	{
		mSprite.SetMode(mode);
	}

	SpriteMode ImageComponent::GetMode() const
	{
		return mSprite.GetMode();
	}

	void ImageComponent::SetSliceBorder(const RectI& border)
	{
		mSprite.SetSliceBorder(border);
	}

	RectI ImageComponent::GetSliceBorder() const
	{
		return mSprite.GetSliceBorder();
	}

	void ImageComponent::LoadFromImage(ImageAsset* image)
	{
		mSprite.LoadFromImage(image);
	}

	void ImageComponent::LoadFromImage(const String& imagePath)
	{
		mSprite.LoadFromImage(imagePath);
	}

	void ImageComponent::LoadFromImage(AssetId imageId)
	{
		mSprite.LoadFromImage(imageId);
	}

	void ImageComponent::LoadMonoColor(const Color4& color)
	{
		mSprite.LoadMonoColor(color);
	}

	void ImageComponent::LoadFromBitmap(Bitmap* bitmap)
	{
		mSprite.LoadFromBitmap(bitmap);
	}

	AssetId ImageComponent::GetImageId() const
	{
		return mSprite.GetImageId();
	}

	void ImageComponent::NormalizeSize()
	{
		mSprite.NormalizeSize();
	}

	void ImageComponent::NormalizeAspectByWidth()
	{
		mSprite.NormalizeAspectByWidth();
	}

	void ImageComponent::NormalizeAspectByHeight()
	{
		mSprite.NormalizeAspectByHeight();
	}

	void ImageComponent::NormalizeAspect()
	{
		mSprite.NormalizeAspect();
	}

	void ImageComponent::OnTransformChanged()
	{
		mSprite.SetBasis(mOwner->transform.GetWorldBasis());
	}

	void ImageComponent::SetOwnerActor(Actor* actor)
	{
		if (actor)
			actor->transform.SetSize(mSprite.GetSize());

		Component::SetOwnerActor(actor);
	}

	void ImageComponent::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ImageComponent, texture, SetTexture, GetTexture);
		INITIALIZE_PROPERTY(ImageComponent, textureSrcRect, SetTextureSrcRect, GetTextureSrcRect);
		INITIALIZE_PROPERTY(ImageComponent, imageAssetId, LoadFromImage, GetImageId);
		INITIALIZE_SETTER(ImageComponent, imageAssetPath, LoadFromImage);
		INITIALIZE_SETTER(ImageComponent, imageAsset, LoadFromImage);
		INITIALIZE_SETTER(ImageComponent, bitmap, LoadFromBitmap);
		INITIALIZE_PROPERTY(ImageComponent, leftTopColor, SetLeftTopColor, GetLeftTopCorner);
		INITIALIZE_PROPERTY(ImageComponent, rightTopColor, SetRightTopColor, GetRightTopCorner);
		INITIALIZE_PROPERTY(ImageComponent, leftBottomColor, SetLeftBottomColor, GetLeftBottomCorner);
		INITIALIZE_PROPERTY(ImageComponent, rightBottomColor, SetRightBottomColor, GetRightBottomCorner);
		INITIALIZE_PROPERTY(ImageComponent, mode, SetMode, GetMode);
		INITIALIZE_PROPERTY(ImageComponent, fill, SetFill, GetFill);
		INITIALIZE_PROPERTY(ImageComponent, sliceBorder, SetSliceBorder, GetSliceBorder);
	}
}