#include "Text.h"

#include "Assets/Assets.h"
#include "Assets/BitmapFontAsset.h"
#include "Assets/VectorFontAsset.h"
#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Utils/Debug.h"

namespace o2
{
	Text::Text():
		mFont(nullptr), mSymbolsDistCoef(1), mLinesDistanceCoef(1), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable(), mUpdatingMesh(false), mFontAssetId(0),
		mDotsEndings(false), mHeight(11)
	{
		InitializeProperties();
	}

	Text::Text(FontRef font):
		mFont(font), mSymbolsDistCoef(1), mLinesDistanceCoef(1), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable(), mUpdatingMesh(false),
		mFontAssetId(0), mDotsEndings(false), mHeight(11)
	{
		InitializeProperties();

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);
	}

	Text::Text(const Text& text):
		IRectDrawable(text), mUpdatingMesh(false)
	{
		InitializeProperties();

		mText = text.mText;
		mFont = text.mFont;
		mSymbolsDistCoef = text.mSymbolsDistCoef;
		mLinesDistanceCoef = text.mLinesDistanceCoef;
		mVerAlign = text.mVerAlign;
		mHorAlign = text.mHorAlign;
		mWordWrap = text.mWordWrap;
		mFontAssetId = text.mFontAssetId;
		mDotsEndings = text.mDotsEndings;
		mHeight = text.mHeight;

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);
	}

	Text::Text(BitmapFontAsset* fontAsset):
		Text(fontAsset->GetFont())
	{
		mFontAssetId = fontAsset->GetAssetId();
	}

	Text::Text(VectorFontAsset* fontAsset):
		Text(fontAsset->GetFont())
	{
		mFontAssetId = fontAsset->GetAssetId();
	}

	Text::Text(const String& fontFileName):
		Text(o2Assets.GetAssetId(fontFileName))
	{}

	Text::Text(UID fontAssetId):
		mSymbolsDistCoef(1), mLinesDistanceCoef(1), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable(), mDotsEndings(false), mHeight(11),
		mUpdatingMesh(false)
	{
		InitializeProperties();
		SetFontAsset(fontAssetId);
	}

	Text::~Text()
	{
		for (auto mesh : mMeshes)
			delete mesh;

		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);
	}

	Text& Text::operator=(const Text& other)
	{
		IRectDrawable::operator=(other);

		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mText = other.mText;
		mFontAssetId = other.mFontAssetId;
		mFont = other.mFont;
		mSymbolsDistCoef = other.mSymbolsDistCoef;
		mLinesDistanceCoef = other.mLinesDistanceCoef;
		mVerAlign = other.mVerAlign;
		mHorAlign = other.mHorAlign;
		mWordWrap = other.mWordWrap;
		mDotsEndings = other.mDotsEndings;
		mHeight = other.mHeight;

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);

		UpdateMesh();

		return *this;
	}

	void Text::Draw()
	{
		if (!mEnabled)
			return;

		for (auto mesh : mMeshes)
		{
			mesh->Draw();
			//o2Render.DrawMeshWire(mesh, Color4(0, 255, 0, 150));
		}

		OnDrawn();
	}

	void Text::SetFont(FontRef font)
	{
		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont = font;

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);

		UpdateMesh();
	}

	FontRef Text::GetFont() const
	{
		return mFont;
	}

	void Text::SetFontAsset(BitmapFontAsset* asset)
	{
		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont = asset->GetFont();
		mFontAssetId = asset->GetAssetId();

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);
	}

	void Text::SetFontAsset(VectorFontAsset* asset)
	{
		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont = asset->GetFont();
		mFontAssetId = asset->GetAssetId();

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);
	}

	void Text::SetFontAsset(UID assetId)
	{
		if (!o2Assets.IsAssetExist(assetId))
		{
			o2Debug.LogError("Can't load font asset: %i - isn't exist", assetId);
			return;
		}

		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFontAssetId = assetId;
		AssetInfo fontAssetInfo = o2Assets.GetAssetInfo(mFontAssetId);
		if (fontAssetInfo.GetType() == TypeOf(BitmapFontAsset))
		{
			auto asset = BitmapFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}
		else
		{
			auto asset = VectorFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);

		UpdateMesh();
	}

	void Text::SetFontAsset(const String& fileName)
	{
		if (!o2Assets.IsAssetExist(fileName))
		{
			o2Debug.LogError("Can't load font asset: %s - isn't exist", fileName);
			return;
		}

		if (mFont)
			mFont->onCharactersRebuild -= ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);
		
		AssetInfo fontAssetInfo = o2Assets.GetAssetInfo(fileName);
		mFontAssetId = fontAssetInfo.mId;

		if (fontAssetInfo.GetType() == TypeOf(BitmapFontAsset))
		{
			auto asset = BitmapFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}
		else
		{
			auto asset = VectorFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}

		if (mFont)
			mFont->onCharactersRebuild += ObjFunctionPtr<Text, void>(this, &Text::UpdateMesh);

		mFont->CheckCharacters(mBasicSymbolsPreset, mHeight);
	}

	Asset* Text::GetFontAsset() const
	{
		AssetInfo fontAssetInfo = o2Assets.GetAssetInfo(mFontAssetId);
		if (fontAssetInfo.GetType() == TypeOf(BitmapFontAsset))
			return mnew BitmapFontAsset(mFontAssetId);
		
		return mnew VectorFontAsset(mFontAssetId);
	}

	UID Text::GetFontAssetId() const
	{
		return mFontAssetId;
	}

	void Text::SetHeight(int height)
	{
		mHeight = height;
	}

	int Text::GetHeight() const
	{
		return mHeight;
	}

	void Text::SetText(const WString& text)
	{
		mText = text;

		if (mFont)
		{
			mFont->CheckCharacters(text, height);
			mFont->CheckCharacters(".", height);
		}

		UpdateMesh();
	}

	WString Text::GetText() const
	{
		return mText;
	}

	void Text::SetHorAlign(HorAlign align)
	{
		mHorAlign = align;
		UpdateMesh();
	}

	HorAlign Text::GetHorAlign() const
	{
		return mHorAlign;
	}

	void Text::SetVerAlign(VerAlign align)
	{
		mVerAlign = align;
		UpdateMesh();
	}

	VerAlign Text::GetVerAlign() const
	{
		return mVerAlign;
	}

	void Text::SetWordWrap(bool flag)
	{
		mWordWrap = flag;
		UpdateMesh();
	}

	bool Text::GetWordWrap() const
	{
		return mWordWrap;
	}

	void Text::SetDotsEngings(bool flag)
	{
		mDotsEndings = flag;
		UpdateMesh();
	}

	bool Text::IsDotsEngings() const
	{
		return mDotsEndings;
	}

	void Text::SetSymbolsDistanceCoef(float coef)
	{
		mSymbolsDistCoef = coef;
		UpdateMesh();
	}

	float Text::GetSymbolsDistanceCoef() const
	{
		return mSymbolsDistCoef;
	}

	void Text::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		mLinesDistanceCoef = coef;
		UpdateMesh();
	}

	float Text::GetLinesDistanceCoef() const
	{
		return mLinesDistanceCoef;
	}

	Text::SymbolsSet& Text::GetSymbolsSet()
	{
		return mSymbolsSet;
	}

	Vec2F Text::GetRealSize()
	{
		return mSymbolsSet.mRealSize;
	}

	RectF Text::GetRealRect()
	{
		return RectF(mTransform.offs, mTransform.offs + mSymbolsSet.mRealSize);
	}

	Vec2F Text::GetTextSize(const WString& text, Font* font, int height /*= 11*/, const Vec2F& areaSize /*= Vec2F()*/,
							HorAlign horAlign /*= HorAlign::Left*/, VerAlign verAlign /*= VerAlign::Top*/,
							bool wordWrap /*= true*/, bool dotsEngings /*= false*/, float charsDistCoef /*= 1.0f*/, 
							float linesDistCoef /*= 1.0f*/)
	{
		SymbolsSet textSet;
		textSet.Initialize(font, text, height, Vec2F(), areaSize, horAlign, verAlign, wordWrap, dotsEngings, charsDistCoef, 
						   linesDistCoef);
		return textSet.mRealSize;
	}

	void Text::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Text, font, SetFont, GetFont);
		INITIALIZE_PROPERTY(Text, text, SetText, GetText);
		INITIALIZE_PROPERTY(Text, height, SetHeight, GetHeight);
		INITIALIZE_PROPERTY(Text, verAlign, SetVerAlign, GetVerAlign);
		INITIALIZE_PROPERTY(Text, horAlign, SetHorAlign, GetHorAlign);
		INITIALIZE_PROPERTY(Text, wordWrap, SetWordWrap, GetWordWrap);
		INITIALIZE_PROPERTY(Text, dotsEngings, SetDotsEngings, IsDotsEngings);
		INITIALIZE_PROPERTY(Text, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef);
		INITIALIZE_PROPERTY(Text, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);
	}

	char* Text::mBasicSymbolsPreset = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	void Text::UpdateMesh()
	{
		if (mUpdatingMesh)
			return;

		mUpdatingMesh = true;

		if (!mFont)
		{
			for (auto mesh : mMeshes)
			{
				mesh->vertexCount = 0;
				mesh->polyCount = 0;
			}

			mUpdatingMesh = false;

			return;
		}

		int textLen = mText.Length();
		if (mMeshes.Count() == 0 && textLen == 0)
		{
			mUpdatingMesh = false;
			return;
		}

		PrepareMesh(textLen);

		for (auto mesh : mMeshes)
		{
			mesh->vertexCount = 0;
			mesh->polyCount = 0;
		}

		int currentMeshIdx = 0;
		Mesh* currentMesh = mMeshes[0];

		mSymbolsSet.Initialize(mFont, mText, mHeight, mTransform.offs, mSize, mHorAlign, mVerAlign, mWordWrap, mDotsEndings,
							   mSymbolsDistCoef, mLinesDistanceCoef);

		Basis transf = CalculateTextBasis();
		mLastTransform = transf;

		for (auto& line : mSymbolsSet.mLines)
		{
			for (auto& symb : line.mSymbols)
			{
				if (currentMesh->polyCount + 2 >= currentMesh->GetMaxPolyCount())
					currentMesh = mMeshes[currentMeshIdx++];

				unsigned long color = mColor.ABGR();
				Vec2F points[4] =
				{
					transf.Transform(symb.mFrame.LeftTop() - mSymbolsSet.mPosition),
					transf.Transform(symb.mFrame.RightTop() - mSymbolsSet.mPosition),
					transf.Transform(symb.mFrame.RightBottom() - mSymbolsSet.mPosition),
					transf.Transform(symb.mFrame.LeftBottom() - mSymbolsSet.mPosition)
				};
				
				currentMesh->vertices[currentMesh->vertexCount++] = Vertex2(points[0], color, symb.mTexSrc.left, 1.0f - symb.mTexSrc.top);
				currentMesh->vertices[currentMesh->vertexCount++] = Vertex2(points[1], color, symb.mTexSrc.right, 1.0f - symb.mTexSrc.top);
				currentMesh->vertices[currentMesh->vertexCount++] = Vertex2(points[2], color, symb.mTexSrc.right, 1.0f - symb.mTexSrc.bottom);
				currentMesh->vertices[currentMesh->vertexCount++] = Vertex2(points[3], color, symb.mTexSrc.left, 1.0f - symb.mTexSrc.bottom);

				int pp = currentMesh->polyCount*3;
				currentMesh->indexes[pp] = currentMesh->vertexCount - 4;
				currentMesh->indexes[pp + 1] = currentMesh->vertexCount - 3;
				currentMesh->indexes[pp + 2] = currentMesh->vertexCount - 2;
				currentMesh->polyCount++;

				pp += 3;
				currentMesh->indexes[pp] = currentMesh->vertexCount - 4;
				currentMesh->indexes[pp + 1] = currentMesh->vertexCount - 2;
				currentMesh->indexes[pp + 2] = currentMesh->vertexCount - 1;
				currentMesh->polyCount++;
			}
		}

		currentMesh->SetTexture(mFont->mTexture);

		mUpdatingMesh = false;
	}

	void Text::PrepareMesh(int charactersCount)
	{
		int needPolygons = charactersCount*2 + 5; // 5 for dots endings
		for (auto mesh : mMeshes)
			needPolygons -= mesh->GetMaxPolyCount();

		if (needPolygons <= 0)
			return;

		if (mMeshes.Count() > 0 &&
			needPolygons + mMeshes.Last()->GetMaxPolyCount() < mMeshMaxPolyCount)
		{
			mMeshes.Last()->Resize(mMeshes.Last()->GetMaxVertexCount() + (UInt)needPolygons*2,
								   mMeshes.Last()->GetMaxPolyCount() + (UInt)needPolygons);
			return;
		}

		while (needPolygons > 0)
		{
			int polyCount = Math::Min<int>(needPolygons, mMeshMaxPolyCount);
			needPolygons -= polyCount;
			mMeshes.Add(mnew Mesh(mFont->mTexture, polyCount * 2, polyCount));
		}
	}

	Basis Text::CalculateTextBasis() const
	{
		Basis transf;
		if (mSize == Vec2F())
		{
			transf = Basis::Build(mPosition, mScale, mAngle, mShear);
		}
		else
		{
			transf = mTransform;
			transf.xv /= mSize.x;
			transf.yv /= mSize.y;
		}

		return transf;
	}

	void Text::ColorChanged()
	{
		ULong dcolor = mColor.ABGR();
		for (auto mesh : mMeshes)
		{
			for (int i = 0; i < (int)mesh->vertexCount; i++)
				mesh->vertices[i].color = dcolor;
		}
	}

	void Text::BasisChanged()
	{
		if (mSymbolsSet.mAreaSize != mSize)
			UpdateMesh();
		else
		{
			Basis transform = CalculateTextBasis();
			Basis meshTransform = mLastTransform.Inverted()*transform;

			meshTransform.offs.x = Math::Round(meshTransform.offs.x);
			meshTransform.offs.y = Math::Round(meshTransform.offs.y);

			TransformMesh(mLastTransform.Inverted()*transform);
			mLastTransform = transform;
		}
	}

	void Text::OnDeserialized(const DataNode& node)
	{
		SetFontAsset(mFontAssetId);
	}

	void Text::TransformMesh(const Basis& bas)
	{
		if (bas == Basis::Identity())
			return;

		for (auto& mesh : mMeshes)
		{
			for (unsigned int i = 0; i < mesh->vertexCount; i++)
			{
				Vertex2* vx = &mesh->vertices[i];
				bas.Transform(vx->x, vx->y);
			}
		}

		mSymbolsSet.Move(bas.offs);
	}

	void Text::SymbolsSet::Initialize(FontRef font, const WString& text, int height, const Vec2F& position, const Vec2F& areaSize,
									  HorAlign horAlign, VerAlign verAlign, bool wordWrap, bool dotsEngings, 
									  float charsDistCoef, float linesDistCoef)
	{
		mFont = font;
		mText = text;
		mHeight = height;
		mPosition = Vec2F(Math::Round(position.x), Math::Round(position.y));
		mAreaSize = areaSize;
		mRealSize = Vec2F();
		mHorAlign = horAlign;
		mVerAlign = verAlign;
		mWordWrap = wordWrap;
		mSymbolsDistCoef = charsDistCoef;
		mLinesDistCoef = linesDistCoef;
		mDotsEndings = dotsEngings;

		mLines.Clear();
		int textLen = mText.Length();

		if (textLen == 0)
			return;

		float linesDist = mFont->GetLineHeightPx(mHeight)*mLinesDistCoef;
		float fontHeight = mFont->GetHeightPx(mHeight);

		mLines.Add(Line());
		Line* curLine = &mLines.Last();
		curLine->mSize.y = fontHeight;

		float dotsSize = mFont->GetCharacter('.', mHeight).mAdvance*3.0f;

		Vec2F fullSize(0, fontHeight);
		bool checkAreaBounds = mWordWrap && mAreaSize.x > FLT_EPSILON;
		int wrapCharIdx = -1;
		for (int i = 0; i < textLen; i++)
		{
			const Font::Character& ch = mFont->GetCharacter(mText[i], mHeight);
			Vec2F chSize = ch.mSize; 
			Vec2F chPos = Vec2F(curLine->mSize.x - ch.mOrigin.x, -ch.mOrigin.y); 
			
			if (mDotsEndings && mText[i] != '\n' && curLine->mSize.x + ch.mAdvance*mSymbolsDistCoef > mAreaSize.x - dotsSize)
			{
				const Font::Character& dotCh = mFont->GetCharacter('.', mHeight);
				Vec2F dotChSize = dotCh.mSize;

				for (int j = 0; j < 3; j++)
				{
					Vec2F dotChPos = Vec2F(curLine->mSize.x - dotCh.mOrigin.x, -dotCh.mOrigin.y);
					curLine->mSymbols.Add(Symbol(dotChPos, dotChSize, dotCh.mTexSrc, dotCh.mId, dotCh.mOrigin, dotCh.mAdvance));
					curLine->mString += '.'; 
					curLine->mSize.x += dotCh.mAdvance*mSymbolsDistCoef;
				}

				for (; i < textLen - 1; i++)
				{
					if (mText[i + 1] == '\n')
						break;
				}

				continue;
			}

			curLine->mSymbols.Add(Symbol(chPos, chSize, ch.mTexSrc, ch.mId, ch.mOrigin, ch.mAdvance));

			if (mText[i] != '\n')
				curLine->mSize.x += ch.mAdvance*mSymbolsDistCoef;

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

					curLine->mSymbols.RemoveRange(cutLen, curLine->mSymbols.Count());
					curLine->mString.Erase(cutLen);


					if (curLine->mSymbols.Count() > 0)
						curLine->mSize.x = curLine->mSymbols.Last().mFrame.right;
					else
						curLine->mSize.x = 0;

					i = wrapCharIdx;
					wrapCharIdx = -1;
				}
				else
				{
					curLine->mSymbols.PopBack();
					curLine->mString.PopBack();
					curLine->mEndedNewLine = true;
				}

				fullSize.x = Math::Max(fullSize.x, curLine->mSize.x);
				fullSize.y += linesDist;

				mLines.Add(Line());
				curLine = &mLines.Last();
				curLine->mSize.y = linesDist;
				curLine->mLineBegSymbol = i + 1;
			}
			else if (mText[i] == ' '/* || mFont->mAllSymbolReturn*/)
			{
				curLine->mSpacesCount++;
				wrapCharIdx = i;
			}
		}

		fullSize.x = Math::Max(fullSize.x, curLine->mSize.x);

		float lineHeight = linesDist;
		float yOffset = mAreaSize.y - mLines[0].mSize.y;

		if (mVerAlign == VerAlign::Both)
			lineHeight = Math::Max(fontHeight, (mAreaSize.y - fontHeight)/(float)(mLines.Count() - 1));
		else if (mVerAlign == VerAlign::Bottom)
			yOffset = fullSize.y - mLines[0].mSize.y;
		else if (mVerAlign == VerAlign::Middle)
			yOffset -= mAreaSize.y*0.5f - fullSize.y*0.5f;

		yOffset += mPosition.y;

		for (LineDefsVec::Iterator it = mLines.begin(); it != mLines.end(); ++it)
		{
			Line* line = &(*it);

			float xOffset = 0;
			float additiveSpaceOffs = 0;

			if (mHorAlign == HorAlign::Middle)
				xOffset = (mAreaSize.x - line->mSize.x)*0.5f;
			else if (mHorAlign == HorAlign::Right)
				xOffset = mAreaSize.x - line->mSize.x;
			else if (mHorAlign == HorAlign::Both)
				additiveSpaceOffs = Math::Max(0.0f, (mAreaSize.x - line->mSize.x)/(float)line->mSpacesCount);

			xOffset += mPosition.x;

			Vec2F locOrigin((float)(int)xOffset, (float)(int)yOffset);
			line->mPosition = locOrigin;
			yOffset -= lineHeight;

			for (SymbolDefsVec::Iterator jt = line->mSymbols.begin(); jt != line->mSymbols.end(); ++jt)
			{
				if (jt->mCharId == ' ')
					locOrigin.x += additiveSpaceOffs;

				jt->mFrame = jt->mFrame + locOrigin;
			}
		}

		mRealSize = fullSize;
	}

	void Text::SymbolsSet::Move(const Vec2F& offs)
	{
		for (auto& line : mLines)
		{
			for (auto& sm : line.mSymbols)
				sm.mFrame += offs;

			line.mPosition += offs;
		}
	}

	Text::SymbolsSet::Symbol::Symbol()
	{}

	Text::SymbolsSet::Symbol::Symbol(const Vec2F& position, const Vec2F& size, const RectF& texSrc,
										   UInt16 charId, const Vec2F& origin, float advance):
		mFrame(position, position + size), mTexSrc(texSrc), mCharId(charId), mOrigin(origin), mAdvance(advance)
	{}

	bool Text::SymbolsSet::Symbol::operator==(const Symbol& other) const
	{
		return mCharId == other.mCharId && mFrame == other.mFrame;
	}

	Text::SymbolsSet::Line::Line():
		mLineBegSymbol(0), mSpacesCount(0), mEndedNewLine(false)
	{}

	bool Text::SymbolsSet::Line::operator==(const Line& other) const
	{
		return false;
	}
}
 
