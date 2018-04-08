#include "stdafx.h"
#include "BitmapFont.h"

#include "Assets/ImageAsset.h"
#include "Dependencies/PugiXml/pugixml.hpp"
#include "Render/Render.h"
#include "Utils/Serialization/DataNode.h"
#include "Utils/Debug/Log/LogStream.h"

namespace o2
{
	BitmapFont::BitmapFont():
		Font()
	{}

	BitmapFont::BitmapFont(const String& fileName):
		Font()
	{
		Load(fileName);
	}

	bool BitmapFont::Load(const String& fileName)
	{
		mFileName = fileName;

		pugi::xml_document doc;
		auto res = doc.load_file(fileName.Data());

		if (res.status != pugi::status_ok)
		{
			o2Render.mLog->Error("Failed to load Bitmap Font file: %s", fileName);
			return false;
		}

		pugi::xml_node root = doc.child(L"font");

		if (pugi::xml_node commonNode = root.child(L"common"))
		{
			String textureName = commonNode.attribute(L"texture").value();
			ImageAssetRef image(textureName);
			mTexture = image->GetAtlasTextureRef();
			mTextureSrcRect = image->GetAtlasRect();

			mBaseHeight = commonNode.attribute(L"base").as_float();
			mLineHeight = commonNode.attribute(L"lineHeight").as_float();
		}
		else
		{
			o2Render.mLog->Error("Failed to get common info in font: %s. Bad file format", fileName);
			return false;
		}

		if (pugi::xml_node charsNode = root.child(L"chars"))
		{
			int i = 0;
			Vec2F texOffs = mTextureSrcRect.LeftBottom();
			for (pugi::xml_node charNode = charsNode.child(L"char"); charNode; charNode = charNode.next_sibling(L"char"))
			{
				Character newChar;

				newChar.mTexSrc.left = charNode.attribute(L"x").as_float();
				newChar.mTexSrc.top = charNode.attribute(L"y").as_float();
				newChar.mTexSrc.right = charNode.attribute(L"width").as_float() + newChar.mTexSrc.left;
				newChar.mTexSrc.bottom = charNode.attribute(L"height").as_float() + newChar.mTexSrc.top;
				newChar.mTexSrc += texOffs;

				newChar.mOrigin.x = -charNode.attribute(L"xoffset").as_float();
				newChar.mOrigin.y = mLineHeight - mBaseHeight;

				newChar.mAdvance = charNode.attribute(L"xadvance").as_float();

				newChar.mId = charNode.attribute(L"id").as_uint();

				mCharacters.Add(newChar);
			}
		}
		else
		{
			o2Render.mLog->Error("Failed to get characters node in BMFont file: %s. Bad file format", fileName);
			return false;
		}

		Vec2F invTexSize(1.0f/mTexture->GetSize().x, 1.0f/mTexture->GetSize().y);
		for (auto& ch : mCharacters)
		{
			ch.mSize = ch.mTexSrc.Size().InvertedY();
			ch.mTexSrc.left *= invTexSize.x;
			ch.mTexSrc.right *= invTexSize.x;
			ch.mTexSrc.top *= invTexSize.y;
			ch.mTexSrc.bottom *= invTexSize.y;
		}

		mReady = true;
		return true;
	}

	String BitmapFont::GetFileName() const
	{
		return mFileName;
	}

	float BitmapFont::GetHeightPx(int height) const
	{
		return mBaseHeight;
	}

	float BitmapFont::GetLineHeightPx(int height) const
	{
		return mLineHeight;
	}

	const Font::Character& BitmapFont::GetCharacter(UInt16 id, int height)
	{
		for (const Character& ch : mCharacters)
			if (ch.mId == id)
				return ch;

		return mCharacters.Get(0);
	}

}