#pragma once

#include "Render/RectDrawable.h"
#include "Utils/Memory/Ptr.h"
#include "Assets/Asset.h"

namespace o2
{
	class Mesh;
	class Texture;
	class ImageAsset;
	class Bitmap;

	class Sprite: public IRectDrawable
	{
	public:
		enum class Corner { LeftTop, RightTop, RightBottom, LeftBottom };

	public:
		Property<Ptr<Texture>>  TexturePtr;
		Property<RectI>         TexSrcRect;
		Property<AssetId>       ImageId;
		Setter<String>          ImagePath;
		Setter<Ptr<ImageAsset>> Image;
		Setter<Ptr<Bitmap>>     Pixels;
		Property<Color4>        LeftTopColor;
		Property<Color4>        RightTopColor;
		Property<Color4>        LeftBottomColor;
		Property<Color4>        RightBottomColor;

		Sprite();
		Sprite(Ptr<ImageAsset> image);
		Sprite(const String& imagePath);
		Sprite(AssetId imageId);
		Sprite(Ptr<Texture> texture, const RectI& srcRect);
		Sprite(const Color4& color);
		Sprite(Ptr<Bitmap> bitmap);
		~Sprite();

		void SetTexture(Ptr<Texture> texture);
		Ptr<Texture> GetTexture() const;

		void SetTextureSrcRect(const RectI& rect);
		RectI GetTextureSrcRect() const;

		void SetCornerColor(Corner corner, const Color4& color);
		Color4 GetCornerColor(Corner corner) const;

		void SetLeftTopColor(const Color4& color);
		Color4 GetLeftTopCorner() const;

		void SetRightTopColor(const Color4& color);
		Color4 GetRightTopCorner() const;

		void SetRightBottomColor(const Color4& color);
		Color4 GetRightBottomCorner() const;

		void SetLeftBottomColor(const Color4& color);
		Color4 GetLeftBottomCorner() const;

		void LoadFromImage(Ptr<ImageAsset> image);

		void LoadFromImage(const String& imagePath);

		void LoadFromImage(AssetId imageId);

		void LoadMonoColor(const Color4& color);

		void LoadFromBitmap(Ptr<Bitmap> bitmap);

		AssetId GetImageId() const;

	protected:
		Ptr<Texture> mTexture;
		RectI        mTextureSrcRect;
		Color4       mCornersColors[4];
		AssetId      mImageAssetId;

		Ptr<Mesh>    mMesh;
	};
}
