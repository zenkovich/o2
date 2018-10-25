#include "stdafx.h"
#include "BitmapFontAsset.h"

#include "Assets/Assets.h"
#include "Render/BitmapFont.h"
#include "Render/Font.h"
#include "Render/Render.h"

namespace o2
{
	const Type* BitmapFontAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(BitmapFontAsset);
	}

	BitmapFontAsset::BitmapFontAsset():
		FontAsset()
	{
		mMeta = mnew MetaInfo();
	}

	BitmapFontAsset::BitmapFontAsset(const String& path):
		FontAsset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		Load();
	}

	BitmapFontAsset::BitmapFontAsset(UID id)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	BitmapFontAsset::BitmapFontAsset(const BitmapFontAsset& asset):
		FontAsset(asset), meta(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();
	}

	BitmapFontAsset::~BitmapFontAsset()
	{}

	BitmapFontAsset& BitmapFontAsset::operator=(const BitmapFontAsset& asset)
	{
		FontAsset::operator=(asset);
		*mMeta = *(MetaInfo*)(asset.mMeta);
		return *this;
	}

	bool BitmapFontAsset::operator==(const BitmapFontAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool BitmapFontAsset::operator!=(const BitmapFontAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
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

	BitmapFontAssetRef BitmapFontAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<BitmapFontAsset>();
	}

}

DECLARE_CLASS(o2::BitmapFontAsset);

DECLARE_CLASS(o2::BitmapFontAssetRef);

DECLARE_CLASS(o2::BitmapFontAsset::MetaInfo);
