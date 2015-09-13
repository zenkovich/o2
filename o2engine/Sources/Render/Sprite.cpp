#include "Sprite.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/ImageAsset.h"
#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Utils/Debug.h"

namespace o2
{
	UInt16 SpriteMeshIndexes[6] ={0, 1, 2, 0, 2, 3};

	Sprite::Sprite():
		mImageAssetId(0)
	{
		mMesh = mnew Mesh();
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		UpdateMesh();
		InitializeProperties();
	}

	Sprite::Sprite(Ptr<ImageAsset> image):
		mImageAssetId(0)
	{
		mMesh = mnew Mesh();
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		LoadFromImage(image);
		InitializeProperties();
	}

	Sprite::Sprite(const String& imagePath):
		mImageAssetId(0)
	{
		mMesh = mnew Mesh();
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		LoadFromImage(imagePath);
		InitializeProperties();
	}

	Sprite::Sprite(AssetId imageId):
		mImageAssetId(0)
	{
		mMesh = mnew Mesh();
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		LoadFromImage(imageId);
		InitializeProperties();
	}

	Sprite::Sprite(TextureRef texture, const RectI& srcRect):
		mTextureSrcRect(srcRect), mImageAssetId(0)
	{
		mMesh = mnew Mesh(texture);
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		UpdateMesh();
		InitializeProperties();
	}

	Sprite::Sprite(const Color4& color):
		mImageAssetId(0)
	{
		mMesh = mnew Mesh();
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		LoadMonoColor(color);
		InitializeProperties();
	}

	Sprite::Sprite(Ptr<Bitmap> bitmap):
		mImageAssetId(0)
	{
		mMesh = mnew Mesh();
		memcpy(mMesh->mIndexes, SpriteMeshIndexes, 6*sizeof(UInt16));
		mMesh->mVertexCount = 4;
		mMesh->mPolyCount = 2;

		LoadFromBitmap(bitmap);
		InitializeProperties();
	}

	Sprite::Sprite(const Sprite& other):
		mImageAssetId(other.mImageAssetId), mTextureSrcRect(other.mTextureSrcRect),
		IRectDrawable(other), mMesh(other.mMesh)
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
		mMesh = other.mMesh;
		mTextureSrcRect = other.mTextureSrcRect;
		mImageAssetId = other.mImageAssetId;
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

		o2Render.DrawLine(mTransform.offs, mTransform.offs + mTransform.xv, Color4::Red());
		o2Render.DrawLine(mTransform.offs, mTransform.offs + mTransform.yv, Color4::Blue());
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

	void Sprite::LoadFromImage(Ptr<ImageAsset> image)
	{
		if (image)
		{
			mMesh->mTexture = AtlasAsset::GetPageTextureRef(image->GetAtlasId(), image->GetAtlasPage());
			mImageAssetId = image->GetAssetId();
			mTextureSrcRect = image->GetAtlasRect();
			SetSize(mTextureSrcRect.Size());
		}
		else Debug.LogWarningStr("Can't load sprite from image: image is null");
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
		else Debug.LogWarning("Can't load sprite from image by path (%s): image isn't exist", imagePath);
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
		else Debug.LogWarning("Can't create sprite from image by id (%i): image isn't exist", imageId);
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
		else Debug.LogWarningStr("Can't create sprite from bitmap: bitmap is null");
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
		Vec2F invTexSize(1.0f, 1.0f);
		if (mMesh->mTexture)
			invTexSize.Set(1.0f/mMesh->mTexture->GetSize().x, 1.0f/mMesh->mTexture->GetSize().y);

		Color4 rcc[4];
		for (int i = 0; i < 4; i++)
			rcc[i] = mColor*mCornersColors[i];

		mMesh->mVertices[0] = Vertex2(mTransform.offs + mTransform.yv, rcc[0].ABGR(), mTextureSrcRect.left*invTexSize.x, 1.0f - mTextureSrcRect.bottom*invTexSize.y);
		mMesh->mVertices[1] = Vertex2(mTransform.offs + mTransform.yv + mTransform.xv, rcc[1].ABGR(), mTextureSrcRect.right*invTexSize.x, 1.0f - mTextureSrcRect.bottom*invTexSize.y);
		mMesh->mVertices[2] = Vertex2(mTransform.offs + mTransform.xv, rcc[2].ABGR(), mTextureSrcRect.right*invTexSize.x, 1.0f - mTextureSrcRect.top*invTexSize.y);
		mMesh->mVertices[3] = Vertex2(mTransform.offs, rcc[3].ABGR(), mTextureSrcRect.left*invTexSize.x, 1.0f - mTextureSrcRect.top*invTexSize.y);
	}

	void Sprite::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Sprite, TexturePtr, SetTexture, GetTexture);
		INITIALIZE_PROPERTY(Sprite, TexSrcRect, SetTextureSrcRect, GetTextureSrcRect);
		INITIALIZE_PROPERTY(Sprite, ImageId, LoadFromImage, GetImageId);
		INITIALIZE_SETTER(Sprite, ImagePath, LoadFromImage);
		INITIALIZE_SETTER(Sprite, Image, LoadFromImage);
		INITIALIZE_SETTER(Sprite, Pixels, LoadFromBitmap);
		INITIALIZE_PROPERTY(Sprite, LeftTopColor, SetLeftTopColor, GetLeftTopCorner);
		INITIALIZE_PROPERTY(Sprite, RightTopColor, SetRightTopColor, GetRightTopCorner);
		INITIALIZE_PROPERTY(Sprite, LeftBottomColor, SetLeftBottomColor, GetLeftBottomCorner);
		INITIALIZE_PROPERTY(Sprite, RightBottomColor, SetRightBottomColor, GetRightBottomCorner);
	}
}