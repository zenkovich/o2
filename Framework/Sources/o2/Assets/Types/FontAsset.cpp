#include "o2/stdafx.h"
#include "FontAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/Font.h"
#include "o2/Render/Render.h"

namespace o2
{
	FontAsset::FontAsset():
		Asset(nullptr)
	{}

	FontAsset::FontAsset(const FontAsset& other) :
		Asset(other), mFont(other.mFont), font(this)
	{}

	FontAsset::FontAsset(AssetMeta* meta):
		Asset(meta)
	{}

	FontRef FontAsset::GetFont() const
	{
		return mFont;
	}

	FontAsset& FontAsset::operator=(const FontAsset& other)
	{
		Asset::operator=(other);
		mFont = other.mFont;
		return *this;
	}
}
DECLARE_TEMPLATE_CLASS(o2::Ref<o2::FontAsset>);

DECLARE_CLASS(o2::FontAsset);
