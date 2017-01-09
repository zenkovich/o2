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
		Property<TextureRef> texture;          // Texture property
		Property<RectI>      textureSrcRect;   // Texture source rectangle property
		Property<UID>        imageAssetId;     // Image asset id property
		Setter<String>       imageAssetPath;   // Sets image asset from path
		Setter<ImageAsset>   imageAsset;       // Sets image asset
		Setter<Bitmap*>      bitmap;           // Sets image from bitmap
		Property<Color4>     leftTopColor;	   // Color of left top corner property
		Property<Color4>     rightTopColor;	   // Color of right top corner property
		Property<Color4>     leftBottomColor;  // Color of left bottom corner property
		Property<Color4>     rightBottomColor; // Color of right bottom corner property
		Property<SpriteMode> mode;             // Sprite drawing mode property
		Property<float>      fill;             // Sprite fill property
		Property<float>      tileScale;        // Sprite tile scale property, 1.0f is default
		Property<BorderI>    sliceBorder;      // Slice border property

		// Default constructor
		ImageComponent();

		// Constructor from image asset
		ImageComponent(const ImageAsset& image);

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
		void LoadFromImage(const ImageAsset& image);

		// Loads sprite from image asset by path
		void LoadFromImage(const String& imagePath);

		// Loads sprite from image asset by id
		void LoadFromImage(UID imageId);

		// Loads sprite from mono color
		void LoadMonoColor(const Color4& color);

		// Loads sprite from bitmap
		void LoadFromBitmap(Bitmap* bitmap);

		// Returns image asset id (returns 0 when asset id is unknown)
		UID GetImageId() const;

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
		// Calls when actor's transform was changed
		void OnTransformChanged();

		// Sets owner actor
		void SetOwnerActor(Actor* actor);

		// Initializes properties
		void InitializeProperties();
	};
}