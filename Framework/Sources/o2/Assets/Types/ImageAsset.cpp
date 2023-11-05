#include "o2/stdafx.h"
#include "ImageAsset.h"

#include "o2/Assets/Types/AtlasAsset.h"
#include "o2/Assets/Assets.h"
#include "o2/Utils/Bitmap/Bitmap.h"
#include "o2/Utils/Debug/Log/LogStream.h"

//#undef LoadBitmap

namespace o2
{
    ImageAsset::ImageAsset():
        Asset(mnew Meta())
    {}

    ImageAsset::ImageAsset(const ImageAsset& other):
        Asset(other), mAtlasPage(other.mAtlasPage), mSourceRect(other.mSourceRect), bitmap(this), atlas(this), sliceBorder(this),
        atlasPage(this), atlasRect(this), size(this), width(this), height(this), meta(this)
    {
        if (other.mBitmap)
            mBitmap = other.mBitmap->Clone();
        else
            mBitmap = nullptr;

        mAtlasPage = other.mAtlasPage;
        mSourceRect = other.mSourceRect;
    }

    ImageAsset::~ImageAsset()
    {
        if (mBitmap)
            delete mBitmap;
    }

    ImageAsset& ImageAsset::operator=(const ImageAsset& asset)
    {
        Asset::operator=(asset);

        if (mBitmap)
            delete mBitmap;

        if (asset.mBitmap)
            SetBitmap(asset.mBitmap);

        mAtlasPage = asset.mAtlasPage;
        mSourceRect = asset.mSourceRect;

        return *this;
    }

    Bitmap* ImageAsset::GetBitmap()
    {
        if (!mBitmap)
            LoadBitmap();

        return mBitmap;
    }

    void ImageAsset::SetBitmap(Bitmap* bitmap)
    {
        if (mBitmap)
            delete mBitmap;

        mBitmap = bitmap;
    }

    UID ImageAsset::GetAtlasUID() const
    {
        return GetMeta()->atlasId;
    }

    void ImageAsset::SetAtlas(const UID& atlas)
    {
        ImageAssetRef thisRef(GetUID());

        if (auto prevAtlas = AtlasAssetRef(GetMeta()->atlasId)) 
            prevAtlas->RemoveImage(thisRef);

        GetMeta()->atlasId = atlas;

        if (auto newAtlas = AtlasAssetRef(GetMeta()->atlasId))
            newAtlas->AddImage(thisRef);
    }

    bool ImageAsset::IsInAtlas() const
    {
        return GetAtlasUID() != UID::empty;
    }

    void ImageAsset::SetSliceBorder(const BorderI& border)
    {
        GetMeta()->sliceBorder = border;
    }

    BorderI ImageAsset::GetSliceBorder() const
    {
        return GetMeta()->sliceBorder;
    }

    void ImageAsset::SetDefaultMode(SpriteMode mode)
    {
        GetMeta()->defaultMode = mode;
    }

    SpriteMode ImageAsset::GetDefaultMode() const
    {
        return GetMeta()->defaultMode;
    }

    UInt ImageAsset::GetAtlasPage() const
    {
        return mAtlasPage;
    }

    RectI ImageAsset::GetAtlasRect() const
    {
        return mSourceRect;
    }

    Vec2F ImageAsset::GetSize() const
    {
        return mSourceRect.Size();
    }

    float ImageAsset::GetWidth() const
    {
        return GetSize().x;
    }

    float ImageAsset::GetHeight() const
    {
        return GetSize().y;
    }

    TextureSource ImageAsset::GetTextureSource() const
    {
        TextureRef texture; 

        if (!IsInAtlas())
            texture = mTexture;
        else
        {
            if (auto atlas = dynamic_cast<const AtlasAsset*>(mAtlas.Get()))
            {
                auto& pages = atlas->GetPages();
                if (mAtlasPage < pages.Count())
                    texture = pages[mAtlasPage].GetTexture();
            }
        }

        return { texture, mSourceRect };
    }

    ImageAsset::Meta* ImageAsset::GetMeta() const
    {
        return (Meta*)mInfo.meta;
    }

    Vector<String> ImageAsset::GetFileExtensions()
    {
        return { "png", "jpg", "bmp" };
    }

    void ImageAsset::LoadData(const String& path)
    {
        if (!IsInAtlas())
        {
            mTexture = TextureRef(path);
            mSourceRect = RectI(Vec2F(), mTexture->GetSize());
            mAtlas = nullptr;
        }
        else
        {
            Asset::LoadData(path);
            mAtlas = AtlasAssetRef(GetAtlasUID());
        }
    }

    void ImageAsset::SaveData(const String& path) const
    {
        if (mBitmap)
            mBitmap->Save(GetFullPath(), Bitmap::ImageType::Png);
    }

    void ImageAsset::LoadBitmap()
    {
        String assetFullPath = GetFullPath();
        mBitmap->Load(assetFullPath);
    }

    bool ImageAsset::PlatformMeta::operator==(const PlatformMeta& other) const
    {
        return format == other.format;
    }

    ImageAsset::PlatformMeta ImageAsset::Meta::GetResultPlatformMeta(Platform platform) const
    {
        PlatformMeta res = common;

        switch (platform)
        {
            case Platform::iOS: if (ios) res = *ios; break;
            case Platform::Android: if (android) res = *android; break;
            case Platform::Mac: if (macOS) res = *macOS; break;
            case Platform::Windows: if (windows) res = *windows; break;
            case Platform::Linux: if (linuxOS) res = *linuxOS; break;
        }

        return res;
    }

    bool ImageAsset::Meta::IsEqual(AssetMeta* other) const
    {
        if (!AssetMeta::IsEqual(other))
            return false;

        Meta* otherMeta = (Meta*)other;
        if (atlasId != otherMeta->atlasId ||
            sliceBorder != otherMeta->sliceBorder ||
            defaultMode != otherMeta->defaultMode)
        {
            return false;
        }

        if (!(common == otherMeta->common))
            return false;

        auto comparePlatformMeta = [](PlatformMeta* a, PlatformMeta* b)
        {
            if (a && b)
            {
                if (!(*a == *b))
                    return false;
            }
            else if (a || b)
                return false;

            return true;
        };

        if (!comparePlatformMeta(ios, otherMeta->ios) ||
            !comparePlatformMeta(android, otherMeta->android) ||
            !comparePlatformMeta(macOS, otherMeta->macOS) ||
            !comparePlatformMeta(windows, otherMeta->windows) ||
            !comparePlatformMeta(linuxOS, otherMeta->linuxOS))
        {
            return false;
        }

        return true;
    }
}

DECLARE_TEMPLATE_CLASS(o2::DefaultAssetMeta<o2::ImageAsset>);
DECLARE_TEMPLATE_CLASS(o2::Ref<o2::ImageAsset>);
// --- META ---

DECLARE_CLASS(o2::ImageAsset);

DECLARE_CLASS(o2::ImageAsset::PlatformMeta);

DECLARE_CLASS(o2::ImageAsset::Meta);
// --- END META ---
