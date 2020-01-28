#include "o2/stdafx.h"
#include "FontAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/Font.h"
#include "o2/Render/Render.h"

namespace o2
{
	FontAsset::FontAsset()
	{}

	FontAsset::FontAsset(const FontAsset& other) :
		TAsset(other), mFont(other.mFont), font(this)
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

	const char* FontAsset::GetFileExtensions() const
	{
		return "ufnt";
	}
}

DECLARE_CLASS(o2::FontAsset);
