#include "Text.h"

#include "Render/Mesh.h"
#include "Render/Render.h"

namespace o2
{
	Text::Text():
		mFont(nullptr), mSymbolsDistCoef(1), mLinesDistance(0), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable()
	{
		InitializeProperties();
	}

	Text::Text(Ptr<Font> font):
		mFont(font), mSymbolsDistCoef(1), mLinesDistance(0), mVerAlign(VerAlign::Top),
		mHorAlign(HorAlign::Left), mWordWrap(false), IRectDrawable()
	{
		if (mFont)
			mLinesDistance = Math::Floor(mFont->GetHeight()*1.5f);
		InitializeProperties();
	}

	Text::Text(const Text& text):
		IRectDrawable(text)
	{
		mText = text.mText;
		mFont = text.mFont;
		mSymbolsDistCoef = text.mSymbolsDistCoef;
		mLinesDistance = text.mLinesDistance;
		mVerAlign = text.mVerAlign;
		mHorAlign = text.mHorAlign;
		mWordWrap = text.mWordWrap;

		InitializeProperties();
	}

	Text::~Text()
	{
		for (auto mesh : mMeshes)
			mesh.Release();
	}

	Text* Text::Clone() const
	{
		return mnew Text(*this);
	}

	void Text::Draw()
	{
		if (!mEnabled)
			return;

		for (auto mesh : mMeshes)
		{
			mesh->Draw();
			//o2Render.DrawMeshWire(mesh, Color4(0, 255, 0, 50));
		}
	}

	void Text::SetFont(Ptr<Font> font)
	{
		if (font == mFont)
			return;

		mFont = font;
		UpdateMesh();
	}

