#include "o2/stdafx.h"
#include "BitmapFontAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/BitmapFont.h"
#include "o2/Render/Font.h"
#include "o2/Render/Render.h"

namespace o2
{
	BitmapFontAsset::BitmapFontAsset():
		FontAsset(mnew Meta())
	{}

	BitmapFontAsset::BitmapFontAsset(const BitmapFontAsset& asset):
		FontAsset(asset), meta(this)
	{}

	BitmapFontAsset& BitmapFontAsset::operator=(const BitmapFontAsset& asset)
	{
		FontAsset::operator=(asset);
		return *this;
	}

	const char* BitmapFontAsset::GetFileExtensions()
	{
		return "fnt";
	}

	BitmapFontAsset::Meta* BitmapFontAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
	}

	void BitmapFontAsset::LoadData(const String& path)
	{
		mFont = o2Render.mFonts.FindMatchOrDefault([&](auto fnt) { return fnt->GetFileName() == path; });

		if (!mFont)
			mFont = mnew BitmapFont(path);
	}
}
DECLARE_CLASS_MANUAL(o2::DefaultAssetMeta<o2::BitmapFontAsset>);
DECLARE_CLASS_MANUAL(o2::Ref<o2::BitmapFontAsset>);

DECLARE_CLASS(o2::BitmapFontAsset);

DECLARE_CLASS(o2::BitmapFontAsset::Meta);
