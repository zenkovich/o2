#include "o2/stdafx.h"
#include "TextureRef.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/Render.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
    Ref<Texture>::Ref(const Vec2I& size,
                      TextureFormat format /*= TextureFormat::R8G8B8A8*/,
                      Texture::Usage usage /*= Texture::Usage::Default*/) :
        Ref(mmake<Texture>(size, format, usage))
    {}

    Ref<Texture>::Ref(const String& fileName)
    {
        *this = o2Render.mTextures.FindOrDefault([&](const Ref<Texture>& tex) { return tex->GetFileName() == fileName; });

        if (!mPtr)
            *this = mmake<Texture>(fileName);
    }

    Ref<Texture>::Ref(const Bitmap& bitmap) :
        Ref(mmake<Texture>(bitmap))
    {}

    Ref<Texture>::Ref(UID atlasAssetId, int page)
    {
        *this = (o2Render.mTextures.FindOrDefault(
            [&](const Ref<Texture>& tex)
            {
                return tex->GetAtlasAssetId() == atlasAssetId && tex->GetAtlasPage() == page;
            }));

        if (!mPtr)
            *this = mmake<Texture>(atlasAssetId, page);
    }

    Ref<Texture>::Ref(const String& atlasAssetName, int page)
    {
        UID atlasAssetId = o2Assets.GetAssetId(atlasAssetName);
        if (atlasAssetId == UID::empty)
        {
            o2Render.mLog->Error("Can't load texture for atlas " + atlasAssetName + " and page " + (String)page + ": atlas isn't exist");
            *this = nullptr;
            return;
        }

        *this = (o2Render.mTextures.FindOrDefault(
            [&](const Ref<Texture>& tex)
            {
                return tex->GetAtlasAssetId() == atlasAssetId && tex->GetAtlasPage() == page;
            }));

        if (!mPtr)
            *this = mmake<Texture>(atlasAssetId, page);
    }

    Ref<Texture> Ref<Texture>::Null()
    {
        return Ref<Texture>();
    }
}
// --- META ---

DECLARE_CLASS(o2::Ref<o2::Texture>, o2__Ref_o2__Texture_);
// --- END META ---
