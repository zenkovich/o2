#pragma once

#include "Assets/ImageAsset.h"
#include "Render/Sprite.h"
#include "Render/TextureRef.h"
#include "Scene/DrawableComponent.h"
#include "Utils/UID.h"

namespace o2
{
	// ---------------
	// Image component
	// ---------------
	class ImageComponent: public DrawableComponent
	{
	public:
		PROPERTIES(ImageComponent);
		PROPERTY(TextureRef, texture, SetTexture, GetTexture);                         // Texture property
		PROPERTY(RectI, textureSrcRect, SetTextureSrcRect, GetTextureSrcRect);         // Texture source rectangle property
		SETTER(String, imageAssetPath, LoadFromImage);                                 // Sets image asset from path
		PROPERTY(ImageAssetRef, imageAsset, LoadFromImage, GetImageAsset);             // Image asset property
		SETTER(Bitmap*, bitmap, LoadFromBitmap);                                       // Sets image from bitmap
		PROPERTY(Color4, color, SetColor, GetColor);                                   // Color property
		PROPERTY(Color4, leftTopColor, SetLeftTopColor, GetLeftTopCorner);	           // Color of left top corner property
		PROPERTY(Color4, rightTopColor, SetRightTopColor, GetRightTopCorner);	       // Color of right top corner property
		PROPERTY(Color4, leftBottomColor, SetLeftBottomColor, GetLeftBottomCorner);    // Color of left bottom corner property
		PROPERTY(Color4, rightBottomColor, SetRightBottomColor, GetRightBottomCorner); // Color of right bottom corner property
		PROPERTY(SpriteMode, mode, SetMode, GetMode);                                  // Sprite drawing mode property
		PROPERTY(float, fill, SetFill, GetFill);                                       // Sprite fill property
		PROPERTY(float, tileScale, SetTileScale, GetTileScale);                        // Sprite tile scale property, 1.0f is default
		PROPERTY(BorderI, sliceBorder, SetSliceBorder, GetSliceBorder);                // Slice border property

		// Default constructor
		ImageComponent();

		// Constructor from image asset
		ImageComponent(const ImageAssetRef& image);

		// Constructor from image asset by path
		ImageComponent(const String& imagePath);

		// Constructor from image asset by id
		ImageComponent(UID imageId);

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
		void Draw();

		// Sets using texture
		void SetTexture(TextureRef texture);

		// Returns using texture
		TextureRef GetTexture() const;

		// Sets texture source rectangle
		void SetTextureSrcRect(const RectI& rect);

		// Returns texture source rectangle
		RectI GetTextureSrcRect() const;

		// Sets corner color
		void SetCornerColor(Corner corner, const Color4& color);

		// Returns corner color
		Color4 GetCornerColor(Corner corner) const;

		// Sets left top corner color
		void SetLeftTopColor(const Color4& color);

		// Returns left top corner color
		Color4 GetLeftTopCorner() const;

		// Sets right top corner color
		void SetRightTopColor(const Color4& color);

		// Returns right top corner color
		Color4 GetRightTopCorner() const;

		// Sets right bottom corner color
		void SetRightBottomColor(const Color4& color);

		// Returns right bottom corner color
		Color4 GetRightBottomCorner() const;

		// Sets left bottom corner color
		void SetLeftBottomColor(const Color4& color);

		// Returns left bottom corner color
		Color4 GetLeftBottomCorner() const;

		// Sets color
		void SetColor(const Color4& color);

		// Returns color
		Color4 GetColor() const;

		// Sets sprite fill value (0 ... 1)
		void SetFill(float fill);

		// Returns sprite fill
		float GetFill() const;

		// Sets tile scale. 1.0f is default
		void SetTileScale(float scale);

		// Returns tile scale
		float GetTileScale() const;

		// Sets sprite drawing mode
		void SetMode(SpriteMode mode);

		// Returns sprite drawing mode
		SpriteMode GetMode() const;

		// Sets sprite slice border
		void SetSliceBorder(const BorderI& border);

		// Returns sprite slice border
		BorderI GetSliceBorder() const;

		// Loads sprite from image asset
		void LoadFromImage(const ImageAssetRef& image);

		// Loads sprite from image asset by path
		void LoadFromImage(const String& imagePath);

