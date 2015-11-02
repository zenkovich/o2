#include "Text.h"

#include "Assets/Assets.h"
#include "Assets/BitmapFontAsset.h"
#include "Assets/VectorFontAsset.h"
#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Utils/Debug.h"

namespace o2
{
	IOBJECT_CPP(Text);

	Text::Text():
		mFont(nullptr), mSymbolsDistCoef(1), mLinesDistanceCoef(1), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable()
	{
		InitializeProperties();
	}

	Text::Text(FontRef font):
		mFont(font), mSymbolsDistCoef(1), mLinesDistanceCoef(1), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable()
	{
		InitializeProperties();
	}

	Text::Text(const Text& text):
		IRectDrawable(text)
	{
		mText = text.mText;
		mFont = text.mFont;
		mSymbolsDistCoef = text.mSymbolsDistCoef;
		mLinesDistanceCoef = text.mLinesDistanceCoef;
		mVerAlign = text.mVerAlign;
		mHorAlign = text.mHorAlign;
		mWordWrap = text.mWordWrap;

		InitializeProperties();
	}

	Text::Text(Ptr<BitmapFontAsset> fontAsset):
		Text(fontAsset->GetFont())
	{
		mFontAssetId = fontAsset->GetAssetId();
	}

	Text::Text(Ptr<VectorFontAsset> fontAsset):
		Text(fontAsset->GetFont())
	{
		mFontAssetId = fontAsset->GetAssetId();
	}

	Text::Text(const String& fontFileName):
		Text(o2Assets.GetAssetId(fontFileName))
	{}

	Text::Text(AssetId fontAssetId):
		mSymbolsDistCoef(1), mLinesDistanceCoef(1), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable()
	{
		InitializeProperties();
		SetFontAsset(fontAssetId);
	}

	Text::~Text()
	{
		for (auto mesh : mMeshes)
			mesh.Release();
	}

	Text& Text::operator=(const Text& other)
	{
		IRectDrawable::operator=(other);

		mText = other.mText;
		mFontAssetId = other.mFontAssetId;
		mFont = other.mFont;
		mSymbolsDistCoef = other.mSymbolsDistCoef;
		mLinesDistanceCoef = other.mLinesDistanceCoef;
		mVerAlign = other.mVerAlign;
		mHorAlign = other.mHorAlign;
		mWordWrap = other.mWordWrap;

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
	}

	void Text::SetFont(FontRef font)
	{
		if (font == mFont)
			return;

		mFont = font;

		UpdateMesh();
	}

	FontRef Text::GetFont() const
	{
		return mFont;
	}

	void Text::SetFontAsset(Ptr<BitmapFontAsset> asset)
	{
		mFont = asset->GetFont();
		mFontAssetId = asset->GetAssetId();
	}

	void Text::SetFontAsset(Ptr<VectorFontAsset> asset)
	{
		mFont = asset->GetFont();
		mFontAssetId = asset->GetAssetId();
	}

	void Text::SetFontAsset(AssetId assetId)
	{
		if (!o2Assets.IsAssetExist(assetId))
		{
			o2Debug.LogError("Can't load font asset: %i - isn't exist", assetId);
			return;
		}

		mFontAssetId = assetId;
		AssetInfo fontAssetInfo = o2Assets.GetAssetInfo(mFontAssetId);
		if (fontAssetInfo.GetTypeId() == BitmapFontAsset::type->ID())
		{
			auto asset = BitmapFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}
		else
		{
			auto asset = VectorFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}

		UpdateMesh();
	}

	void Text::SetFontAsset(const String& fileName)
	{
		if (!o2Assets.IsAssetExist(fileName))
		{
			o2Debug.LogError("Can't load font asset: %s - isn't exist", fileName);
			return;
		}
		
		AssetInfo fontAssetInfo = o2Assets.GetAssetInfo(fileName);
		mFontAssetId = fontAssetInfo.mId;

		if (fontAssetInfo.GetTypeId() == BitmapFontAsset::type->ID())
		{
			auto asset = BitmapFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}
		else
		{
			auto asset = VectorFontAsset(mFontAssetId);
			mFont = asset.GetFont();
		}
	}

	Ptr<Asset> Text::GetFontAsset() const
	{
		AssetInfo fontAssetInfo = o2Assets.GetAssetInfo(mFontAssetId);
		if (fontAssetInfo.GetTypeId() == BitmapFontAsset::type->ID())
			return mnew BitmapFontAsset(mFontAssetId);
		
		return mnew VectorFontAsset(mFontAssetId);
	}

	AssetId Text::GetFontAssetId() const
	{
		return mFontAssetId;
	}

	void Text::SetText(const WString& text)
	{
		if (mText == text)
			return;

		mText = text;
		UpdateMesh();
	}

	WString Text::GetText() const
	{
		return mText;
	}

	void Text::SetCText(const String& text)
	{
		if (mText == text)
			return;

		mText = text;
		UpdateMesh();
	}

	String Text::GetCText() const
	{
		return mText;
	}

	void Text::SetHorAlign(HorAlign align)
	{
		if (align == mHorAlign)
			return;

		mHorAlign = align;
		UpdateMesh();
	}

