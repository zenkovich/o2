#include "stdafx.h"
#include "ImageComponent.h"

#include "Scene/Actor.h"
#include "Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
	ImageComponent::ImageComponent():
		DrawableComponent(), mSprite()
	{}

	ImageComponent::ImageComponent(const ImageAssetRef& image) :
		DrawableComponent(), mSprite(image)
	{}

	ImageComponent::ImageComponent(const String& imagePath) :
		DrawableComponent(), mSprite(imagePath)
	{}

	ImageComponent::ImageComponent(UID imageId) :
		DrawableComponent(), mSprite(imageId)
	{}

	ImageComponent::ImageComponent(TextureRef texture, const RectI& srcRect) :
		DrawableComponent(), mSprite(texture, srcRect)
	{}

	ImageComponent::ImageComponent(const Color4& color) :
		DrawableComponent(), mSprite(color)
	{}

	ImageComponent::ImageComponent(Bitmap* bitmap) :
		DrawableComponent(), mSprite(bitmap)
	{}

	ImageComponent::ImageComponent(const Sprite& sprite) :
		DrawableComponent(), mSprite(sprite)
	{}

	ImageComponent::ImageComponent(const ImageComponent& other) :
		DrawableComponent(other), mSprite(other.mSprite), texture(this), textureSrcRect(this), imageAssetPath(this),
		imageAsset(this), bitmap(this), color(this), leftTopColor(this), rightTopColor(this), leftBottomColor(this),
		rightBottomColor(this), mode(this), fill(this), tileScale(this), sliceBorder(this)
	{}

	ImageComponent::~ImageComponent()
	{}

	ImageComponent& ImageComponent::operator=(const ImageComponent& other)
	{
		DrawableComponent::operator=(other);
		mSprite = other.mSprite;
		return *this;
	}

	void ImageComponent::Draw()
	{
		mSprite.Draw();
		OnDrawn();
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

	void ImageComponent::SetColor(const Color4& color)
	{
		mSprite.SetColor(color);
	}

	Color4 ImageComponent::GetColor() const
	{
		return mSprite.GetColor();
	}

	void ImageComponent::SetFill(float fill)
	{
		mSprite.SetFill(fill);
	}

	float ImageComponent::GetFill() const
	{
		return mSprite.GetFill();
	}

	void ImageComponent::SetTileScale(float scale)
	{
		mSprite.SetTileScale(scale);
	}

	float ImageComponent::GetTileScale() const
	{
		return mSprite.GetTileScale();
	}

	void ImageComponent::SetMode(SpriteMode mode)
	{
		mSprite.SetMode(mode);
	}

	SpriteMode ImageComponent::GetMode() const
	{
		return mSprite.GetMode();
	}

	void ImageComponent::SetSliceBorder(const BorderI& border)
	{
		mSprite.SetSliceBorder(border);
	}

	BorderI ImageComponent::GetSliceBorder() const
	{
		return mSprite.GetSliceBorder();
	}

	void ImageComponent::LoadFromImage(const ImageAssetRef& image)
	{
		mSprite.LoadFromImage(image);
		OnTransformUpdated();
	}

	void ImageComponent::LoadFromImage(const String& imagePath)
	{
		mSprite.LoadFromImage(imagePath);
	}

	void ImageComponent::LoadFromImage(UID imageId)
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

	ImageAssetRef ImageComponent::GetImageAsset() const
	{
		return mSprite.GetImageAsset();
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

	String ImageComponent::GetName() const
	{
		return "Image";
	}

	String ImageComponent::GetCategory() const
	{
		return "Render";
	}

	String ImageComponent::GetIcon() const
	{
		return "ui/UI4_image_component.png";
	}

	void ImageComponent::OnTransformUpdated()
	{
		mSprite.SetBasis(mOwner->transform->GetWorldBasis());
	}

	void ImageComponent::SetOwnerActor(Actor* actor)
	{
		if (actor)
			actor->transform->SetSize(mSprite.GetSize());

		DrawableComponent::SetOwnerActor(actor);
	}

	void ImageComponent::OnDeserialized(const DataNode& node)
	{
		mSprite.OnDeserialized(node);
	}
}

DECLARE_CLASS(o2::ImageComponent);
