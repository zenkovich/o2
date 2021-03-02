#include "o2/stdafx.h"
#include "ImageComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
	ImageComponent::ImageComponent():
		DrawableComponent(), Sprite()
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(const ImageAssetRef& image) :
		DrawableComponent(), Sprite(image)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(const String& imagePath) :
		DrawableComponent(), Sprite(imagePath)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(const UID& imageId) :
		DrawableComponent(), Sprite(imageId)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(TextureRef texture, const RectI& srcRect) :
		DrawableComponent(), Sprite(texture, srcRect)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(const Color4& color) :
		DrawableComponent(), Sprite(color)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(Bitmap* bitmap) :
		DrawableComponent(), Sprite(bitmap)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(const Sprite& sprite) :
		DrawableComponent(), Sprite(sprite)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::ImageComponent(const ImageComponent& other) :
		DrawableComponent(other), Sprite(other)
	{
		mSerializeEnabled = false;
	}

	ImageComponent::~ImageComponent()
	{}

	ImageComponent& ImageComponent::operator=(const ImageComponent& other)
	{
		DrawableComponent::operator=(other);
		Sprite::operator=(other);
		return *this;
	}

	void ImageComponent::Draw()
	{
		Sprite::Draw();
		DrawableComponent::OnDrawn();
	}

	void ImageComponent::FitActorByImage() const
	{
		if (mImageAsset)
			mOwner->transform->size = mImageAsset->GetSize();
	}

	String ImageComponent::GetName()
	{
		return "Image";
	}

	String ImageComponent::GetCategory()
	{
		return "Render";
	}

	String ImageComponent::GetIcon()
	{
		return "ui/UI4_image_component.png";
	}

	void ImageComponent::OnTransformUpdated()
	{
		SetBasis(mOwner->transform->GetWorldBasis());
	}

	void ImageComponent::SetOwnerActor(Actor* actor)
	{
		DrawableComponent::SetOwnerActor(actor);
	}

	void ImageComponent::OnDeserialized(const DataValue& node)
	{
		DrawableComponent::OnDeserialized(node);
		Sprite::OnDeserialized(node);
	}

	void ImageComponent::OnSerialize(DataValue& node) const
	{
		DrawableComponent::OnSerialize(node);
		Sprite::OnSerialize(node);
	}

}

DECLARE_CLASS_MANUAL(o2::Ref<o2::ImageComponent>);

DECLARE_CLASS(o2::ImageComponent);
