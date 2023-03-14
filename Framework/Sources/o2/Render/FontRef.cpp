#include "o2/stdafx.h"
#include "FontRef.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/BitmapFontAsset.h"
#include "o2/Assets/Types/VectorFontAsset.h"
#include "o2/Render/BitmapFont.h"
#include "o2/Render/VectorFont.h"
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

DECLARE_CLASS(o2::Ref<o2::Font>);
