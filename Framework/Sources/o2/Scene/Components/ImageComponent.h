#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Sprite.h"
#include "o2/Render/TextureRef.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Utils/Types/UID.h"

namespace o2
{
	// ---------------
	// Image component
	// ---------------
	class ImageComponent: public DrawableComponent, public Sprite
	{
	public:
		PROPERTIES(ImageComponent);

	public:
		// Default constructor @SCRIPTABLE
		ImageComponent();

		// Constructor from image asset
		ImageComponent(const ImageAssetRef& image);

		// Constructor from image asset by path
		ImageComponent(const String& imagePath);

		// Constructor from image asset by id
		ImageComponent(const UID& imageId);

		// Constructor from texture and source rectangle
		ImageComponent(TextureRef texture, const RectI& srcRect);

		// Constructor from color
		ImageComponent(const Color4& color);

		// Constructor from bitmap
		ImageComponent(const Ref<Bitmap>& bitmap);

		// Constructor from sprite
		ImageComponent(const Sprite& sprite);

		// Copy-constructor
		ImageComponent(const ImageComponent& other);

		// Destructor
		~ImageComponent();

		// Assign operator
		ImageComponent& operator=(const ImageComponent& other);

		// Draws sprite 
		void Draw() override;

		// Sets actor's size as image size
		void FitActorByImage() const;

		// Returns true if point is under drawable
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		// Returns name of component icon
		static String GetIcon();

		SERIALIZABLE(ImageComponent);

		using Sprite::onDraw;

	protected:
		// Called when actor's transform was changed
		void OnTransformUpdated() override;

		// Sets owner actor
		void SetOwnerActor(Actor* actor) override;

		// Calling when deserializing
		void OnDeserialized(const DataValue& node) override;

		// Calling when serializing
		void OnSerialize(DataValue& node) const override;

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;
	};
}

CLASS_BASES_META(o2::ImageComponent)
{
	BASE_CLASS(o2::DrawableComponent);
	BASE_CLASS(o2::Sprite);
}
END_META;
CLASS_FIELDS_META(o2::ImageComponent)
{
}
END_META;
CLASS_METHODS_META(o2::ImageComponent)
{

	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ImageAssetRef&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const String&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const UID&);
	FUNCTION().PUBLIC().CONSTRUCTOR(TextureRef, const RectI&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Color4&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<Bitmap>&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Sprite&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const ImageComponent&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, FitActorByImage);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetName);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetIcon);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
	FUNCTION().PROTECTED().SIGNATURE(void, SetOwnerActor, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
}
END_META;
