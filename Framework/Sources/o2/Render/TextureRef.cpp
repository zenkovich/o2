#include "o2/stdafx.h"
#include "TextureRef.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/Render.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
    TextureRef::TextureRef():
        mTexture(nullptr)
    {
    }

    TextureRef::TextureRef(const Vec2I& size, 
                           TextureFormat format /*= TextureFormat::R8G8B8A8*/,
                           Texture::Usage usage /*= Texture::Usage::Default*/)
    {
        mTexture = mnew Texture(size, format, usage);
        mTexture->mRefs++;
    }

    TextureRef::TextureRef(const String& fileName)
    {
        mTexture = o2Render.mTextures.FindOrDefault([&](Texture* tex) { return tex->GetFileName() == fileName; });

        if (!mTexture)
            mTexture = mnew Texture(fileName);

        mTexture->mRefs++;
    }

    TextureRef::TextureRef(Bitmap* bitmap)
    {
        mTexture = mnew Texture(bitmap);
        mTexture->mRefs++;
    }

    TextureRef::TextureRef(const TextureRef& other):
        mTexture(other.mTexture)
    {
        if (mTexture)
            mTexture->mRefs++;
    }

    TextureRef::TextureRef(Texture* texture):
        mTexture(texture)
    {
        if (mTexture)
            mTexture->mRefs++;
    }

    TextureRef::TextureRef(UID atlasAssetId, int page)
    {
        mTexture = o2Render.mTextures.FindOrDefault([&](Texture* tex) {
            return tex->GetAtlasAssetId() == atlasAssetId && tex->GetAtlasPage() == page;
        });

        if (!mTexture)
            mTexture = mnew Texture(atlasAssetId, page);

        mTexture->mRefs++;
    }

    TextureRef::TextureRef(const String& atlasAssetName, int page)
    {
        UID atlasAssetId = o2Assets.GetAssetId(atlasAssetName);
        if (atlasAssetId == 0)
        {
            o2Render.mLog->Error("Can't load texture for atlas " + atlasAssetName + " and page " + (String)page + ": atlas isn't exist");
            mTexture = nullptr;
            return;
        }

        mTexture = o2Render.mTextures.FindOrDefault([&](Texture* tex) {
            return tex->GetAtlasAssetId() == atlasAssetId && tex->GetAtlasPage() == page;
        });

        if (!mTexture)
            mTexture = mnew Texture(atlasAssetId, page);

        mTexture->mRefs++;
    }

    TextureRef::~TextureRef()
    {
        if (mTexture)
            mTexture->mRefs--;
    }

    TextureRef& TextureRef::operator=(const TextureRef& other)
    {
        if (mTexture)
            mTexture->mRefs--;

        mTexture = other.mTexture;

        if (mTexture)
            mTexture->mRefs++;

        return *this;
    }

    Texture* TextureRef::operator->()
    {
        return mTexture;
    }

    const Texture* TextureRef::operator->() const
    {
        return mTexture;
    }

    Texture* TextureRef::Get() const
    {
        return mTexture;
    }

    bool TextureRef::IsValid() const
    {
        return mTexture != nullptr;
    }

    TextureRef::operator bool() const
    {
        return mTexture != nullptr;
    }

    TextureRef TextureRef::Null()
    {
        return TextureRef();
    }

    bool TextureRef::operator!=(const TextureRef& other) const
    {
        return mTexture != other.mTexture;
    }

    bool TextureRef::operator==(const TextureRef& other) const
    {
        return mTexture == other.mTexture;
    }

    TextureRef NoTexture()
    {
        return TextureRef();
    }

}
// --- META ---

DECLARE_CLASS(o2::TextureRef, o2__TextureRef);
// --- END META ---
