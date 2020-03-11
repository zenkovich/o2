#include "o2/stdafx.h"
#include "Font.h"

#include "o2/Render/Render.h"

namespace o2
{
	Font::Font():
		mReady(false)
	{
		o2Render.mFonts.Add(this);
	}

	Font::Font(const Font& font):
		mCharacters(font.mCharacters), mTexture(font.mTexture), 
		mTextureSrcRect(font.mTextureSrcRect), mReady(font.mReady)
	{
		o2Render.mFonts.Add(this);
	}

	Font::~Font()
	{
		o2Render.mFonts.Remove(this);
	}

	float Font::GetHeightPx(int height) const
	{
		return 1.0f;
	}

	float Font::GetLineHeightPx(int height) const
	{
		return 1.0f;
	}

	const Font::Character& Font::GetCharacter(UInt16 id, int height)
	{
		for (const Character& ch : mCharacters)
		{
			if (ch.mId == id && height == ch.mHeight)
				return ch;
		}

		static Character empty;
		return empty;
	}

	void Font::CheckCharacters(const WString& needChararacters, int height)
	{}

	String Font::GetFileName() const
	{
		return String();
	}

	bool Font::Character::operator==(const Character& other) const
	{
		return mId == other.mId && mHeight == other.mHeight;
	}
}
