#include "o2/stdafx.h"
#include "ImageComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"

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

	ImageComponent::ImageComponent(const UID& imageId) :
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

	void ImageComponent::SerializeBasicOverride(DataValue& node) const
	{
		SerializeTypeProcessor processor(node);
		IRectDrawable::ProcessFields((IRectDrawable*)const_cast<ImageComponent*>(this), processor);
		Sprite::ProcessFields((Sprite*)const_cast<ImageComponent*>(this), processor);
		ProcessFields(const_cast<ImageComponent*>(this), processor);
		OnSerialize(node);
	}

	void ImageComponent::DeserializeBasicOverride(const DataValue& node)
	{
		DeserializeTypeProcessor processor(node);
		IRectDrawable::ProcessFields((IRectDrawable*)const_cast<ImageComponent*>(this), processor);
		Sprite::ProcessFields((Sprite*)const_cast<ImageComponent*>(this), processor);
		ProcessFields(const_cast<ImageComponent*>(this), processor);
		OnDeserialized(node);
	}

	void ImageComponent::SerializeDeltaBasicOverride(DataValue& node, const IObject& origin) const
	{
		SerializeDeltaTypeProcessor<ImageComponent> processor(node, dynamic_cast<const ImageComponent&>(origin));
		IRectDrawable::ProcessFields((IRectDrawable*)const_cast<ImageComponent*>(this), processor);
		Sprite::ProcessFields((Sprite*)const_cast<ImageComponent*>(this), processor);
		ProcessFields(const_cast<ImageComponent*>(this), processor);
		OnSerializeDelta(node, origin);
		OnSerialize(node);
	}

	void ImageComponent::DeserializeDeltaBasicOverride(const DataValue& node, const IObject& origin)
	{
		DeserializeDeltaTypeProcessor<ImageComponent> processor(node, dynamic_cast<const ImageComponent&>(origin));
		IRectDrawable::ProcessFields((IRectDrawable*)const_cast<ImageComponent*>(this), processor);
		Sprite::ProcessFields((Sprite*)const_cast<ImageComponent*>(this), processor);
		ProcessFields(const_cast<ImageComponent*>(this), processor);
		OnDeserializedDelta(node, origin);
		OnDeserialized(node);
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