	Text::HorAlign Text::GetHorAlign() const
	{
		return mHorAlign;
	}

	void Text::SetVerAlign(VerAlign align)
	{
		if (align == mVerAlign)
			return;

		mVerAlign = align;
		UpdateMesh();
	}

	Text::VerAlign Text::GetVerAlign() const
	{
		return mVerAlign;
	}

	void Text::SetWordWrap(bool flag)
	{
		if (flag == mWordWrap)
			return;

		mWordWrap = flag;
		UpdateMesh();
	}

	bool Text::GetWordWrap() const
	{
		return mWordWrap;
	}

	void Text::SetSymbolsDistanceCoef(float coef)
	{
		if (coef == mSymbolsDistCoef)
			return;

		mSymbolsDistCoef = coef;
		UpdateMesh();
	}

	float Text::GetSymbolsDistanceCoef() const
	{
		return mSymbolsDistCoef;
	}

	void Text::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		if (Math::Equals(coef, mLinesDistanceCoef))
			return;

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

	Vec2F Text::GetTextSize(const WString& text, Ptr<Font> font, const Vec2F& areaSize /*= Vec2F()*/,
							HorAlign horAlign /*= HorAlign::Left*/, VerAlign verAlign /*= VerAlign::Top*/,
							bool wordWrap /*= true*/, float charsDistCoef /*= 1.0f*/, float linesDistCoef /*= 1.0f*/)
	{
		SymbolsSet textSet;
		textSet.Initialize(font, text, Vec2F(), areaSize, horAlign, verAlign, wordWrap, charsDistCoef, linesDistCoef);
		return textSet.mRealSize;
	}

	void Text::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Text, font, SetFont, GetFont);
		INITIALIZE_PROPERTY(Text, text, SetText, GetText);
		INITIALIZE_PROPERTY(Text, ctext, SetCText, GetCText);
		INITIALIZE_PROPERTY(Text, verAlign, SetVerAlign, GetVerAlign);
		INITIALIZE_PROPERTY(Text, horAlign, SetHorAlign, GetHorAlign);
		INITIALIZE_PROPERTY(Text, wordWrap, SetWordWrap, GetWordWrap);
		INITIALIZE_PROPERTY(Text, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef);
		INITIALIZE_PROPERTY(Text, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);
	}

	void Text::UpdateMesh()
	{
		if (!mFont)
		{
			for (auto mesh : mMeshes)
			{
				mesh->vertexCount = 0;
				mesh->polyCount = 0;
			}

			return;
		}

		int textLen = mText.Length();
		if (mMeshes.Count() == 0 && textLen == 0)
			return;

		PrepareMesh(textLen);

		for (auto mesh : mMeshes)
		{
			mesh->vertexCount = 0;
			mesh->polyCount = 0;
		}

		int currentMeshIdx = 0;
		Ptr<Mesh> currentMesh = mMeshes[0];

		mSymbolsSet.Initialize(font, mText, mTransform.offs, mSize, mHorAlign, mVerAlign, mWordWrap, mSymbolsDistCoef,
							   mLinesDistanceCoef);

		Basis transf = CalculateTextBasis();
		mLastTransform = transf;

		for (auto& line : mSymbolsSet.mLineDefs)
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

				currentMesh->indexes[currentMesh->polyCount*3] = currentMesh->vertexCount - 4;
				currentMesh->indexes[currentMesh->polyCount*3 + 1] = currentMesh->vertexCount - 3;
				currentMesh->indexes[currentMesh->polyCount*3 + 2] = currentMesh->vertexCount - 2;
				currentMesh->polyCount++;

				currentMesh->indexes[currentMesh->polyCount*3] = currentMesh->vertexCount - 4;
				currentMesh->indexes[currentMesh->polyCount*3 + 1] = currentMesh->vertexCount - 2;
				currentMesh->indexes[currentMesh->polyCount*3 + 2] = currentMesh->vertexCount - 1;
				currentMesh->polyCount++;
			}
		}

		currentMesh->SetTexture(mFont->mTexture);
	}

	void Text::PrepareMesh(int charactersCount)
	{
		int needPolygons = charactersCount*2;
		for (auto mesh : mMeshes)
			needPolygons -= mesh->GetMaxPolyCount();

		if (needPolygons <= 0)
			return;

		if (needPolygons < 100 && mMeshes.Count() > 0 &&
			needPolygons + mMeshes.Last()->GetMaxPolyCount() < mMeshMaxPolyCount)
		{
			mMeshes.Last()->Resize(mMeshes.Last()->GetMaxVertexCount() + (UInt)needPolygons*2,
								   mMeshes.Last()->GetMaxPolyCount() + (UInt)needPolygons);
			return;
		}

		for (int i = 0; i < charactersCount/needPolygons + 1; i++)
			mMeshes.Add(mnew Mesh(mFont->mTexture, mMeshMaxPolyCount*2, mMeshMaxPolyCount));
	}

	Basis Text::CalculateTextBasis() const
	{
		Basis transf;
		if (mSize == Vec2F())
		{
			transf = Basis::Build(mPosition, mScale, mAngle, mShear);
			transf.offs = transf.offs - transf.xv*mPivot.x - transf.yv*mPivot.y;
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
			Basis transf = CalculateTextBasis();
			TransformMesh(mLastTransform.Inverted()*transf);
			mLastTransform = transf;
		}
	}

	void Text::OnDeserialized(const DataNode& node)
	{
		SetFontAsset(mFontAssetId);
	}

	void Text::TransformMesh(const Basis& bas)
	{
		for (auto& mesh : mMeshes)
		{
			for (unsigned int i = 0; i < mesh->vertexCount; i++)
			{
				Vertex2* vx = &mesh->vertices[i];
				bas.Transform(vx->x, vx->y);
			}
		}
	}

	void Text::SymbolsSet::Initialize(FontRef font, const WString& text, const Vec2F& position, const Vec2F& areaSize,
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
		mSymbolsDistCoef = charsDistCoef;
		mLinesDistCoef = linesDistCoef;

		mLineDefs.Clear();
		int textLen = mText.Length();

		if (textLen == 0)
			return;

		mFont->CheckCharacters(text);

		float linesDist = mFont->GetLineHeight()*mLinesDistCoef;
		float fontHeight = mFont->GetHeight();

		mLineDefs.Add(LineDef());
		LineDef* curLine = &mLineDefs.Last();
		curLine->mSize.y = fontHeight;

		Vec2F fullSize(0, fontHeight);
		bool checkAreaBounds = mWordWrap && mAreaSize.x > FLT_EPSILON;
		int wrapCharIdx = -1;
		for (int i = 0; i < textLen; i++)
		{
			const Font::Character& ch = mFont->GetCharacter(mText[i]);
			Vec2F chSize = ch.mSize; 
			Vec2F chPos = Vec2F(curLine->mSize.x - ch.mOrigin.x, -ch.mOrigin.y);

			curLine->mSymbols.Add(SymbolDef(chPos, chSize, ch.mTexSrc, ch.mId, ch.mOrigin, ch.mAdvance));

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

				mLineDefs.Add(LineDef());
				curLine = &mLineDefs.Last();
				curLine->mSize.y = linesDist;
				curLine->mLineBegSymbol = i + 1;
				fullSize.x = Math::Max(fullSize.x, curLine->mSize.x);
				fullSize.y += linesDist;
			}
			else if (mText[i] == ' '/* || mFont->mAllSymbolReturn*/)
			{
				curLine->mSpacesCount++;
				wrapCharIdx = i;
			}
		}

		fullSize.x = Math::Max(fullSize.x, curLine->mSize.x);

		float lineHeight = linesDist;
		float yOffset = mAreaSize.y - mLineDefs[0].mSize.y;

		if (mVerAlign == VerAlign::Both)
			lineHeight = Math::Max(fontHeight, (mAreaSize.y - fontHeight)/(float)(mLineDefs.Count() - 1));
		else if (mVerAlign == VerAlign::Bottom)
			yOffset = fullSize.y - mLineDefs[0].mSize.y;
		else if (mVerAlign == VerAlign::Middle)
			yOffset -= mAreaSize.y*0.5f - fullSize.y*0.5f;

		yOffset += mPosition.y;
