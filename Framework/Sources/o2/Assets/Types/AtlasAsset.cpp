#include "o2/stdafx.h"
#include "AtlasAsset.h"

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Assets/Assets.h"

namespace o2
{
	bool AtlasAsset::Meta::IsEqual(AssetMeta* other) const
	{
		if (!AssetMeta::IsEqual(other))
			return false;

		Meta* otherMeta = (Meta*)other;
		return ios == otherMeta->ios && android == otherMeta->android && macOS == otherMeta->macOS &&
			windows == otherMeta->windows && Math::Equals(border, otherMeta->border);
	}

	UInt AtlasAsset::Page::ID() const
	{
		return mId;
	}

	Vec2I AtlasAsset::Page::Size() const
	{
		return mSize;
	}

	TextureRef AtlasAsset::Page::GetTextureRef() const
	{
		return AtlasAsset::GetPageTextureRef(mOwner->GetUID(), mId);
	}

	String AtlasAsset::Page::GetTextureFileName() const
	{
		return AtlasAsset::GetPageTextureFileName(mOwner->GetUID(), mId);
	}

	const Map<UID, RectI>& AtlasAsset::Page::ImagesRects() const
	{
		return mImagesRects;
	}

	bool AtlasAsset::Page::operator==(const Page& other) const
	{
		return mId == other.mId;
	}

	AtlasAsset::AtlasAsset():
		Asset(mnew Meta())
	{}

	AtlasAsset::AtlasAsset(const AtlasAsset& other):
		Asset(other), mImages(other.mImages), mPages(other.mPages), meta(this), images(this), pages(this)
	{
		for (auto& page : mPages)
			page.mOwner = this;
	}

	void AtlasAsset::OnDeserialized(const DataNode& node)
	{
		for (auto& page : mPages)
			page.mOwner = this;
	}

	AtlasAsset& AtlasAsset::operator=(const AtlasAsset& other)
	{
		Asset::operator=(other);

		mImages = other.mImages;
		mPages = other.mPages;

		return *this;
	}

	const Vector<ImageAssetRef>& AtlasAsset::GetImages() const
	{
		return mImages;
	}

	const Vector<AtlasAsset::Page>& AtlasAsset::GetPages() const
	{
		return mPages;
	}

	bool AtlasAsset::ContainsImage(const ImageAssetRef& image)
	{
		return mImages.Contains(image);
	}

	AtlasAsset::Meta* AtlasAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
	}

	const char* AtlasAsset::GetFileExtensions() const
	{
		return "atlas";
	}

	String AtlasAsset::GetPageTextureFileName(const UID& atlasId, UInt pageIdx)
	{
		return GetPageTextureFileName(o2Assets.GetAssetPath(atlasId), pageIdx);
	}

	String AtlasAsset::GetPageTextureFileName(const String& atlasPath, UInt pageIdx)
	{
		return o2Assets.GetBuiltAssetsPath() + atlasPath + (String)pageIdx + ".png";
	}

	TextureRef AtlasAsset::GetPageTextureRef(const UID& atlasId, UInt pageIdx)
	{
		return TextureRef(GetPageTextureFileName(atlasId, pageIdx));
	}

	TextureRef AtlasAsset::GetPageTextureRef(const String& atlasPath, UInt pageIdx)
	{
		return TextureRef(GetPageTextureFileName(atlasPath, pageIdx));
	}

	bool AtlasAsset::PlatformMeta::operator==(const PlatformMeta& other) const
	{
		return maxSize == other.maxSize && format == other.format;
	}
}

DECLARE_CLASS_MANUAL(o2::DefaultAssetMeta<o2::AtlasAsset>);

DECLARE_CLASS(o2::AtlasAsset);

DECLARE_CLASS(o2::AtlasAsset::PlatformMeta);

DECLARE_CLASS(o2::AtlasAsset::Meta);

DECLARE_CLASS(o2::AtlasAsset::Page);
