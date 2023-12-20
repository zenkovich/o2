#include "o2/stdafx.h"
#include "FontRef.h"

#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
    Ref<Font>::Ref(const String& fileName)
    {
        String ext = o2FileSystem.GetFileExtension(fileName);
        if (ext == "xml")
        {
            BitmapFontAssetRef fontAsset(fileName);
            *this = fontAsset->GetFont();
        }
        else
        {
            VectorFontAssetRef fontAsset(fileName);
            *this = fontAsset->GetFont();
        }
    }
}
// --- META ---

DECLARE_CLASS(o2::Ref<o2::Font>, o2__Ref_o2__Font_);
// --- END META ---
