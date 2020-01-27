#include "o2/stdafx.h"
#include "FontAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/Font.h"
#include "o2/Render/Render.h"

namespace o2
{
	const Type* FontAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(FontAsset);
	}

	FontAsset::FontAsset() :
		Asset()
	{
		mMeta = mnew MetaInfo();
	}

	FontAsset::FontAsset(const String& path) :
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		ID() = o2Assets.GetAssetId(path);

		Load();
	}

	FontAsset::FontAsset(const UID& id)
	{
		mMeta = mnew MetaInfo();
		ID() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	FontAsset::FontAsset(const FontAsset& asset) :
		Asset(asset), mFont(asset.mFont), meta(this), font(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		ID() = asset.GetUID();
	}

	FontAsset::~FontAsset()
	{}

	FontRef FontAsset::GetFont() const
	{
		return mFont;
	}

	FontAsset& FontAsset::operator=(const FontAsset& asset)
	{
		Asset::operator=(asset);
		mFont = asset.mFont;
		*mMeta = *(MetaInfo*)(asset.mMeta);
		return *this;
	}

	bool FontAsset::operator==(const FontAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool FontAsset::operator!=(const FontAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	const char* FontAsset::GetFileExtensions() const
	{
		return "ufnt";
	}

	FontAsset::MetaInfo* FontAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	void FontAsset::LoadData(const String& path)
	{}

	FontAssetRef FontAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<FontAsset>();
	}

}

DECLARE_CLASS(o2::FontAsset);

DECLARE_CLASS(o2::FontAssetRef);

DECLARE_CLASS(o2::FontAsset::MetaInfo);