// 
// 		o2Debug.DrawRect(RectF(Vec2F(), fullSize), Color4::Red());

		for (LineDefsVec::Iterator it = mLineDefs.begin(); it != mLineDefs.end(); ++it)
		{
			LineDef* line = &(*it);

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

// 
// 			o2Debug.DrawRect(RectF(locOrigin, locOrigin + line->mSize), Color4::Red());
// 			o2Debug.DrawRect(RectF(locOrigin, locOrigin + Vec2F(10, linesDist)), Color4::Blue());

			for (SymbolDefsVec::Iterator jt = line->mSymbols.begin(); jt != line->mSymbols.end(); ++jt)
			{
				if (jt->mCharId == ' ')
					locOrigin.x += additiveSpaceOffs;

				jt->mFrame = jt->mFrame + locOrigin;
			}

			mRealSize.x = Math::Max(mRealSize.x, line->mSize.x);
			mRealSize.y = Math::Max(mRealSize.y, line->mSize.y + line->mPosition.y - mPosition.y);
		}
	}

	Text::SymbolsSet::SymbolDef::SymbolDef()
	{}

	Text::SymbolsSet::SymbolDef::SymbolDef(const Vec2F& position, const Vec2F& size, const RectF& texSrc,
										   UInt16 charId, const Vec2F& origin, float advance):
		mFrame(position, position + size), mTexSrc(texSrc), mCharId(charId), mOrigin(origin), mAdvance(advance)
	{}

	bool Text::SymbolsSet::SymbolDef::operator==(const SymbolDef& other) const
	{
		return mCharId == other.mCharId && mFrame == other.mFrame;
	}

	Text::SymbolsSet::LineDef::LineDef():
		mLineBegSymbol(0), mSpacesCount(0), mEndedNewLine(false)
	{}

	bool Text::SymbolsSet::LineDef::operator==(const LineDef& other) const
	{
		return false;
	}
}