		// Loads sprite from image asset by id
		void LoadFromImage(UID imageId);

		// Loads sprite from mono color
		void LoadMonoColor(const Color4& color);

		// Loads sprite from bitmap
		void LoadFromBitmap(Bitmap* bitmap);

		// Returns image asset
		ImageAssetRef GetImageAsset() const;

		// Sets size by texture source rectangle size
		void NormalizeSize();

		// Sets size with equal aspect as texture source rectangle by width
		void NormalizeAspectByWidth();

		// Sets size with equal aspect as texture source rectangle by width
		void NormalizeAspectByHeight();

		// Sets size with equal aspect as texture source rectangle by nearest value
		void NormalizeAspect();

		// Returns name of component
		String GetName() const;

		SERIALIZABLE(ImageComponent);

	protected:
		Sprite mSprite; // @SERIALIZABLE @EDITOR_IGNORE

	protected:
		// It is called when actor's transform was changed
		void OnTransformUpdated();

		// Sets owner actor
		void SetOwnerActor(Actor* actor);

		// Calling when deserializing
		void OnDeserialized(const DataNode& node);
	};
}

CLASS_BASES_META(o2::ImageComponent)
{
	BASE_CLASS(o2::DrawableComponent);
}
END_META;
CLASS_FIELDS_META(o2::ImageComponent)
{
	PUBLIC_FIELD(texture);
	PUBLIC_FIELD(textureSrcRect);
	PUBLIC_FIELD(imageAssetPath);
	PUBLIC_FIELD(imageAsset);
	PUBLIC_FIELD(bitmap);
	PUBLIC_FIELD(color);
	PUBLIC_FIELD(leftTopColor);
	PUBLIC_FIELD(rightTopColor);
	PUBLIC_FIELD(leftBottomColor);
	PUBLIC_FIELD(rightBottomColor);
	PUBLIC_FIELD(mode);
	PUBLIC_FIELD(fill);
	PUBLIC_FIELD(tileScale);
	PUBLIC_FIELD(sliceBorder);
	PROTECTED_FIELD(mSprite).EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ImageComponent)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetTexture, TextureRef);
	PUBLIC_FUNCTION(TextureRef, GetTexture);
	PUBLIC_FUNCTION(void, SetTextureSrcRect, const RectI&);
	PUBLIC_FUNCTION(RectI, GetTextureSrcRect);
	PUBLIC_FUNCTION(void, SetCornerColor, Corner, const Color4&);
	PUBLIC_FUNCTION(Color4, GetCornerColor, Corner);
	PUBLIC_FUNCTION(void, SetLeftTopColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetLeftTopCorner);
	PUBLIC_FUNCTION(void, SetRightTopColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetRightTopCorner);
	PUBLIC_FUNCTION(void, SetRightBottomColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetRightBottomCorner);
	PUBLIC_FUNCTION(void, SetLeftBottomColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetLeftBottomCorner);
	PUBLIC_FUNCTION(void, SetColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetColor);
	PUBLIC_FUNCTION(void, SetFill, float);
	PUBLIC_FUNCTION(float, GetFill);
	PUBLIC_FUNCTION(void, SetTileScale, float);
	PUBLIC_FUNCTION(float, GetTileScale);
	PUBLIC_FUNCTION(void, SetMode, SpriteMode);
	PUBLIC_FUNCTION(SpriteMode, GetMode);
	PUBLIC_FUNCTION(void, SetSliceBorder, const BorderI&);
	PUBLIC_FUNCTION(BorderI, GetSliceBorder);
	PUBLIC_FUNCTION(void, LoadFromImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, LoadFromImage, const String&);
	PUBLIC_FUNCTION(void, LoadFromImage, UID);
	PUBLIC_FUNCTION(void, LoadMonoColor, const Color4&);
	PUBLIC_FUNCTION(void, LoadFromBitmap, Bitmap*);
	PUBLIC_FUNCTION(ImageAssetRef, GetImageAsset);
	PUBLIC_FUNCTION(void, NormalizeSize);
	PUBLIC_FUNCTION(void, NormalizeAspectByWidth);
	PUBLIC_FUNCTION(void, NormalizeAspectByHeight);
	PUBLIC_FUNCTION(void, NormalizeAspect);
	PUBLIC_FUNCTION(String, GetName);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;
