#include "Sprite.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/ImageAsset.h"
#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Utils/Debug.h"

namespace o2
{
	Sprite::Sprite():
		mImageAssetId(0), mAtlasAssetId(0), mMode(SpriteMode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		UpdateMesh();
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(const ImageAsset& image):
		mImageAssetId(0), mAtlasAssetId(0), mMode(SpriteMode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		LoadFromImage(image);
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(const String& imagePath):
		mImageAssetId(0), mAtlasAssetId(0), mMode(SpriteMode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		LoadFromImage(imagePath);
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(UID imageId):
		mImageAssetId(0), mAtlasAssetId(0), mMode(SpriteMode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		LoadFromImage(imageId);
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(TextureRef texture, const RectI& srcRect):
		mTextureSrcRect(srcRect), mImageAssetId(0), mMode(SpriteMode::Default), mFill(1.0f),
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mAtlasAssetId(0), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		UpdateMesh();
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(const Color4& color):
		mImageAssetId(0), mAtlasAssetId(0), mMode(SpriteMode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		LoadMonoColor(color);
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(Bitmap* bitmap):
		mImageAssetId(0), mAtlasAssetId(0), mMode(SpriteMode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh), mTileScale(1.0f)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = Color4::White();

		LoadFromBitmap(bitmap);
		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::Sprite(const Sprite& other):
		mImageAssetId(other.mImageAssetId), mTextureSrcRect(other.mTextureSrcRect),
		IRectDrawable(other), mMesh(mnew Mesh(*other.mMesh)), mMode(other.mMode), mFill(other.mFill), mSlices(other.mSlices),
		mMeshBuildFunc(other.mMeshBuildFunc), mAtlasAssetId(other.mAtlasAssetId), mImageName(other.mImageName),
		mTileScale(other.mTileScale)
	{
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = other.mCornersColors[i];

		InitializeProperties();

		o2Render.mSprites.Add(this);
	}

	Sprite::~Sprite()
	{
		o2Render.mSprites.Remove(this);
		delete mMesh;
	}

	Sprite& Sprite::operator=(const Sprite& other)
	{
		mMesh = mnew Mesh(*other.mMesh);
		mTextureSrcRect = other.mTextureSrcRect;
		mImageAssetId = other.mImageAssetId;
		mAtlasAssetId = other.mAtlasAssetId;
		mMode = other.mMode;
		mFill = other.mFill;
		mSlices = other.mSlices;
		mImageName = other.mImageName;
		mTileScale = other.mTileScale;
		mMeshBuildFunc = other.mMeshBuildFunc;
		IRectDrawable::operator=(other);

		return *this;
	}

	void Sprite::Draw()
	{
		if (!mEnabled)
			return;

		mMesh->Draw();
		OnDrawn();
// 		o2Render.DrawMeshWire(mMesh, Color4(255, 255, 255, 100));
// 		o2Render.DrawBasis(mTransform);
	}

	void Sprite::SetTexture(TextureRef texture)
	{
		mMesh->SetTexture(texture);
		mImageName = texture->GetFileName();
	}

	TextureRef Sprite::GetTexture() const
	{
		return mMesh->GetTexture();
	}

	void Sprite::SetTextureSrcRect(const RectI& rect)
	{
		mTextureSrcRect = rect;
	}

	RectI Sprite::GetTextureSrcRect() const
	{
		return mTextureSrcRect;
	}

	Vec2I Sprite::GetOriginalSize() const
	{
		return mTextureSrcRect.Size();
	}

	void Sprite::SetCornerColor(Corner corner, const Color4& color)
	{
		mCornersColors[(int)corner] = color;
	}

	Color4 Sprite::GetCornerColor(Corner corner) const
	{
		return mCornersColors[(int)corner];
	}

	void Sprite::SetLeftTopColor(const Color4& color)
	{
		mCornersColors[(int)Corner::LeftTop] = color;
	}

	Color4 Sprite::GetLeftTopCorner() const
	{
		return mCornersColors[(int)Corner::LeftTop];
	}

	void Sprite::SetRightTopColor(const Color4& color)
	{
		mCornersColors[(int)Corner::RightTop] = color;
	}

	Color4 Sprite::GetRightTopCorner() const
	{
		return mCornersColors[(int)Corner::RightTop];
	}

	void Sprite::SetRightBottomColor(const Color4& color)
	{
		mCornersColors[(int)Corner::RightBottom] = color;
	}

	Color4 Sprite::GetRightBottomCorner() const
	{
		return mCornersColors[(int)Corner::RightBottom];
	}

	void Sprite::SetLeftBottomColor(const Color4& color)
	{
		mCornersColors[(int)Corner::LeftBottom] = color;
	}

	Color4 Sprite::GetLeftBottomCorner() const
	{
		return mCornersColors[(int)Corner::LeftBottom];
	}

	void Sprite::SetFill(float fill)
	{
		if (Math::Equals(mFill, fill))
			return;

		mFill = Math::Clamp01(fill);
		UpdateMesh();
	}

	float Sprite::GetFill() const
	{
		return mFill;
	}

	void Sprite::SetTileScale(float scale)
	{
		mTileScale = Math::Abs(scale);
		UpdateMesh();
	}

	float Sprite::GetTileScale() const
	{
		return mTileScale;
	}

	void Sprite::SetMode(SpriteMode mode)
	{
		if (mode == mMode)
			return;

		mMode = mode;

		switch (mode)
		{
		case SpriteMode::Default:         mMeshBuildFunc = &Sprite::BuildDefaultMesh; break;
		case SpriteMode::Sliced:          mMeshBuildFunc = &Sprite::BuildSlicedMesh; break;
		case SpriteMode::Tiled:           mMeshBuildFunc = &Sprite::BuildTiledMesh; break;
		case SpriteMode::FillLeftToRight: mMeshBuildFunc = &Sprite::BuildFillLeftToRightMesh; break;
		case SpriteMode::FillRightToLeft: mMeshBuildFunc = &Sprite::BuildFillRightToLeftMesh; break;
		case SpriteMode::FillUpToDown:    mMeshBuildFunc = &Sprite::BuildFillUpToDownMesh; break;
		case SpriteMode::FillDownToUp:    mMeshBuildFunc = &Sprite::BuildFillDownToUpMesh; break;
		case SpriteMode::Fill360CW:       mMeshBuildFunc = &Sprite::BuildFill360CWMesh; break;
		case SpriteMode::Fill360CCW:      mMeshBuildFunc = &Sprite::BuildFill360CCWMesh; break;
		default:                          mMeshBuildFunc = &Sprite::BuildDefaultMesh; break;
		}

		UpdateMesh();
	}

	SpriteMode Sprite::GetMode() const
	{
		return mMode;
	}

	void Sprite::SetSliceBorder(const BorderI& border)
	{
		if (mSlices == border)
			return;

		mSlices = border;
		UpdateMesh();
	}

	BorderI Sprite::GetSliceBorder() const
	{
		return mSlices;
	}

	void Sprite::LoadFromImage(const ImageAsset& image)
	{
		mMesh->mTexture = TextureRef(image.GetAtlasId(), image.GetAtlasPage());
		mAtlasAssetId = image.GetAtlasId();
		mImageAssetId = image.GetAssetId();
		mTextureSrcRect = image.GetAtlasRect();
		mSlices = image.GetMeta()->mSliceBorder;
		mImageName = image.GetPath();
		SetMode(image.GetMeta()->mDefaultMode);
		SetSize(mTextureSrcRect.Size());
	}

	void Sprite::LoadFromImage(const String& imagePath)
	{
		if (o2Assets.IsAssetExist(imagePath))
		{
			ImageAsset image(imagePath);
			mMesh->mTexture = TextureRef(image.GetAtlasId(), image.GetAtlasPage());
			mAtlasAssetId = image.GetAtlasId();
			mImageAssetId = image.GetAssetId();
			mTextureSrcRect = image.GetAtlasRect();
			mImageName = imagePath;
			mSlices = image.GetMeta()->mSliceBorder;
			SetMode(image.GetMeta()->mDefaultMode);
			SetSize(mTextureSrcRect.Size());
		}
		else o2Debug.LogWarning("Can't load sprite from image by path (%s): image isn't exist", imagePath);
	}

	void Sprite::LoadFromImage(UID imageId)
	{
		if (o2Assets.IsAssetExist(imageId))
		{
			ImageAsset image(imageId);
			mMesh->mTexture = TextureRef(image.GetAtlasId(), image.GetAtlasPage());
			mAtlasAssetId = image.GetAtlasId();
			mImageAssetId = image.GetAssetId();
			mTextureSrcRect = image.GetAtlasRect();
			mSlices = image.GetMeta()->mSliceBorder;
			mImageName = image.GetPath();
			SetMode(image.GetMeta()->mDefaultMode);
			SetSize(mTextureSrcRect.Size());
		}
		else o2Debug.LogWarning("Can't create sprite from image by id (%i): image isn't exist", imageId);
	}

	void Sprite::LoadMonoColor(const Color4& color)
	{
		mImageAssetId = 0;
		mAtlasAssetId = 0;
		mMesh->mTexture = TextureRef();
		mColor = color;
		mCornersColors[0] = Color4::White();
		mCornersColors[1] = Color4::White();
		mCornersColors[2] = Color4::White();
		mCornersColors[3] = Color4::White();
		mImageName = "";
		UpdateMesh();
	}

	void Sprite::LoadFromBitmap(Bitmap* bitmap)
	{
		if (bitmap)
		{
			mImageAssetId = 0;
			mAtlasAssetId = 0;
			mMesh->mTexture = TextureRef(bitmap);
			mTextureSrcRect.Set(Vec2F(), mMesh->mTexture->GetSize());
			mImageName = "";
			SetSize(mMesh->mTexture->GetSize());
		}
		else o2Debug.LogWarningStr("Can't create sprite from bitmap: bitmap is null");
	}

	UID Sprite::GetImageId() const
	{
		return mImageAssetId;
	}

	UID Sprite::GetAtlasAssetId() const
	{
		return mAtlasAssetId;
	}

	void Sprite::NormalizeSize()
	{
		SetSize(mTextureSrcRect.Size());
	}

	void Sprite::NormalizeAspectByWidth()
	{
		float aspect = (float)mTextureSrcRect.Width()/(float)mTextureSrcRect.Height();
		SetSize(Vec2F(mSize.x, mSize.x/aspect));
	}

	void Sprite::NormalizeAspectByHeight()
	{
		float aspect = (float)mTextureSrcRect.Width()/(float)mTextureSrcRect.Height();
		SetSize(Vec2F(mSize.y*aspect, mSize.y));
	}

	void Sprite::NormalizeAspect()
	{
		float aspect = (float)mTextureSrcRect.Width()/(float)mTextureSrcRect.Height();

		if (aspect > 1.0f)
			SetSize(Vec2F(mSize.y*aspect, mSize.y));
		else
			SetSize(Vec2F(mSize.x, mSize.x/aspect));
	}

	void Sprite::BasisChanged()
	{
		UpdateMesh();
	}

	void Sprite::ColorChanged()
	{
		UpdateMesh();
	}

	void Sprite::UpdateMesh()
	{
		(this->*mMeshBuildFunc)();
	}

	void Sprite::BuildDefaultMesh()
	{
		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		for (int i = 0; i < 4; i++)
			rcc[i] = (mColor*mCornersColors[i]).ABGR();

		static UInt16 indexes[] ={ 0, 1, 2, 0, 2, 3 };

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->vertices[0].Set(mTransform.offs + mTransform.yv, rcc[0], uvLeft, uvUp);
		mMesh->vertices[1].Set(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->vertices[2].Set(mTransform.offs + mTransform.xv, rcc[2], uvRight, uvDown);
		mMesh->vertices[3].Set(mTransform.offs, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->indexes, indexes, sizeof(UInt16)*6);

		mMesh->vertexCount = 4;
		mMesh->polyCount = 2;
	}

	void Sprite::BuildSlicedMesh()
	{
		Vec2F lastTransformXv = mTransform.xv;
		float lastSizeX = mSize.x;

		mTransform.xv *= mFill;
		mSize.x *= mFill;

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		for (int i = 0; i < 4; i++)
			rcc[i] = (mColor*mCornersColors[i]).ABGR();

		static UInt16 indexes[] ={
			0, 1, 5,    0, 5, 4,    1, 2, 6,    1, 6, 5,    2, 3, 7,    2, 7, 6,
			4, 5, 9,    4, 9, 8,    5, 6, 10,   5, 10, 9,   6, 7, 11,   6, 11, 10,
			8, 9, 13,   8, 13, 12,  9, 10, 14,  9, 14, 13,  10, 11, 15, 10, 15, 14
		};

		Vec2F texSrcSize = mTextureSrcRect.Size();
		Vec2F invTexSrcSize = Vec2F::One()/texSrcSize;
		Vec2F sz = mSize*mScale;

		float u0 = mTextureSrcRect.left*invTexSize.x;
		float u3 = mTextureSrcRect.right*invTexSize.x;
		float u1 = Math::Lerp(u0, u3, mSlices.left*invTexSrcSize.x);
		float u2 = Math::Lerp(u0, u3, (texSrcSize.x - mSlices.right)*invTexSrcSize.x);

		float v0 = 1.0f - mTextureSrcRect.top*invTexSize.y;
		float v3 = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float v1 = Math::Lerp(v0, v3, mSlices.top*invTexSrcSize.y);
		float v2 = Math::Lerp(v0, v3, (texSrcSize.y - mSlices.bottom)*invTexSrcSize.y);

		Vec2F xv = mTransform.xv/sz.x;
		Vec2F yv = mTransform.yv/sz.y;

		Vec2F o = mTransform.offs;

		Vec2F r1 = xv*(float)mSlices.left;
		Vec2F r2 = xv*(sz.x - (float)mSlices.right);
		Vec2F r3 = mTransform.xv;

		Vec2F t1 = yv*(float)mSlices.bottom;
		Vec2F t2 = yv*(sz.y - (float)mSlices.top);
		Vec2F t3 = mTransform.yv;

		if (mSlices.left + mSlices.right > sz.x)
		{
			float d = (mSlices.left + mSlices.right - sz.x)*0.5f;
			r1 -= xv*d;
			r2 += xv*d;

			u1 = Math::Lerp(u0, u3, (mSlices.left - d)*invTexSrcSize.x);
			u2 = Math::Lerp(u0, u3, (texSrcSize.x - mSlices.right + d)*invTexSrcSize.x);
		}

		if (mSlices.top + mSlices.bottom > sz.y)
		{
			float d = (mSlices.top + mSlices.bottom - sz.y)*0.5f;
			t1 -= yv*d;
			t2 += yv*d;

			v1 = Math::Lerp(v0, v3, (mSlices.bottom - d)*invTexSrcSize.y);
			v2 = Math::Lerp(v0, v3, (texSrcSize.y - mSlices.top + d)*invTexSrcSize.y);
		}

		mMesh->vertices[0].Set(o      + t3, rcc[0], u0, v3);
		mMesh->vertices[1].Set(o + r1 + t3, rcc[0], u1, v3);
		mMesh->vertices[2].Set(o + r2 + t3, rcc[1], u2, v3);
		mMesh->vertices[3].Set(o + r3 + t3, rcc[1], u3, v3);

		mMesh->vertices[4].Set(o      + t2, rcc[0], u0, v2);
		mMesh->vertices[5].Set(o + r1 + t2, rcc[0], u1, v2);
		mMesh->vertices[6].Set(o + r2 + t2, rcc[1], u2, v2);
		mMesh->vertices[7].Set(o + r3 + t2, rcc[1], u3, v2);

		mMesh->vertices[8].Set(o      + t1, rcc[3], u0, v1);
		mMesh->vertices[9].Set(o + r1 + t1, rcc[3], u1, v1);
		mMesh->vertices[10].Set(o + r2 + t1, rcc[2], u2, v1);
		mMesh->vertices[11].Set(o + r3 + t1, rcc[2], u3, v1);

		mMesh->vertices[12].Set(o + Vec2F(), rcc[3], u0, v0);
		mMesh->vertices[13].Set(o      + r1, rcc[3], u1, v0);
		mMesh->vertices[14].Set(o      + r2, rcc[2], u2, v0);
		mMesh->vertices[15].Set(o      + r3, rcc[2], u3, v0);

		memcpy(mMesh->indexes, indexes, sizeof(UInt16)*18*3);

		mMesh->vertexCount = 16;
		mMesh->polyCount = 18;

		mTransform.xv = lastTransformXv;
		mSize.x = lastSizeX;
	}

	void Sprite::BuildTiledMesh()
	{
		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		for (int i = 0; i < 4; i++)
			rcc[i] = (mColor*mCornersColors[i]).ABGR();

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		Vec2F tileSize = (Vec2F)(mTextureSrcRect.Size())*mTileScale;
		Vec2F sz = mSize*mScale;

		Vec2I tilesCount(Math::CeilToInt(sz.x/tileSize.x), Math::CeilToInt(sz.y/tileSize.y));

		UInt requiredPolygons = tilesCount.x*tilesCount.y*2;
		UInt requiredVecticiesCount = requiredPolygons*2;

		if (mMesh->GetMaxVertexCount() < requiredVecticiesCount || mMesh->GetMaxPolyCount() < requiredPolygons)
			mMesh->Resize(requiredVecticiesCount, requiredPolygons);

		Vec2F xv = mTransform.xv/sz.x;
		Vec2F yv = mTransform.yv/sz.y;

		Vec2F o = mTransform.offs;

		int vi = 0, pi = 0;

		float px0 = 0;
		for (int x = 0; x < tilesCount.x; x++)
		{
			float px = (float)(x + 1)*tileSize.x;
			float u = uvRight;

			if (px > sz.x)
			{
				u = Math::Lerp(uvRight, uvLeft, (px - sz.x)/tileSize.x);
				px = sz.x;
			}

			float py0 = 0;
			for (int y = 0; y < tilesCount.y; y++)
			{
				float py = (float)(y + 1)*tileSize.y;
				float v = uvUp;

				if (py > sz.y)
				{
					v = Math::Lerp(uvUp, uvDown, (py - sz.y)/tileSize.y);
					py = sz.y;
				}

				int vii = vi;
				mMesh->vertices[vi++].Set(o + xv*px0 + yv*py,  rcc[0], uvLeft, v);
				mMesh->vertices[vi++].Set(o + xv*px  + yv*py,  rcc[1], u, v);
				mMesh->vertices[vi++].Set(o + xv*px  + yv*py0, rcc[2], u, uvDown);
				mMesh->vertices[vi++].Set(o + xv*px0 + yv*py0, rcc[3], uvLeft, uvDown);

				mMesh->indexes[pi++] = vii; mMesh->indexes[pi++] = vii + 1; mMesh->indexes[pi++] = vii + 2;
				mMesh->indexes[pi++] = vii; mMesh->indexes[pi++] = vii + 2; mMesh->indexes[pi++] = vii + 3;

				py0 = py;
			}

			px0 = px;
		}

		mMesh->vertexCount = vi;
		mMesh->polyCount = pi/3;
	}

	void Sprite::BuildFillLeftToRightMesh()
	{
		float coef = Math::Clamp01(mFill);

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		rcc[0] = (mColor*mCornersColors[0]).ABGR();
		rcc[1] = (mColor*Math::Lerp(mCornersColors[0], mCornersColors[1], coef)).ABGR();
		rcc[2] = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[2], coef)).ABGR();
		rcc[3] = (mColor*mCornersColors[3]).ABGR();

		static UInt16 indexes[] ={ 0, 1, 2, 0, 2, 3 };

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = Math::Lerp((float)mTextureSrcRect.left, (float)mTextureSrcRect.right, coef)*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->vertices[0].Set(mTransform.offs + mTransform.yv, rcc[0], uvLeft, uvUp);
		mMesh->vertices[1].Set(mTransform.offs + mTransform.yv + mTransform.xv*coef, rcc[1], uvRight, uvUp);
		mMesh->vertices[2].Set(mTransform.offs + mTransform.xv*coef, rcc[2], uvRight, uvDown);
		mMesh->vertices[3].Set(mTransform.offs, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->indexes, indexes, sizeof(UInt16)*6);

		mMesh->vertexCount = 4;
		mMesh->polyCount = 2;
	}

	void Sprite::BuildFillRightToLeftMesh()
	{
		float coef = Math::Clamp01(mFill);
		float invCoef = 1.0f - coef;

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		rcc[0] = (mColor*Math::Lerp(mCornersColors[1], mCornersColors[0], coef)).ABGR();
		rcc[1] = (mColor*mCornersColors[1]).ABGR();
		rcc[2] = (mColor*mCornersColors[2]).ABGR();
		rcc[3] = (mColor*Math::Lerp(mCornersColors[2], mCornersColors[3], coef)).ABGR();

		static UInt16 indexes[] ={ 0, 1, 2, 0, 2, 3 };

		float uvLeft = Math::Lerp((float)mTextureSrcRect.right, (float)mTextureSrcRect.left, coef)*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->vertices[0].Set(mTransform.offs + mTransform.yv + mTransform.xv*invCoef, rcc[0], uvLeft, uvUp);
		mMesh->vertices[1].Set(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->vertices[2].Set(mTransform.offs + mTransform.xv, rcc[2], uvRight, uvDown);
		mMesh->vertices[3].Set(mTransform.offs + mTransform.xv*invCoef, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->indexes, indexes, sizeof(UInt16)*6);

		mMesh->vertexCount = 4;
		mMesh->polyCount = 2;
	}

	void Sprite::BuildFillUpToDownMesh()
	{
		float coef = Math::Clamp01(mFill);
		float invCoef = 1.0f - coef;

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		rcc[0] = (mColor*mCornersColors[0]).ABGR();
		rcc[1] = (mColor*mCornersColors[1]).ABGR();
		rcc[2] = (mColor*Math::Lerp(mCornersColors[1], mCornersColors[2], coef)).ABGR();
		rcc[3] = (mColor*Math::Lerp(mCornersColors[0], mCornersColors[3], coef)).ABGR();

		static UInt16 indexes[] ={ 0, 1, 2, 0, 2, 3 };

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - Math::Lerp((float)mTextureSrcRect.bottom, (float)mTextureSrcRect.top, coef)*invTexSize.y;

		mMesh->vertices[0].Set(mTransform.offs + mTransform.yv, rcc[0], uvLeft, uvUp);
		mMesh->vertices[1].Set(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->vertices[2].Set(mTransform.offs + mTransform.xv + mTransform.yv*invCoef, rcc[2], uvRight, uvDown);
		mMesh->vertices[3].Set(mTransform.offs + mTransform.yv*invCoef, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->indexes, indexes, sizeof(UInt16)*6);

		mMesh->vertexCount = 4;
		mMesh->polyCount = 2;
	}

	void Sprite::BuildFillDownToUpMesh()
	{
		float coef = Math::Clamp01(mFill);

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong rcc[4];
		rcc[0] = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[0], coef)).ABGR();
		rcc[1] = (mColor*Math::Lerp(mCornersColors[2], mCornersColors[1], coef)).ABGR();
		rcc[2] = (mColor*mCornersColors[2]).ABGR();
		rcc[3] = (mColor*mCornersColors[3]).ABGR();

		static UInt16 indexes[] ={ 0, 1, 2, 0, 2, 3 };

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - Math::Lerp((float)mTextureSrcRect.top, (float)mTextureSrcRect.bottom, coef)*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->vertices[0].Set(mTransform.offs + mTransform.yv*coef, rcc[0], uvLeft, uvUp);
		mMesh->vertices[1].Set(mTransform.offs + mTransform.yv*coef + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->vertices[2].Set(mTransform.offs + mTransform.xv, rcc[2], uvRight, uvDown);
		mMesh->vertices[3].Set(mTransform.offs, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->indexes, indexes, sizeof(UInt16)*6);

		mMesh->vertexCount = 4;
		mMesh->polyCount = 2;
	}

	void Sprite::BuildFill360CWMesh()
	{
		float coef = Math::Clamp01(mFill);
		float angle = 360.0f*coef;

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong cornerResColr[4];
		for (int i = 0; i < 4; i++)
			cornerResColr[i] = (mColor*mCornersColors[i]).ABGR();

		float uLeft = mTextureSrcRect.left*invTexSize.x;
		float uRight = mTextureSrcRect.right*invTexSize.x;

		float vUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float vDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		Vec2F zeroPos    = mTransform.offs + mTransform.xv*0.5f + mTransform.yv;
		Vec2F centerPos  = mTransform.offs + mTransform.xv*0.5f + mTransform.yv*0.5f;

		ULong centerResColr = (mColor*((mCornersColors[0] + mCornersColors[1] + mCornersColors[2] + mCornersColors[3])*0.25f)).ABGR();
		ULong zeroResColor = (mColor*((mCornersColors[0] + mCornersColors[1])*0.5f)).ABGR();
		float uZero = (uLeft + uRight)*0.5f;
		float uCenter = uZero;
		float vCenter = (vUp + vDown)*0.5f;

		Vec2F dir = Vec2F::Rotated(Math::Deg2rad(-angle + 90.0f));
		if (angle < 45.0f)
		{
			float dirCoef = 0.5f + dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef + mTransform.yv;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[0], mCornersColors[1], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(dirPoint, dirColor, uDir, vUp);
			mMesh->vertices[2].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 0, 1, 2 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3);

			mMesh->vertexCount = 3;
			mMesh->polyCount = 1;
		}
		else if (angle < 135.0f)
		{
			float dirCoef = 0.5f + dir.y/dir.x*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv + mTransform.yv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[2], mCornersColors[1], dirCoef)).ABGR();
			float vDir = Math::Lerp(vDown, vUp, dirCoef);

			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->vertices[2].Set(dirPoint, dirColor, uRight, vDir);
			mMesh->vertices[3].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 0, 1, 3, 1, 2, 3 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*2);

			mMesh->vertexCount = 4;
			mMesh->polyCount = 2;
		}
		else if (angle < 225.0f)
		{
			float dirCoef = 0.5f - dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[2], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;
			Vec2F cornerPos2 = mTransform.offs + mTransform.xv;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->vertices[2].Set(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->vertices[3].Set(dirPoint, dirColor, uDir, vDown);
			mMesh->vertices[4].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 0, 1, 4, 1, 2, 4, 2, 3, 4 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*3);

			mMesh->vertexCount = 5;
			mMesh->polyCount = 3;
		}
		else if (angle < 315.0f)
		{
			float dirCoef = 0.5f - dir.y/dir.x*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.yv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[0], dirCoef)).ABGR();
			float vDir = Math::Lerp(vDown, vUp, dirCoef);

			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;
			Vec2F cornerPos2 = mTransform.offs + mTransform.xv;
			Vec2F cornerPos3 = mTransform.offs;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->vertices[2].Set(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->vertices[3].Set(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->vertices[4].Set(dirPoint, dirColor, uLeft, vDir);
			mMesh->vertices[5].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 0, 1, 5, 1, 2, 5, 2, 3, 5, 3, 4, 5 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*4);

			mMesh->vertexCount = 6;
			mMesh->polyCount = 4;
		}
		else
		{
			float dirCoef = 0.5f + dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef + mTransform.yv;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[0], mCornersColors[1], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;
			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;
			Vec2F cornerPos2 = mTransform.offs + mTransform.xv;
			Vec2F cornerPos3 = mTransform.offs;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos1, cornerResColr[0], uRight, vUp);
			mMesh->vertices[2].Set(cornerPos2, cornerResColr[1], uRight, vDown);
			mMesh->vertices[3].Set(cornerPos3, cornerResColr[2], uLeft, vDown);
			mMesh->vertices[4].Set(cornerPos0, cornerResColr[3], uLeft, vUp);
			mMesh->vertices[5].Set(dirPoint, dirColor, uDir, vUp);
			mMesh->vertices[6].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 0, 1, 6, 1, 2, 6, 2, 3, 6, 3, 4, 6, 4, 5, 6 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*5);

