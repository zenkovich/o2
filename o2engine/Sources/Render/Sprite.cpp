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
		mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		UpdateMesh();
		InitializeProperties();
	}

	Sprite::Sprite(Ptr<ImageAsset> image):
		mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		LoadFromImage(image);
		InitializeProperties();
	}

	Sprite::Sprite(const String& imagePath):
		mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		LoadFromImage(imagePath);
		InitializeProperties();
	}

	Sprite::Sprite(AssetId imageId):
		mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		LoadFromImage(imageId);
		InitializeProperties();
	}

	Sprite::Sprite(TextureRef texture, const RectI& srcRect):
		mTextureSrcRect(srcRect), mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), 
		mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		UpdateMesh();
		InitializeProperties();
	}

	Sprite::Sprite(const Color4& color):
		mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		LoadMonoColor(color);
		InitializeProperties();
	}

	Sprite::Sprite(Ptr<Bitmap> bitmap):
		mImageAssetId(0), mMode(Mode::Default), mFill(1.0f), mMeshBuildFunc(&Sprite::BuildDefaultMesh)
	{
		mMesh = mnew Mesh(NoTexture(), 16, 18);
		for (int i = 0; i < 4; i++)
			mCornersColors[0] = Color4::White();

		LoadFromBitmap(bitmap);
		InitializeProperties();
	}

	Sprite::Sprite(const Sprite& other):
		mImageAssetId(other.mImageAssetId), mTextureSrcRect(other.mTextureSrcRect),
		IRectDrawable(other), mMesh(mnew Mesh(*other.mMesh)), mMode(other.mMode), mFill(other.mFill), mSlices(other.mSlices),
		mMeshBuildFunc(other.mMeshBuildFunc)
	{
		for (int i = 0; i < 4; i++)
			mCornersColors[i] = other.mCornersColors[i];

		InitializeProperties();
	}

	Sprite::~Sprite()
	{
		mMesh.Release();
	}

	Sprite& Sprite::operator=(const Sprite& other)
	{
		mMesh = mnew Mesh(*other.mMesh);
		mTextureSrcRect = other.mTextureSrcRect;
		mImageAssetId = other.mImageAssetId;
		mMode = other.mMode;
		mFill = other.mFill;
		mSlices = other.mSlices;
		mMeshBuildFunc = other.mMeshBuildFunc;
		IRectDrawable::operator=(other);

		return *this;
	}

	Sprite* Sprite::Clone() const
	{
		return mnew Sprite(*this);
	}

	void Sprite::Draw()
	{
		if (!mEnabled)
			return;

		mMesh->Draw();
		mDrawingDepth = mMesh->mDrawingDepth;
// 		o2Render.DrawMeshWire(mMesh, Color4(255, 255, 255, 100));
// 		o2Render.DrawBasis(mTransform);
	}

	void Sprite::SetTexture(TextureRef texture)
	{
		mMesh->SetTexture(texture);
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

	void Sprite::SetMode(Mode mode)
	{
		if (mode == mMode)
			return;

		mMode = mode;

		switch (mode)
		{
			case Mode::Default:         mMeshBuildFunc = &Sprite::BuildDefaultMesh; break;
			case Mode::Sliced:          mMeshBuildFunc = &Sprite::BuildSlicedMesh; break;
			case Mode::FillLeftToRight: mMeshBuildFunc = &Sprite::BuildFillLeftToRightMesh; break;
			case Mode::FillRightToLeft: mMeshBuildFunc = &Sprite::BuildFillRightToLeftMesh; break;
			case Mode::FillUpToDown:    mMeshBuildFunc = &Sprite::BuildFillUpToDownMesh; break;
			case Mode::FillDownToUp:    mMeshBuildFunc = &Sprite::BuildFillDownToUpMesh; break;
			case Mode::Fill360CW:       mMeshBuildFunc = &Sprite::BuildFill360CWMesh; break;
			case Mode::Fill360CCW:      mMeshBuildFunc = &Sprite::BuildFill360CCWMesh; break;
			default:                    mMeshBuildFunc = &Sprite::BuildDefaultMesh; break;
		}

		UpdateMesh();
	}

	Sprite::Mode Sprite::GetMode() const
	{
		return mMode;
	}

	void Sprite::SetSliceBorder(const RectI& border)
	{
		if (mSlices == border)
			return;

		mSlices = border;
		UpdateMesh();
	}

	RectI Sprite::GetSliceBorder() const
	{
		return mSlices;
	}

	void Sprite::LoadFromImage(Ptr<ImageAsset> image)
	{
		if (image)
		{
			mMesh->mTexture = AtlasAsset::GetPageTextureRef(image->GetAtlasId(), image->GetAtlasPage());
			mImageAssetId = image->GetAssetId();
			mTextureSrcRect = image->GetAtlasRect();
			SetSize(mTextureSrcRect.Size());
		}
		else o2Debug.LogWarningStr("Can't load sprite from image: image is null");
	}

	void Sprite::LoadFromImage(const String& imagePath)
	{
		if (o2Assets.IsAssetExist(imagePath))
		{
			ImageAsset image(imagePath);
			mMesh->mTexture = AtlasAsset::GetPageTextureRef(image.GetAtlasId(), image.GetAtlasPage());
			mImageAssetId = image.GetAssetId();
			mTextureSrcRect = image.GetAtlasRect();
			SetSize(mTextureSrcRect.Size());
		}
		else o2Debug.LogWarning("Can't load sprite from image by path (%s): image isn't exist", imagePath);
	}

	void Sprite::LoadFromImage(AssetId imageId)
	{
		if (o2Assets.IsAssetExist(imageId))
		{
			ImageAsset image(imageId);
			mMesh->mTexture = AtlasAsset::GetPageTextureRef(image.GetAtlasId(), image.GetAtlasPage());
			mImageAssetId = image.GetAssetId();
			mTextureSrcRect = image.GetAtlasRect();
			SetSize(mTextureSrcRect.Size());
		}
		else o2Debug.LogWarning("Can't create sprite from image by id (%i): image isn't exist", imageId);
	}

	void Sprite::LoadMonoColor(const Color4& color)
	{
		mImageAssetId = 0;
		mMesh->mTexture = TextureRef();
		mCornersColors[0] = color;
		mCornersColors[1] = color;
		mCornersColors[2] = color;
		mCornersColors[3] = color;
		UpdateMesh();
	}

	void Sprite::LoadFromBitmap(Ptr<Bitmap> bitmap)
	{
		if (bitmap)
		{
			mMesh->mTexture = TextureRef(bitmap);
			mTextureSrcRect.Set(Vec2F(), mMesh->mTexture->GetSize());
			SetSize(mMesh->mTexture->GetSize());
			mImageAssetId = 0;
		}
		else o2Debug.LogWarningStr("Can't create sprite from bitmap: bitmap is null");
	}

	AssetId Sprite::GetImageId() const
	{
		return mImageAssetId;
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

		static UInt16 indexes[] ={0, 1, 2, 0, 2, 3};

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->mVertices[0] = Vertex2(mTransform.offs + mTransform.yv, rcc[0], uvLeft, uvUp);
		mMesh->mVertices[1] = Vertex2(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->mVertices[2] = Vertex2(mTransform.offs + mTransform.xv, rcc[2], uvRight, uvDown);
		mMesh->mVertices[3] = Vertex2(mTransform.offs, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*6);

		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;
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

		float v0 = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float v3 = 1.0f - mTextureSrcRect.top*invTexSize.y;
		float v1 = Math::Lerp(v0, v3, mSlices.bottom*invTexSrcSize.y);
		float v2 = Math::Lerp(v0, v3, (texSrcSize.y - mSlices.top)*invTexSrcSize.y);

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

		mMesh->mVertices[0]  = Vertex2(o      + t3, rcc[0], u0, v3);
		mMesh->mVertices[1]  = Vertex2(o + r1 + t3, rcc[0], u1, v3);
		mMesh->mVertices[2]  = Vertex2(o + r2 + t3, rcc[1], u2, v3);
		mMesh->mVertices[3]  = Vertex2(o + r3 + t3, rcc[1], u3, v3);

		mMesh->mVertices[4]  = Vertex2(o      + t2, rcc[0], u0, v2);
		mMesh->mVertices[5]  = Vertex2(o + r1 + t2, rcc[0], u1, v2);
		mMesh->mVertices[6]  = Vertex2(o + r2 + t2, rcc[1], u2, v2);
		mMesh->mVertices[7]  = Vertex2(o + r3 + t2, rcc[1], u3, v2);

		mMesh->mVertices[8]  = Vertex2(o      + t1, rcc[3], u0, v1);
		mMesh->mVertices[9]  = Vertex2(o + r1 + t1, rcc[3], u1, v1);
		mMesh->mVertices[10] = Vertex2(o + r2 + t1, rcc[2], u2, v1);
		mMesh->mVertices[11] = Vertex2(o + r3 + t1, rcc[2], u3, v1);

		mMesh->mVertices[12] = Vertex2(o + Vec2F(), rcc[3], u0, v0);
		mMesh->mVertices[13] = Vertex2(o      + r1, rcc[3], u1, v0);
		mMesh->mVertices[14] = Vertex2(o      + r2, rcc[2], u2, v0);
		mMesh->mVertices[15] = Vertex2(o      + r3, rcc[2], u3, v0);

		memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*18*3);

		mMesh->mVertexCount = 16;
		mMesh->mPolyCount = 18;

		mTransform.xv = lastTransformXv;
		mSize.x = lastSizeX;
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

		static UInt16 indexes[] ={0, 1, 2, 0, 2, 3};

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = Math::Lerp((float)mTextureSrcRect.left, (float)mTextureSrcRect.right, coef)*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->mVertices[0] = Vertex2(mTransform.offs + mTransform.yv, rcc[0], uvLeft, uvUp);
		mMesh->mVertices[1] = Vertex2(mTransform.offs + mTransform.yv + mTransform.xv*coef, rcc[1], uvRight, uvUp);
		mMesh->mVertices[2] = Vertex2(mTransform.offs + mTransform.xv*coef, rcc[2], uvRight, uvDown);
		mMesh->mVertices[3] = Vertex2(mTransform.offs, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*6);

		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;
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

		static UInt16 indexes[] ={0, 1, 2, 0, 2, 3};

		float uvLeft = Math::Lerp((float)mTextureSrcRect.right, (float)mTextureSrcRect.left, coef)*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->mVertices[0] = Vertex2(mTransform.offs + mTransform.yv + mTransform.xv*invCoef, rcc[0], uvLeft, uvUp);
		mMesh->mVertices[1] = Vertex2(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->mVertices[2] = Vertex2(mTransform.offs + mTransform.xv, rcc[2], uvRight, uvDown);
		mMesh->mVertices[3] = Vertex2(mTransform.offs + mTransform.xv*invCoef, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*6);

		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;
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

		static UInt16 indexes[] ={0, 1, 2, 0, 2, 3};

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - mTextureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - Math::Lerp((float)mTextureSrcRect.bottom, (float)mTextureSrcRect.top, coef)*invTexSize.y;

		mMesh->mVertices[0] = Vertex2(mTransform.offs + mTransform.yv, rcc[0], uvLeft, uvUp);
		mMesh->mVertices[1] = Vertex2(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->mVertices[2] = Vertex2(mTransform.offs + mTransform.xv + mTransform.yv*invCoef, rcc[2], uvRight, uvDown);
		mMesh->mVertices[3] = Vertex2(mTransform.offs + mTransform.yv*invCoef, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*6);

		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;
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

		static UInt16 indexes[] ={0, 1, 2, 0, 2, 3};

		float uvLeft = mTextureSrcRect.left*invTexSize.x;
		float uvRight = mTextureSrcRect.right*invTexSize.x;

		float uvUp = 1.0f - Math::Lerp((float)mTextureSrcRect.top, (float)mTextureSrcRect.bottom, coef)*invTexSize.y;
		float uvDown = 1.0f - mTextureSrcRect.top*invTexSize.y;

		mMesh->mVertices[0] = Vertex2(mTransform.offs + mTransform.yv*coef, rcc[0], uvLeft, uvUp);
		mMesh->mVertices[1] = Vertex2(mTransform.offs + mTransform.yv*coef + mTransform.xv, rcc[1], uvRight, uvUp);
		mMesh->mVertices[2] = Vertex2(mTransform.offs + mTransform.xv, rcc[2], uvRight, uvDown);
		mMesh->mVertices[3] = Vertex2(mTransform.offs, rcc[3], uvLeft, uvDown);

		memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*6);

		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;
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

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(dirPoint, dirColor, uDir, vUp);
			mMesh->mVertices[2] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={0, 1, 2};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3);

			mMesh->mVertexCount = 3;
			mMesh->mPolyCount = 1;
		}
		else if (angle < 135.0f)
		{
			float dirCoef = 0.5f + dir.y/dir.x*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv + mTransform.yv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[2], mCornersColors[1], dirCoef)).ABGR();
			float vDir = Math::Lerp(vDown, vUp, dirCoef);

			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->mVertices[2] = Vertex2(dirPoint, dirColor, uRight, vDir);
			mMesh->mVertices[3] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={0, 1, 3, 1, 2, 3};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*2);

			mMesh->mVertexCount = 4;
			mMesh->mPolyCount = 2;
		}
		else if (angle < 225.0f)
		{
			float dirCoef = 0.5f - dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[2], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			Vec2F cornerPos1 = mTransform.offs + mTransform.yv + mTransform.xv;
			Vec2F cornerPos2 = mTransform.offs + mTransform.xv;

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->mVertices[2] = Vertex2(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->mVertices[3] = Vertex2(dirPoint, dirColor, uDir, vDown);
			mMesh->mVertices[4] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={0, 1, 4, 1, 2, 4, 2, 3, 4};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*3);

			mMesh->mVertexCount = 5;
			mMesh->mPolyCount = 3;
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

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->mVertices[2] = Vertex2(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->mVertices[3] = Vertex2(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->mVertices[4] = Vertex2(dirPoint, dirColor, uLeft, vDir);
			mMesh->mVertices[5] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={0, 1, 5, 1, 2, 5, 2, 3, 5, 3, 4, 5};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*4);

			mMesh->mVertexCount = 6;
			mMesh->mPolyCount = 4;
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

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos1, cornerResColr[0], uRight, vUp);
			mMesh->mVertices[2] = Vertex2(cornerPos2, cornerResColr[1], uRight, vDown);
			mMesh->mVertices[3] = Vertex2(cornerPos3, cornerResColr[2], uLeft, vDown);
			mMesh->mVertices[4] = Vertex2(cornerPos0, cornerResColr[3], uLeft, vUp);
			mMesh->mVertices[5] = Vertex2(dirPoint, dirColor, uDir, vUp);
			mMesh->mVertices[6] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={0, 1, 6, 1, 2, 6, 2, 3, 6, 3, 4, 6, 4, 5, 6};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*5);

			mMesh->mVertexCount = 7;
			mMesh->mPolyCount = 5;
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

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(dirPoint, dirColor, uDir, vUp);
			mMesh->mVertices[2] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={1, 0, 2};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3);

			mMesh->mVertexCount = 3;
			mMesh->mPolyCount = 1;
		}
		else if (angle < 135.0f)
		{
			float dirCoef = 0.5f - dir.y/dir.x*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.yv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[2], mCornersColors[1], dirCoef)).ABGR();
			float vDir = Math::Lerp(vDown, vUp, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->mVertices[2] = Vertex2(dirPoint, dirColor, uLeft, vDir);
			mMesh->mVertices[3] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={1, 0, 3, 2, 1, 3};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*2);

			mMesh->mVertexCount = 4;
			mMesh->mPolyCount = 2;
		}
		else if (angle < 225.0f)
		{
			float dirCoef = 0.5f - dir.x/dir.y*0.5f;
			Vec2F dirPoint = mTransform.offs + mTransform.xv*dirCoef;
			ULong dirColor = (mColor*Math::Lerp(mCornersColors[3], mCornersColors[2], dirCoef)).ABGR();
			float uDir = Math::Lerp(uLeft, uRight, dirCoef);

			Vec2F cornerPos0 = mTransform.offs + mTransform.yv;
			Vec2F cornerPos3 = mTransform.offs;

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->mVertices[2] = Vertex2(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->mVertices[3] = Vertex2(dirPoint, dirColor, uDir, vDown);
			mMesh->mVertices[4] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={1, 0, 4, 2, 1, 4, 3, 2, 4};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*3);

			mMesh->mVertexCount = 5;
			mMesh->mPolyCount = 3;
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

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->mVertices[2] = Vertex2(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->mVertices[3] = Vertex2(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->mVertices[4] = Vertex2(dirPoint, dirColor, uRight, vDir);
			mMesh->mVertices[5] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3, 5};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*4);

			mMesh->mVertexCount = 6;
			mMesh->mPolyCount = 4;
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

			mMesh->mVertices[0] = Vertex2(zeroPos, zeroResColor, uZero, vUp);
			mMesh->mVertices[1] = Vertex2(cornerPos0, cornerResColr[0], uLeft, vUp);
			mMesh->mVertices[2] = Vertex2(cornerPos3, cornerResColr[3], uLeft, vDown);
			mMesh->mVertices[3] = Vertex2(cornerPos2, cornerResColr[2], uRight, vDown);
			mMesh->mVertices[4] = Vertex2(cornerPos1, cornerResColr[1], uRight, vUp);
			mMesh->mVertices[5] = Vertex2(dirPoint, dirColor, uDir, vUp);
			mMesh->mVertices[6] = Vertex2(centerPos, centerResColr, uCenter, vCenter);

			static UInt16 indexes[] ={1, 0, 6, 2, 1, 6, 3, 2, 6, 4, 3, 6, 5, 4, 6};
			memcpy(mMesh->mIndexes, indexes, sizeof(UInt16)*3*5);

			mMesh->mVertexCount = 7;
			mMesh->mPolyCount = 5;
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
		INITIALIZE_PROPERTY(Sprite, sliceBorder, SetSliceBorder, GetSliceBorder);
	}
}