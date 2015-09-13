#pragma once

#include "Assets/Asset.h"
#include "Render/RectDrawable.h"
#include "Render/TextureRef.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	class Mesh;
	class ImageAsset;
	class Bitmap;

	// -----------------
	// Quad image sprite
	// -----------------
	class Sprite: public IRectDrawable
	{
	public:
		enum class Corner { LeftTop, RightTop, RightBottom, LeftBottom };

	public:
		Property<TextureRef>    TexturePtr;       // Texture property
		Property<RectI>         TexSrcRect;       // Texture source rectangle property
		Property<AssetId>       ImageId;          // Image asset id property
		Setter<String>          ImagePath;        // Sets image asset from path
		Setter<Ptr<ImageAsset>> Image;            // Sets image asset
		Setter<Ptr<Bitmap>>     Pixels;           // Sets image from bitmap
		Property<Color4>        LeftTopColor;	  // Color of left top corner property
		Property<Color4>        RightTopColor;	  // Color of right top corner property
		Property<Color4>        LeftBottomColor;  // Color of left bottom corner property
		Property<Color4>        RightBottomColor; // Color of right bottom corner property

		// Default constructor
		Sprite();

		// Constructor from image asset
		Sprite(Ptr<ImageAsset> image);

		// Constructor from image asset by path
		Sprite(const String& imagePath);

		// Constructor from image asset by id
		Sprite(AssetId imageId);

		// Constructor from texture and source rectangle
		Sprite(TextureRef texture, const RectI& srcRect);

		// Constructor from color
		Sprite(const Color4& color);

		// Constructor from bitmap
		Sprite(Ptr<Bitmap> bitmap);

		// Copy-constructor
		Sprite(const Sprite& other);

		// Destructor
		~Sprite();

		// Assign operator
		Sprite& operator=(const Sprite& other);

		// Returns cloned copy
		Sprite* Clone() const;

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

		// Loads sprite from image asset
		void LoadFromImage(Ptr<ImageAsset> image);

		// Loads sprite from image asset by path
		void LoadFromImage(const String& imagePath);

		// Loads sprite from image asset by id
		void LoadFromImage(AssetId imageId);

		// Loads sprite from mono color
		void LoadMonoColor(const Color4& color);

		// Loads sprite from bitmap
		void LoadFromBitmap(Ptr<Bitmap> bitmap);

		// Returns image asset id (returns 0 when asset id is unknown)
		AssetId GetImageId() const;

		// Sets size by texture source rectangle size
		void NormalizeSize();

		// Sets size with equal aspect as texture source rectangle by width
		void NormalizeAspectByWidth();

		// Sets size with equal aspect as texture source rectangle by width
		void NormalizeAspectByHeight();

		// Sets size with equal aspect as texture source rectangle by nearest value
		void NormalizeAspect();

	protected:
		RectI      mTextureSrcRect;   // Texture source rectangle
		Color4     mCornersColors[4]; // Corners colors
		AssetId    mImageAssetId;     // Image asset id (0 by default)

		Ptr<Mesh>  mMesh;             // Drawing mesh

	protected:
		// Calls when basis was changed
		void BasisChanged();

		// Calls when color was changed
		void ColorChanged();

		// Updates mesh geometry
		void UpdateMesh();

		// Initializes properties
		void InitializeProperties();
	};
}
