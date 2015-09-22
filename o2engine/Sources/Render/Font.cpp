#include "Font.h"

namespace o2
{
	Font::Font():
		mReady(false)
	{}

	Font::Font(const Font& font):
		mCharacters(font.mCharacters), mBaseHeight(font.mBaseHeight), mTexture(font.mTexture), 
		mTextureSrcRect(font.mTextureSrcRect), mReady(font.mReady)
	{}

	Font::~Font()
	{}

	float Font::GetHeight() const
	{
		return mBaseHeight;
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

	bool Font::Character::operator==(const Character& other) const
	{
		return mId == other.mId;
	}

}