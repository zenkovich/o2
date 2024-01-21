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

    ImageComponent::ImageComponent(Ref<Texture> texture, const RectI& srcRect) :
        DrawableComponent(), Sprite(texture, srcRect)
    {
        mSerializeEnabled = false;
    }

    ImageComponent::ImageComponent(const Color4& color) :
        DrawableComponent(), Sprite(color)
    {
        mSerializeEnabled = false;
    }

    ImageComponent::ImageComponent(const Bitmap& bitmap) :
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
        //PROFILE_SAMPLE_FUNC();

        Sprite::Draw();
    }

    void ImageComponent::FitActorByImage() const
    {
        if (mImageAsset)
            mOwner.Lock()->transform->size = mImageAsset->GetSize();
    }

    bool ImageComponent::IsUnderPoint(const Vec2F& point)
    {
        return Sprite::IsUnderPoint(point);
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
        SetBasis(mOwner.Lock()->transform->GetWorldBasis());
    }

    void ImageComponent::SetOwnerActor(const Ref<Actor>& actor)
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

    void ImageComponent::OnSerializeDelta(DataValue& node, const IObject& origin) const
    {
        DrawableComponent::OnSerializeDelta(node, origin);
        Sprite::OnSerializeDelta(node, origin);
    }

    void ImageComponent::OnDeserializedDelta(const DataValue& node, const IObject& origin)
    {
        DrawableComponent::OnDeserializedDelta(node, origin);
        Sprite::OnDeserializedDelta(node, origin);
    }

}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::ImageComponent>);
// --- META ---

DECLARE_CLASS(o2::ImageComponent, o2__ImageComponent);
// --- END META ---
