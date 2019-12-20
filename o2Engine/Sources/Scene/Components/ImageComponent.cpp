#include "stdafx.h"
#include "ImageComponent.h"

#include "Scene/Actor.h"
#include "Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
	ImageComponent::ImageComponent():
		DrawableComponent(), Sprite()
	{}

	ImageComponent::ImageComponent(const ImageAssetRef& image) :
		DrawableComponent(), Sprite(image)
	{}

	ImageComponent::ImageComponent(const String& imagePath) :
		DrawableComponent(), Sprite(imagePath)
	{}

	ImageComponent::ImageComponent(UID imageId) :
		DrawableComponent(), Sprite(imageId)
	{}

	ImageComponent::ImageComponent(TextureRef texture, const RectI& srcRect) :
		DrawableComponent(), Sprite(texture, srcRect)
	{}

	ImageComponent::ImageComponent(const Color4& color) :
		DrawableComponent(), Sprite(color)
	{}

	ImageComponent::ImageComponent(Bitmap* bitmap) :
		DrawableComponent(), Sprite(bitmap)
	{}

	ImageComponent::ImageComponent(const Sprite& sprite) :
		DrawableComponent(), Sprite(sprite)
	{}

	ImageComponent::ImageComponent(const ImageComponent& other) :
		DrawableComponent(other), Sprite(other)
	{}

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
		SetBasis(mOwner->transform->GetWorldBasis());
	}

	void ImageComponent::SetOwnerActor(Actor* actor)
	{
		if (actor)
			actor->transform->SetSize(Sprite::GetSize());

		DrawableComponent::SetOwnerActor(actor);
	}

	void ImageComponent::OnDeserialized(const DataNode& node)
	{
		Sprite::OnDeserialized(node);
	}

	void ImageComponent::OnSerialize(DataNode& node) const
	{
		Sprite::OnSerialize(node);
	}

}

DECLARE_CLASS(o2::ImageComponent);
