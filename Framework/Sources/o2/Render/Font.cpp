#include "o2/stdafx.h"
#include "Font.h"

#include "o2/Render/Render.h"

namespace o2
{
	Font::Font():
		mReady(false)
	{
		o2Render.mFonts.Add(Ref(this));
	}

	Font::Font(const Font& font):
		mCharacters(font.mCharacters), mTexture(font.mTexture), 
		mTextureSrcRect(font.mTextureSrcRect), mReady(font.mReady)
	{
		o2Render.mFonts.Add(Ref(this));
	}

	Font::~Font()
	{
		o2Render.mFonts.Remove(Ref(this));
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
		auto fndHeight = mCharacters.find(height);
		if (fndHeight != mCharacters.End())
		{
			auto fndChar = fndHeight->second.find(id);
			if (fndChar != fndHeight->second.End())
				return fndChar->second;
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

	void Font::AddCharacter(const Character& character)
	{
		mCharacters[character.mHeight][character.mId] = character;
	}

	bool Font::Character::operator==(const Character& other) const
	{
		return mId == other.mId && mHeight == other.mHeight;
	}
}
