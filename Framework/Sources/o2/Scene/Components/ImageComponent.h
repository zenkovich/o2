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
		// Default constructor
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
		ImageComponent(Bitmap* bitmap);

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

		// Returns name of component
		String GetName() const override;

		// Returns category of component
		String GetCategory() const override;

		// Returns name of component icon
		String GetIcon() const override;

		SERIALIZABLE(ImageComponent);

	protected:
		// It is called when actor's transform was changed
		void OnTransformUpdated() override;

		// Sets owner actor
		void SetOwnerActor(Actor* actor) override;

		// Calling when deserializing
		void OnDeserialized(const DataNode& node) override;

		// Calling when serializing
		void OnSerialize(DataNode& node) const override;
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

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(String, GetCategory);
	PUBLIC_FUNCTION(String, GetIcon);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
}
END_META;
