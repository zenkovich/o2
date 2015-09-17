#include "Font.h"

#include "Assets/ImageAsset.h"
#include "Dependencies/PugiXml/pugixml.hpp"
#include "Render/Render.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	Font::Font():
		mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mAllSymbolReturn(false)
	{}

	Font::Font(const Font& font):
		mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0)
	{}

	Font::~Font()
	{
		if (mCharacters)
			delete[] mCharacters;

		if (mCharacterIds)
			delete[] mCharacterIds;
	}

	void Font::Create(const String& fontFile)
	{}

	void Font::CreateFromBMFont(const String& fontFile)
	{
		pugi::xml_document doc;
		auto res = doc.load_file(fontFile.Data());

		if (res.status != pugi::status_ok)
		{
			o2Render.mLog->Error("Failed to load BMFont file: %s", fontFile);
			return;
		}

		pugi::xml_node root = doc.child(L"font");

		if (pugi::xml_node commonNode = root.child(L"common"))
		{
			mName = commonNode.attribute(L"name").value();

			String textureName = commonNode.attribute(L"texture").value();
			ImageAsset image(textureName);
			mTexture = image.GetAtlasTextureRef();
			mTextureSrcRect = image.GetAtlasRect();

			mLineHeight = commonNode.attribute(L"lineHeight").as_float();
			mBase = commonNode.attribute(L"base").as_float();
		}
		else
		{
			o2Render.mLog->Error("Failed to get common info in font: %s. Bad file format", fontFile);
			return;
		}

		if (pugi::xml_node charsNode = root.child(L"chars"))
		{
			mCharactersCount = charsNode.attribute(L"count").as_int();

			if (mCharacters)
				delete[] mCharacters;

			if (mCharacterIds)
				delete[] mCharacterIds;

			mCharacters = new Character[mCharactersCount];
			mCharacterIds = new UInt16[nMaxSymbolId];
			memset(mCharacterIds, 0, sizeof(UInt16)*nMaxSymbolId);

			int i = 0;
			Vec2F texOffs = mTextureSrcRect.LeftBottom();
			for (pugi::xml_node charNode = charsNode.child(L"char"); charNode; charNode = charNode.next_sibling(L"char"))
			{
				Character* charp = &(mCharacters[i++]);

				charp->mTexSrc.left = charNode.attribute(L"x").as_float();
				charp->mTexSrc.top = charNode.attribute(L"y").as_float();
				charp->mTexSrc.right = charNode.attribute(L"width").as_float() + charp->mTexSrc.left;
				charp->mTexSrc.bottom = charNode.attribute(L"height").as_float() + charp->mTexSrc.top;
				charp->mTexSrc += texOffs;

				charp->mOffset.x = charNode.attribute(L"xoffset").as_float();
				charp->mOffset.y = -charNode.attribute(L"yoffset").as_float();

				charp->mAdvance = charNode.attribute(L"xadvance").as_float();

				charp->mCharId = charNode.attribute(L"id").as_uint();
			}

			for (int i = 0; i < mCharactersCount; i++)
				mCharacterIds[mCharacters[i].mCharId] = i;
		}
		else
		{
			o2Render.mLog->Error("Failed to get characters node in BMFont file: %s. Bad file format", fontFile);
			return;
		}

		Vec2F invTexSize(1.0f/mTexture->GetSize().x, 1.0f/mTexture->GetSize().y);
		for (int i = 0; i < mCharactersCount; i++)
		{
			mCharacters[i].mSize = mCharacters[i].mTexSrc.Size();
			mCharacters[i].mTexSrc.left *= invTexSize.x;
			mCharacters[i].mTexSrc.right *= invTexSize.x;
			mCharacters[i].mTexSrc.top *= invTexSize.y;
			mCharacters[i].mTexSrc.bottom *= invTexSize.y;
		}
	}

	float Font::GetLineHeight() const
	{
		return mLineHeight;
	}

	float Font::GetBase() const
	{
		return mBase;
	}

	float Font::GetSymbolAdvance(UInt16 id)
	{
		return mCharacters[mCharacterIds[id]].mAdvance;
	}

	bool Font::Character::operator==(const Character& other) const
	{
		return mCharId == other.mCharId;
	}

}