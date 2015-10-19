#include "Font.h"

#include "Render/Render.h"

namespace o2
{
	Font::Font():
		mReady(false)
	{
		o2Render.mFonts.Add(this);
	}

	Font::Font(const Font& font):
		mCharacters(font.mCharacters), mBaseHeight(font.mBaseHeight), mTexture(font.mTexture), 
		mTextureSrcRect(font.mTextureSrcRect), mReady(font.mReady)
	{
		o2Render.mFonts.Add(this);
	}

	Font::~Font()
	{
		o2Render.mFonts.Remove(this);
	}

	float Font::GetHeight() const
	{
		return mBaseHeight;
	}

	float Font::GetLineHeight() const
	{
		return mLineHeight;
	}

	const Font::Character& Font::GetCharacter(UInt16 id)
	{
		for (const Character& ch : mCharacters)
			if (ch.mId == id)
				return ch;

		return mCharacters.Get(0);
	}

	void Font::CheckCharacters(const WString& needChararacters)
	{}

	String Font::GetFileName() const
	{
		return String();
	}

	bool Font::Character::operator==(const Character& other) const
	{
		return mId == other.mId;
	}
}