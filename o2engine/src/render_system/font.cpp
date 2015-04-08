#include "font.h"

#include "render_system.h"
#include "texture.h"
#include "util/log.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

Font::Font():
mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mTexture(NULL), mAllSymbolReturn(false)
{
}

Font::Font(const Font& font):
mCharacters(NULL), mCharacterIds(NULL), mCharactersCount(0), mTexture(NULL)
{
}

Font::~Font()
{
	SafeReleaseArr(mCharacters);
	SafeReleaseArr(mCharacterIds);
}

void Font::Create(const String& fontFile)
{
	pugi::xml_document doc;
	XmlTools::LoadFromFile(fontFile, doc);
}

void Font::CreateFromBMFont(const String& fontFile)
{
	pugi::xml_document doc;
	if (!XmlTools::LoadFromFile(fontFile, doc))
	{
		AppRender()->mLog->Error("Failed to load BMFont file: %s", fontFile.c_str());
		return;
	}

	pugi::xml_node root = doc.child("font");

	if (pugi::xml_node commonNode = root.child("common"))
	{
		mName = commonNode.attribute("name").value();

		String textureName = commonNode.attribute("texture").value();
		mTexture = AppRender()->GetTextureFromFile(textureName);

		mLineHeight = commonNode.attribute("lineHeight").as_float();
		mBase = commonNode.attribute("base").as_float();
	}
	else
	{
		AppRender()->mLog->Error("Failed to get common info in font: %s. Bad file format", fontFile.c_str());
		return;
	}

	if (pugi::xml_node charsNode = root.child("chars"))
	{
		mCharactersCount = charsNode.attribute("count").as_int();

		SafeReleaseArr(mCharacters);
		SafeReleaseArr(mCharacterIds);
		mCharacters = mnew Character[mCharactersCount];
		mCharacterIds = mnew uint16[nMaxSymbolId];
		memset(mCharacterIds, 0, sizeof(uint16)*nMaxSymbolId);

		int i = 0;
		for (pugi::xml_node charNode = charsNode.child("char"); charNode; charNode = charNode.next_sibling("char"))
		{
			Character* charp = &(mCharacters[i++]);

			charp->mTexSrc.left = charNode.attribute("x").as_float();
			charp->mTexSrc.top = charNode.attribute("y").as_float();
			charp->mTexSrc.right = charNode.attribute("width").as_float() + charp->mTexSrc.left;
			charp->mTexSrc.bottom = charNode.attribute("height").as_float() + charp->mTexSrc.top;

			charp->mOffset.x = charNode.attribute("xoffset").as_float();
			charp->mOffset.y = charNode.attribute("yoffset").as_float();

			charp->mAdvance = charNode.attribute("xadvance").as_float();

			charp->mCharId = charNode.attribute("id").as_uint();
		}

		for (int i = 0; i < mCharactersCount; i++)
			mCharacterIds[mCharacters[i].mCharId] = i;
	}
	else
	{
		AppRender()->mLog->Error("Failed to get characters node in BMFont file: %s. Bad file format", fontFile.c_str());
		return;
	}

	Vec2F invTexSize(1.0f/mTexture.GetSize().x, 1.0f/mTexture.GetSize().y);
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

Vec2F Font::GetTextSize(const WString& text, const Vec2F& areaSize /*= vec2f()*/, HorAlign horAlign /*= HA_LEFT*/,
						VerAlign verAlign /*= VA_TOP*/, bool wordWrap /*= true*/, float charsDistCoef /*= 1.0f*/,
						float linesDistCoef /*= 1.0f*/)
{
	TextSymbolsSet textSet;
	textSet.Initialize(this, text, Vec2F(), areaSize, horAlign, verAlign, wordWrap, charsDistCoef, linesDistCoef);
	return textSet.mRealSize;
}

float Font::GetSymbolAdvance(uint16 id)
{
	return mCharacters[mCharacterIds[id]].mAdvance;
}


void Font::TextSymbolsSet::Initialize(Font* font, const WString& text, const Vec2F& position, const Vec2F& areaSize,
									  HorAlign horAlign, VerAlign verAlign, bool wordWrap, float charsDistCoef,
									  float linesDistCoef)
{
	mFont = font;
	mText = text;
	mPosition = position;
	mAreaSize = areaSize;
	mRealSize = Vec2F();
	mHorAlign = horAlign;
	mVerAlign = verAlign;
	mWordWrap = wordWrap;
	mCharactersDistCoef = charsDistCoef;
	mLinesDistCoef = linesDistCoef;

	mLineDefs.clear();
	int textLen = mText.length();

	if (textLen == 0)
		return;

	mLineDefs.push_back(LineDef());
	LineDef* curLine = &mLineDefs.back();
	curLine->mSize.y = mFont->GetLineHeight();

	Vec2F fullSize(0, mFont->GetBase());
	bool checkAreaBounds = mWordWrap && mAreaSize.x > FLT_EPSILON;
	int wrapCharIdx = -1;
	for (int i = 0; i < textLen; i++)
	{
		Font::Character* ch = &mFont->mCharacters[mFont->mCharacterIds[mText[i]]];
		Vec2F chSize = ch->mSize;
		Vec2F chPos = Vec2F(curLine->mSize.x, 0) + ch->mOffset;

		curLine->mSymbols.push_back(SymbolDef(chPos, chSize, ch->mTexSrc, ch->mCharId, ch->mOffset, ch->mAdvance));
		curLine->mSize.x += ch->mAdvance*mCharactersDistCoef;
		curLine->mString += mText[i];

		bool outOfBounds = checkAreaBounds ? curLine->mSize.x > mAreaSize.x:false;

		if (mText[i] == '\n' || outOfBounds)
		{
			if (outOfBounds)
			{
				if (wrapCharIdx < 0)
					wrapCharIdx = i;
				else
					curLine->mSpacesCount--;

				int cutLen = wrapCharIdx - curLine->mLineBegSymbol;

				curLine->mSymbols.erase(curLine->mSymbols.begin() + cutLen, curLine->mSymbols.end());
				curLine->mString.erase(curLine->mString.begin() + cutLen, curLine->mString.end());


				if (curLine->mSymbols.size() > 0)
					curLine->mSize.x = curLine->mSymbols.back().mFrame.right;
				else
					curLine->mSize.x = 0;

				i = wrapCharIdx;
				wrapCharIdx = -1;
			}
			else
			{
				curLine->mSymbols.pop_back();
				curLine->mString.pop_back();
				curLine->mEndedNewLine = true;
			}

			mLineDefs.Add(LineDef());
			curLine = &mLineDefs.back();
			curLine->mSize.y = mFont->GetLineHeight();
			curLine->mLineBegSymbol = i + 1;
			fullSize.x = Max(fullSize.x, curLine->mSize.x);
			fullSize.y += mFont->mLineHeight*mLinesDistCoef;
		}
		else if (mText[i] == ' ' || mFont->mAllSymbolReturn)
		{
			curLine->mSpacesCount++;
			wrapCharIdx = i;
		}
	}

	fullSize.x = Max(fullSize.x, curLine->mSize.x);

	float yOffset = 0;
	float lineHeight = mFont->mLineHeight*mLinesDistCoef;

	if (mVerAlign == VA_CENTER)
		yOffset = mAreaSize.y*0.5f - fullSize.y*0.5f + mFont->GetBase() - mFont->GetLineHeight();
	else if (mVerAlign == VA_BOTH)
		lineHeight = (mAreaSize.y - lineHeight + mFont->GetBase())/(float)(mLineDefs.size() - 1);
	else if (mVerAlign == VA_BOTTOM)
		yOffset = mAreaSize.y - fullSize.y;

	yOffset += mPosition.y;

	for (LineDefsArr::iterator it = mLineDefs.begin(); it != mLineDefs.end(); ++it)
	{
		LineDef* line = &(*it);
		float xOffset = 0;
		float additiveSpaceOffs = 0;

		if (mHorAlign == HA_CENTER)
			xOffset = (mAreaSize.x - line->mSize.x)*0.5f;
		else if (mHorAlign == HA_RIGHT)
			xOffset = mAreaSize.x - line->mSize.x;
		else if (mHorAlign == Both)
			additiveSpaceOffs = (mAreaSize.x - line->mSize.x)/(float)line->mSpacesCount;

		xOffset += mPosition.x;

		Vec2F locOrigin((float)(int)xOffset, (float)(int)yOffset);
		line->mPosition = locOrigin;
		yOffset += lineHeight;
		for (SymbolDefsArr::iterator jt = line->mSymbols.begin(); jt != line->mSymbols.end(); ++jt)
		{
			if (jt->mCharId == ' ')
				locOrigin.x += additiveSpaceOffs;

			jt->mFrame = jt->mFrame + locOrigin;
		}

		mRealSize.x = Max(mRealSize.x, line->mSize.x);
		mRealSize.y = Max(mRealSize.y, line->mSize.y + line->mPosition.y - mPosition.y);
	}
}

CLOSE_O2_NAMESPACE