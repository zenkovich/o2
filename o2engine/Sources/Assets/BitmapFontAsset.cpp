#include "BitmapFontAsset.h"

#include "Assets/Assets.h"
#include "Render/BitmapFont.h"
#include "Render/Font.h"
#include "Render/Render.h"

namespace o2
{
	Type::Id BitmapFontAsset::MetaInfo::GetAssetType() const
	{
		return BitmapFontAsset::type->ID();
	}

	BitmapFontAsset::BitmapFontAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	BitmapFontAsset::BitmapFontAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		InitializeProperties();

		Load();
	}

	BitmapFontAsset::BitmapFontAsset(AssetId id)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	BitmapFontAsset::BitmapFontAsset(const BitmapFontAsset& asset):
		Asset(asset), mFont(asset.mFont)
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	BitmapFontAsset::~BitmapFontAsset()
	{}

	FontRef BitmapFontAsset::GetFont() const
	{
		return mFont;
	}

	const char* BitmapFontAsset::GetFileExtensions() const
	{
		return "fnt";
	}

	BitmapFontAsset::MetaInfo* BitmapFontAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	void BitmapFontAsset::LoadData(const String& path)
	{
		mFont = o2Render.mFonts.FindMatch([&](auto fnt) { return fnt->GetFileName() == path; });

		if (!mFont)
			mFont = mnew BitmapFont(path);
	}

	void BitmapFontAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(BitmapFontAsset, meta, GetMeta);
		INITIALIZE_GETTER(BitmapFontAsset, font, GetFont);
	}

	BitmapFontAsset& BitmapFontAsset::operator=(const BitmapFontAsset& asset)
	{
		mFont = asset.mFont;
		return *this;
	}
}