			mMesh->vertexCount = 7;
			mMesh->polyCount = 5;
		}
	}

	void Sprite::BuildFill360CCWMesh()
	{
		float coef = Math::Clamp01(mFill);
		float angle = 360.0f*coef;

		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		ULong cornerResColr[4];
		for (int i = 0; i < 4; i++)
			cornerResColr[i] = (mColor*mCornersColors[i]).ABGR();

		float uLeft = mTextureSrcRect.left*invTexSize.x;
		float uRight = mTextureSrcRect.right*invTexSize.x;

		float vUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float vDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		Vec2F zeroPos   = mTransform.offs + mTransform.xv*0.5f + mTransform.yv;
		Vec2F centerPos = mTransform.offs + mTransform.xv*0.5f + mTransform.yv*0.5f;

		ULong centerResColr = (mColor*((mCornersColors[0] + mCornersColors[1] + mCornersColors[2] + mCornersColors[3])*0.25f)).ABGR();
		ULong zeroResColor = (mColor*((mCornersColors[0] + mCornersColors[1])*0.5f)).ABGR();
		float uZero = (uLeft + uRight)*0.5f;
		float uCenter = uZero;
		float vCenter = (vUp + vDown)*0.5f;

		Vec2F dir = Vec2F::Rotated(Math::Deg2rad(angle + 90.0f));
		if (angle < 45.0f)
		{
			float dirCoef = 0.5f + dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef + mTransform.yv;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[0], mCornersColors[1], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(dirPoint, dirColor, uDir, vUp);
			mMesh->vertices[2].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 1, 0, 2 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3);

			mMesh->vertexCount = 3;
			mMesh->polyCount = 1;
		}
		else if (angle < 135.0f)
		{
			float dirCoef = 0.5f - dir.y/dir.x*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.yv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[2], mCornersColors[1], dirCoef)).ABGR();
			float vDir = Math::Lerp(vDown, vUp, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->vertices[2].Set(dirPoint, dirColor, uLeft, vDir);
			mMesh->vertices[3].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 1, 0, 3, 2, 1, 3 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*2);

			mMesh->vertexCount = 4;
			mMesh->polyCount = 2;
		}
		else if (angle < 225.0f)
		{
			float dirCoef = 0.5f - dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[2], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;
			Vec2F cornerPos3 = mTransform.offs;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->vertices[2].Set(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->vertices[3].Set(dirPoint, dirColor, uDir, vDown);
			mMesh->vertices[4].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 1, 0, 4, 2, 1, 4, 3, 2, 4 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*3);

			mMesh->vertexCount = 5;
			mMesh->polyCount = 3;
		}
		else if (angle < 315.0f)
		{
			float dirCoef = 0.5f + dir.y/dir.x*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.yv*dirCoef + mTransform.xv;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[0], dirCoef)).ABGR();
			float vDir = Math::Lerp(vDown, vUp, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;
			Vec2F cornerPos3 = mTransform.offs;
			Vec2F cornerPos2 = mTransform.offs + mTransform.xv;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->vertices[2].Set(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->vertices[3].Set(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->vertices[4].Set(dirPoint, dirColor, uRight, vDir);
			mMesh->vertices[5].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3, 5 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*4);

			mMesh->vertexCount = 6;
			mMesh->polyCount = 4;
		}
		else
		{
			float dirCoef = 0.5f + dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef + mTransform.yv;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[0], mCornersColors[1], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;
			Vec2F cornerPos3 = mTransform.offs;
			Vec2F cornerPos2 = mTransform.offs + mTransform.xv;
			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;

			mMesh->vertices[0].Set(zeroPos, zeroResColor, uZero, vUp);
			mMesh->vertices[1].Set(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->vertices[2].Set(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->vertices[3].Set(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->vertices[4].Set(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->vertices[5].Set(dirPoint, dirColor, uDir, vUp);
			mMesh->vertices[6].Set(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={ 1, 0, 6, 2, 1, 6, 3, 2, 6, 4, 3, 6, 5, 4, 6 };
			memcpy(mMesh->indexes, indexes, sizeof(UInt16)*3*5);

			mMesh->vertexCount = 7;
			mMesh->polyCount = 5;
		}
	}

	void Sprite::OnSerialize(DataNode& node) const
	{
		if (mImageAssetId != 0)
			node["mImageAssetId"] = mImageAssetId;
		else
		{
			node["mTextureSrcRect"] = mTextureSrcRect;

			if (mMesh->GetTexture())
				node["textureFileName"] = mMesh->GetTexture()->GetFileName();
			else
				node["textureFileName"] = String();
		}
	}

	void Sprite::OnDeserialized(const DataNode& node)
	{
		auto imageAssedIdNode = node.GetNode("mImageAssetId");

		if (!imageAssedIdNode)
		{
			String textureFileName = *node.GetNode("textureFileName");
			if (textureFileName.IsEmpty())
				mMesh->SetTexture(NoTexture());
			else
				mMesh->SetTexture(TextureRef(textureFileName));

			mTextureSrcRect = *node.GetNode("mTextureSrcRect");
		}
		else
		{
			mImageAssetId = (UID)*imageAssedIdNode;
			LoadFromImage(mImageAssetId);
		}

		SpriteMode mode = mMode;
		mMode = (SpriteMode)((int)mode + 1);
		SetMode(mode);
	}

	void Sprite::ReloadImage()
	{
		if (mImageAssetId != 0)
		{
			ImageAsset image(mImageAssetId);
			mMesh->mTexture = TextureRef(image.GetAtlasId(), image.GetAtlasPage());
			mAtlasAssetId = image.GetAtlasId();
			mImageAssetId = image.GetAssetId();
			mTextureSrcRect = image.GetAtlasRect();
			mSlices = image.GetMeta()->mSliceBorder;
			mImageName = image.GetPath();

			UpdateMesh();
		}
	}

	void Sprite::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Sprite, texture, SetTexture, GetTexture);
		INITIALIZE_PROPERTY(Sprite, textureSrcRect, SetTextureSrcRect, GetTextureSrcRect);
		INITIALIZE_PROPERTY(Sprite, imageAssetId, LoadFromImage, GetImageId);
		INITIALIZE_SETTER(Sprite, imageAssetPath, LoadFromImage);
		INITIALIZE_SETTER(Sprite, imageAsset, LoadFromImage);
		INITIALIZE_SETTER(Sprite, bitmap, LoadFromBitmap);
		INITIALIZE_PROPERTY(Sprite, leftTopColor, SetLeftTopColor, GetLeftTopCorner);
		INITIALIZE_PROPERTY(Sprite, rightTopColor, SetRightTopColor, GetRightTopCorner);
		INITIALIZE_PROPERTY(Sprite, leftBottomColor, SetLeftBottomColor, GetLeftBottomCorner);
		INITIALIZE_PROPERTY(Sprite, rightBottomColor, SetRightBottomColor, GetRightBottomCorner);
		INITIALIZE_PROPERTY(Sprite, mode, SetMode, GetMode);
		INITIALIZE_PROPERTY(Sprite, fill, SetFill, GetFill);
		INITIALIZE_PROPERTY(Sprite, tileScale, SetTileScale, GetTileScale);
		INITIALIZE_PROPERTY(Sprite, sliceBorder, SetSliceBorder, GetSliceBorder);
	}
}
 
CLASS_META(o2::Sprite)
{
	BASE_CLASS(o2::IRectDrawable);

	PUBLIC_FIELD(texture);
	PUBLIC_FIELD(textureSrcRect);
	PUBLIC_FIELD(imageAssetId);
	PUBLIC_FIELD(imageAssetPath);
	PUBLIC_FIELD(imageAsset);
	PUBLIC_FIELD(bitmap);
	PUBLIC_FIELD(leftTopColor);
	PUBLIC_FIELD(rightTopColor);
	PUBLIC_FIELD(leftBottomColor);
	PUBLIC_FIELD(rightBottomColor);
	PUBLIC_FIELD(mode);
	PUBLIC_FIELD(fill);
	PUBLIC_FIELD(tileScale);
	PUBLIC_FIELD(sliceBorder);
	PROTECTED_FIELD(mTextureSrcRect);
	PROTECTED_FIELD(mCornersColors);
	PROTECTED_FIELD(mImageAssetId);
	PROTECTED_FIELD(mAtlasAssetId);
	PROTECTED_FIELD(mImageName);
	PROTECTED_FIELD(mMode).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSlices).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFill).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTileScale).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMesh);
	PROTECTED_FIELD(mMeshBuildFunc);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetTexture, TextureRef);
	PUBLIC_FUNCTION(TextureRef, GetTexture);
	PUBLIC_FUNCTION(void, SetTextureSrcRect, const RectI&);
	PUBLIC_FUNCTION(RectI, GetTextureSrcRect);
	PUBLIC_FUNCTION(Vec2I, GetOriginalSize);
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
	PUBLIC_FUNCTION(void, SetFill, float);
	PUBLIC_FUNCTION(float, GetFill);
	PUBLIC_FUNCTION(void, SetTileScale, float);
	PUBLIC_FUNCTION(float, GetTileScale);
	PUBLIC_FUNCTION(void, SetMode, SpriteMode);
	PUBLIC_FUNCTION(SpriteMode, GetMode);
	PUBLIC_FUNCTION(void, SetSliceBorder, const BorderI&);
	PUBLIC_FUNCTION(BorderI, GetSliceBorder);
	PUBLIC_FUNCTION(void, LoadFromImage, const ImageAsset&);
	PUBLIC_FUNCTION(void, LoadFromImage, const String&);
	PUBLIC_FUNCTION(void, LoadFromImage, UID);
	PUBLIC_FUNCTION(void, LoadMonoColor, const Color4&);
	PUBLIC_FUNCTION(void, LoadFromBitmap, Bitmap*);
	PUBLIC_FUNCTION(UID, GetImageId);
	PUBLIC_FUNCTION(UID, GetAtlasAssetId);
	PUBLIC_FUNCTION(void, NormalizeSize);
	PUBLIC_FUNCTION(void, NormalizeAspectByWidth);
	PUBLIC_FUNCTION(void, NormalizeAspectByHeight);
	PUBLIC_FUNCTION(void, NormalizeAspect);
	PROTECTED_FUNCTION(void, BasisChanged);
	PROTECTED_FUNCTION(void, ColorChanged);
	PROTECTED_FUNCTION(void, UpdateMesh);
	PROTECTED_FUNCTION(void, BuildDefaultMesh);
	PROTECTED_FUNCTION(void, BuildSlicedMesh);
	PROTECTED_FUNCTION(void, BuildTiledMesh);
	PROTECTED_FUNCTION(void, BuildFillLeftToRightMesh);
	PROTECTED_FUNCTION(void, BuildFillRightToLeftMesh);
	PROTECTED_FUNCTION(void, BuildFillUpToDownMesh);
	PROTECTED_FUNCTION(void, BuildFillDownToUpMesh);
	PROTECTED_FUNCTION(void, BuildFill360CWMesh);
	PROTECTED_FUNCTION(void, BuildFill360CCWMesh);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, ReloadImage);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 