CLASS_META(o2::Text)
{
	BASE_CLASS(o2::IRectDrawable);

	PUBLIC_FIELD(font);
	PUBLIC_FIELD(text);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(verAlign);
	PUBLIC_FIELD(horAlign);
	PUBLIC_FIELD(wordWrap);
	PUBLIC_FIELD(dotsEngings);
	PUBLIC_FIELD(symbolsDistanceCoef);
	PUBLIC_FIELD(linesDistanceCoef);
	PROTECTED_FIELD(mMeshMaxPolyCount);
	PROTECTED_FIELD(mText).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFontAssetId).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFont);
	PROTECTED_FIELD(mHeight).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSymbolsDistCoef).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLinesDistanceCoef).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerAlign).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHorAlign).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mWordWrap).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDotsEndings).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMeshes);
	PROTECTED_FIELD(mLastTransform);
	PROTECTED_FIELD(mSymbolsSet);
	PROTECTED_FIELD(mUpdatingMesh);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetFont, FontRef);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(void, SetFontAsset, BitmapFontAsset*);
	PUBLIC_FUNCTION(void, SetFontAsset, VectorFontAsset*);
	PUBLIC_FUNCTION(void, SetFontAsset, UID);
	PUBLIC_FUNCTION(void, SetFontAsset, const String&);
	PUBLIC_FUNCTION(Asset*, GetFontAsset);
	PUBLIC_FUNCTION(UID, GetFontAssetId);
	PUBLIC_FUNCTION(void, SetHeight, int);
	PUBLIC_FUNCTION(int, GetHeight);
	PUBLIC_FUNCTION(void, SetText, const WString&);
	PUBLIC_FUNCTION(WString, GetText);
	PUBLIC_FUNCTION(void, SetHorAlign, HorAlign);
	PUBLIC_FUNCTION(HorAlign, GetHorAlign);
	PUBLIC_FUNCTION(void, SetVerAlign, VerAlign);
	PUBLIC_FUNCTION(VerAlign, GetVerAlign);
	PUBLIC_FUNCTION(void, SetWordWrap, bool);
	PUBLIC_FUNCTION(bool, GetWordWrap);
	PUBLIC_FUNCTION(void, SetDotsEngings, bool);
	PUBLIC_FUNCTION(bool, IsDotsEngings);
	PUBLIC_FUNCTION(void, SetSymbolsDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetSymbolsDistanceCoef);
	PUBLIC_FUNCTION(void, SetLinesDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetLinesDistanceCoef);
	PUBLIC_FUNCTION(SymbolsSet&, GetSymbolsSet);
	PUBLIC_FUNCTION(Vec2F, GetRealSize);
	PUBLIC_FUNCTION(RectF, GetRealRect);
	PROTECTED_FUNCTION(void, UpdateMesh);
	PROTECTED_FUNCTION(void, TransformMesh, const Basis&);
	PROTECTED_FUNCTION(void, PrepareMesh, int);
	PROTECTED_FUNCTION(Basis, CalculateTextBasis);
	PROTECTED_FUNCTION(void, ColorChanged);
	PROTECTED_FUNCTION(void, BasisChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 