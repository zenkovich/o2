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

	BitmapFontAsset::BitmapFontAsset(UID id)
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
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();
		InitializeProperties();
	}

	BitmapFontAsset::~BitmapFontAsset()
	{}

	FontRef BitmapFontAsset::GetFont() const
	{
		return mFont;
	}

	BitmapFontAsset& BitmapFontAsset::operator=(const BitmapFontAsset& asset)
	{
		Asset::operator=(asset);
		mFont = asset.mFont;
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

	void BitmapFontAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(BitmapFontAsset, meta, GetMeta);
		INITIALIZE_GETTER(BitmapFontAsset, font, GetFont);
	}

	BitmapFontAssetRef BitmapFontAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<BitmapFontAsset>();
	}

}

CLASS_META(o2::BitmapFontAsset)
{
	BASE_CLASS(o2::Asset);

	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(font);
	PROTECTED_FIELD(mFont);

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::BitmapFontAssetRef)
{
	BASE_CLASS(o2::AssetRef);


	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_META(o2::BitmapFontAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);


	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