	Ptr<Font> Text::GetFont() const
	{
		return mFont;
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

	void Text::SetSymbolsDistCoef(float coef)
	{
		if (coef == mSymbolsDistCoef)
			return;

		mSymbolsDistCoef = coef;
		UpdateMesh();
	}

	float Text::GetSymbolsDistCoef() const
	{
		return mSymbolsDistCoef;
	}

	void Text::SetLinesDistance(float distance)
	{
		if (distance == mLinesDistance)
			return;

		mLinesDistance = distance;
		UpdateMesh();
	}

	float Text::GetLinesDistance() const
	{
		return mLinesDistance;
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
		INITIALIZE_PROPERTY(Text, symbolsDistCoef, SetSymbolsDistCoef, GetSymbolsDistCoef);
		INITIALIZE_PROPERTY(Text, linesDistance, SetLinesDistance, GetLinesDistance);
	}

	void Text::UpdateMesh()
	{
		if (!mFont)
		{
			for (auto mesh : mMeshes)
			{
				mesh->mVertexCount = 0;
				mesh->mPolyCount = 0;
			}

			return;
		}

		int textLen = mText.Length();
		if (mMeshes.Count() == 0 && textLen == 0)
			return;

		PrepareMesh(textLen);

		for (auto mesh : mMeshes)
		{
			mesh->mVertexCount = 0;
			mesh->mPolyCount = 0;
		}

		int currentMeshIdx = 0;
		Ptr<Mesh> currentMesh = mMeshes[0];

		mSymbolsSet.Initialize(font, mText, mTransform.offs, mSize, mHorAlign, mVerAlign, mWordWrap, mSymbolsDistCoef,
							   mLinesDistance);

		Basis transf = CalculateTextBasis();
		mLastTransform = transf;

		for (auto& line : mSymbolsSet.mLineDefs)
		{
			for (auto& symb : line.mSymbols)
			{
				if (currentMesh->mPolyCount + 2 >= currentMesh->GetMaxPolyCount())
					currentMesh = mMeshes[currentMeshIdx++];

				unsigned long color = mColor.ABGR();
				Vec2F points[4] =
				{
					transf.Transform(symb.mFrame.LeftTop() - mSymbolsSet.mPosition),
					transf.Transform(symb.mFrame.RightTop() - mSymbolsSet.mPosition),
					transf.Transform(symb.mFrame.RightBottom() - mSymbolsSet.mPosition),
					transf.Transform(symb.mFrame.LeftBottom() - mSymbolsSet.mPosition)
				};
				
				currentMesh->mVertices[currentMesh->mVertexCount++] = Vertex2(points[0], color, symb.mTexSrc.left, 1.0f - symb.mTexSrc.top);
				currentMesh->mVertices[currentMesh->mVertexCount++] = Vertex2(points[1], color, symb.mTexSrc.right, 1.0f - symb.mTexSrc.top);
				currentMesh->mVertices[currentMesh->mVertexCount++] = Vertex2(points[2], color, symb.mTexSrc.right, 1.0f - symb.mTexSrc.bottom);
				currentMesh->mVertices[currentMesh->mVertexCount++] = Vertex2(points[3], color, symb.mTexSrc.left, 1.0f - symb.mTexSrc.bottom);

				currentMesh->mIndexes[currentMesh->mPolyCount*3] = currentMesh->mVertexCount - 4;
				currentMesh->mIndexes[currentMesh->mPolyCount*3 + 1] = currentMesh->mVertexCount - 3;
				currentMesh->mIndexes[currentMesh->mPolyCount*3 + 2] = currentMesh->mVertexCount - 2;
				currentMesh->mPolyCount++;

				currentMesh->mIndexes[currentMesh->mPolyCount*3] = currentMesh->mVertexCount - 4;
				currentMesh->mIndexes[currentMesh->mPolyCount*3 + 1] = currentMesh->mVertexCount - 2;
				currentMesh->mIndexes[currentMesh->mPolyCount*3 + 2] = currentMesh->mVertexCount - 1;
				currentMesh->mPolyCount++;
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

	void Text::BasisChanged()
	{
		if (mSymbolsSet.mAreaSize != mSize || true)
			UpdateMesh();
		else
		{
			Basis transf = CalculateTextBasis();
			TransformMesh(mLastTransform.Inverted()*transf);
			mLastTransform = transf;
		}
	}

	void Text::TransformMesh(const Basis& bas)
	{
		for (auto& mesh : mMeshes)
		{
			for (unsigned int i = 0; i < mesh->mVertexCount; i++)
			{
				Vertex2* vx = &mesh->mVertices[i];
				bas.Transform(vx->x, vx->y);
			}
		}
	}

	void Text::SymbolsSet::Initialize(Ptr<Font> font, const WString& text, const Vec2F& position, const Vec2F& areaSize,
									  HorAlign horAlign, VerAlign verAlign, bool wordWrap, float charsDistCoef,
									  float linesDistance)
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
		mLinesDistance = linesDistance;

		mLineDefs.Clear();
		int textLen = mText.Length();

		if (textLen == 0)
			return;

		mFont->CheckCharacters(text);

		mLineDefs.Add(LineDef());
		LineDef* curLine = &mLineDefs.Last();
		curLine->mSize.y = mLinesDistance;

		Vec2F fullSize(0, mFont->GetHeight());
		bool checkAreaBounds = mWordWrap && mAreaSize.x > FLT_EPSILON;
		int wrapCharIdx = -1;
		for (int i = 0; i < textLen; i++)
		{
			const Font::Character& ch = mFont->GetCharacter(mText[i]);
			Vec2F chSize = ch.mSize;
			Vec2F chPos = Vec2F(curLine->mSize.x, 0) + ch.mOffset;

			curLine->mSymbols.Add(SymbolDef(chPos, chSize, ch.mTexSrc, ch.mId, ch.mOffset, ch.mAdvance));
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
				curLine->mSize.y = mLinesDistance;
				curLine->mLineBegSymbol = i + 1;
				fullSize.x = Math::Max(fullSize.x, curLine->mSize.x);
				fullSize.y += mLinesDistance;
			}
			else if (mText[i] == ' '/* || mFont->mAllSymbolReturn*/)
			{
				curLine->mSpacesCount++;
				wrapCharIdx = i;
			}
		}

		fullSize.x = Math::Max(fullSize.x, curLine->mSize.x);

		float lineHeight = mLinesDistance;
		float yOffset = mAreaSize.y + (mLinesDistance - mFont->GetHeight());

		if (mVerAlign == VerAlign::Both)
			lineHeight = (mAreaSize.y - lineHeight + mFont->GetHeight())/(float)(mLineDefs.Count() - 1);
		else if (mVerAlign == VerAlign::Bottom)
			yOffset = fullSize.y;
		else if (mVerAlign == VerAlign::Middle)
			yOffset -= mAreaSize.y*0.5f - fullSize.y*0.5f;

		yOffset += mPosition.y;

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
				additiveSpaceOffs = (mAreaSize.x - line->mSize.x)/(float)line->mSpacesCount;

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

			mRealSize.x = Math::Max(mRealSize.x, line->mSize.x);
			mRealSize.y = Math::Max(mRealSize.y, line->mSize.y + line->mPosition.y - mPosition.y);
		}
	}

	Text::SymbolsSet::SymbolDef::SymbolDef()
	{}

	Text::SymbolsSet::SymbolDef::SymbolDef(const Vec2F& position, const Vec2F& size, const RectF& texSrc,
										   UInt16 charId, const Vec2F& offs, float advance):
		mFrame(position, position + size), mTexSrc(texSrc), mCharId(charId), mOffset(offs), mAdvance(advance)
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