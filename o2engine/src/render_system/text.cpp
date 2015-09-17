#include "text.h"

#include "font.h"
#include "mesh.h"
#include "render_system.h"

OPEN_O2_NAMESPACE

Text::Text(Font* font):
	mFont(font), mCharactersDistCoef(1), mLinesDistCoef(1.5f), mVerAlign(Font::VA_TOP), mHorAlign(Font::HA_LEFT),
	mNeedUpdateMesh(true), mWordWrap(false), IRectDrawable()
{
	InitializeProperties();
}

Text::Text(const Text& text):
	IRectDrawable(text)
{
	mText = text.mText;
	mFont = text.mFont;
	mTransform = text.mTransform;
	mTransformDef = text.mTransformDef;
	mCharactersDistCoef = text.mCharactersDistCoef;
	mLinesDistCoef = text.mLinesDistCoef;
	mVerAlign = text.mVerAlign;
	mHorAlign = text.mHorAlign;
	mWordWrap = text.mWordWrap;

	mNeedUpdateMesh = true;
	mNeedTransformMesh = false;

	InitializeProperties();
}

Text::~Text()
{
	for (MeshesArr::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		SafeRelease(*it);
}

IRectDrawable* Text::Clone() const
{
	return mnew Text(*this);
}

void Text::Draw()
{
	if (!mEnabled)
		return;

	if (mNeedUpdateMesh)
		UpdateMesh();

	if (mNeedTransformMesh)
	{
		mLastTransform = mTransform;
		mTransform = mTransformDef.Build();
		TransformMesh(mLastTransform.Inverted()*mTransform);
	}

	for (MeshesArr::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		(*it)->Draw();
}

void Text::SetFont(Font* font)
{
	if (font == mFont)
		return;

	mFont = font;
	mNeedUpdateMesh = true;
}

Font* Text::GetFont() const
{
	return mFont;
}

void Text::GetText(const WString& text)
{
	mText = text;
	mNeedUpdateMesh = true;
}

WString Text::GetText() const
{
	return mText;
}

void Text::SetCText(const String& text)
{
	mText = ConvString2Wide(text);
	mNeedUpdateMesh = true;
}

String Text::GetCText() const
{
	return ConvWide2String(mText);
}

void Text::SetAngle(const float& angle)
{
	if (Equals(angle, mTransformDef.mAngle))
		return;

	mTransformDef.mAngle = angle;
	mNeedTransformMesh = true;
}

float Text::GetAngle() const
{
	return mTransformDef.mAngle;
}

void Text::SetScale(const Vec2F& scale)
{
	if (Equals(scale, mTransformDef.mScale))
		return;

	mTransformDef.mScale = scale;
	mNeedTransformMesh = true;
}

Vec2F Text::GetScale() const
{
	return mTransformDef.mScale;
}

void Text::SetCharactersHeight(const float& height)
{
	SetScale(Vec2F(mTransformDef.mScale.x, height/mFont->GetBase()));
}

float Text::GetCharactersHeight() const
{
	return mTransformDef.mScale.y*mFont->GetBase();
}

void Text::SetTransform(const Basis& bas)
{
	mLastTransform = mTransform;
	mTransform = bas;
	TransformMesh(mTransform*mLastTransform.Inverted());
}

Basis Text::GetTransform() const
{
	return mTransform;
}

void Text::SetTransformDef(const BasisDef& def)
{
	mTransformDef = def;
	mNeedTransformMesh = true;
}

BasisDef Text::GetTransformDef() const
{
	return mTransformDef;
}

void Text::SetHorAlign(const Font::HorAlign& align)
{
	if (align == mHorAlign)
		return;

	mHorAlign = align;
	mNeedUpdateMesh = true;
}

Font::HorAlign Text::GetHorAlign() const
{
	return mHorAlign;
}

void Text::SetVerAlign(const Font::VerAlign& align)
{
	if (align == mVerAlign)
		return;

	mVerAlign = align;
	mNeedUpdateMesh = true;
}

Font::VerAlign Text::GetVerAlign() const
{
	return mVerAlign;
}

void Text::SetWordWrap(const bool& flag)
{
	if (flag == mWordWrap)
		return;

	mWordWrap = flag;
	mNeedUpdateMesh = true;
}

bool Text::GetWordWrap() const
{
	return mWordWrap;
}

void Text::SetCharactersDistCoef(const float& coef)
{
	if (coef == mCharactersDistCoef)
		return;

	mCharactersDistCoef = coef;
	mNeedUpdateMesh = true;
}

float Text::GetCharactersDistCoef() const
{
	return mCharactersDistCoef;
}

void Text::SetLinesDistCoef(const float& coef)
{
	if (coef == mLinesDistCoef)
		return;

	mLinesDistCoef = coef;
	mNeedUpdateMesh = true;
}

float Text::GetLinesDistCoef() const
{
	return mLinesDistCoef;
}

Font::TextSymbolsSet* Text::GetSymbolsSet()
{
	return &mSymbolsSet;
}

Vec2F Text::GetRealSize()
{
	ForceUpdateMesh();
	return mSymbolsSet.mRealSize;
}

void Text::ForceUpdateMesh()
{
	if (mNeedUpdateMesh || mNeedTransformMesh)
		UpdateMesh();
}

void Text::InitializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(Text, font, SetFont, GetFont);
	REG_PROPERTY(Text, text, GetText, GetText);
	REG_PROPERTY(Text, ctext, SetCText, GetCText);
	REG_PROPERTY(Text, verAlign, SetVerAlign, GetVerAlign);
	REG_PROPERTY(Text, horAlign, SetHorAlign, GetHorAlign);
	REG_PROPERTY(Text, wordWrap, SetWordWrap, GetWordWrap);
	REG_PROPERTY(Text, angle, SetAngle, GetAngle);
	REG_PROPERTY(Text, scale, SetScale, GetScale);
	REG_PROPERTY(Text, charactersHeight, SetCharactersHeight, GetCharactersHeight);
	REG_PROPERTY(Text, transform, SetTransform, GetTransform);
	REG_PROPERTY(Text, transformDef, SetTransformDef, GetTransformDef);
	REG_PROPERTY(Text, charactersDistCoef, SetCharactersDistCoef, GetCharactersDistCoef);
	REG_PROPERTY(Text, linesDistCoef, SetLinesDistCoef, GetLinesDistCoef);
}

void Text::UpdateMesh()
{
	mNeedUpdateMesh = false;
	mNeedTransformMesh = false;
	mTransform = mTransformDef.Build();

	int textLen = mText.length();
	if (mMeshes.size() == 0 && textLen == 0)
		return;

	PrepareMesh(textLen);

	FOREACH(MeshesArr, mMeshes, it)
	{
		(*it)->mVertexCount = 0; (*it)->mPolyCount = 0;
	}

	int currentMeshIdx = 0;
	Mesh* currentMesh = mMeshes[0];

	mSymbolsSet.Initialize(font, mText, mPosition, mSize, mHorAlign, mVerAlign, mWordWrap, mCharactersDistCoef, mLinesDistCoef);
	Basis zeroPositionTransform = mTransform;
	zeroPositionTransform.offs = Vec2F();

	FOREACH(Font::TextSymbolsSet::LineDefsArr, mSymbolsSet.mLineDefs, it)
	{
		FOREACH(Font::TextSymbolsSet::SymbolDefsArr, it->mSymbols, jt)
		{
			if (currentMesh->mPolyCount + 2 >= currentMesh->GetMaxPolyCount())
				currentMesh = mMeshes[currentMeshIdx++];

			unsigned long color = mColor.AsULong();
			Vec2F points[4] ={zeroPositionTransform.Transform(jt->mFrame.LeftTop()),
				zeroPositionTransform.Transform(jt->mFrame.RightTop()),
				zeroPositionTransform.Transform(jt->mFrame.RightBottom()),
				zeroPositionTransform.Transform(jt->mFrame.LeftBottom())};

			currentMesh->mVerticies[currentMesh->mVertexCount++] = Vertex2(points[0], color, jt->mTexSrc.left, jt->mTexSrc.top);
			currentMesh->mVerticies[currentMesh->mVertexCount++] = Vertex2(points[1], color, jt->mTexSrc.right, jt->mTexSrc.top);
			currentMesh->mVerticies[currentMesh->mVertexCount++] = Vertex2(points[2], color, jt->mTexSrc.right, jt->mTexSrc.bottom);
			currentMesh->mVerticies[currentMesh->mVertexCount++] = Vertex2(points[3], color, jt->mTexSrc.left, jt->mTexSrc.bottom);

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
}

void Text::PrepareMesh(int charactersCount)
{
	int needPolygons = charactersCount*2;
	for (MeshesArr::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		needPolygons -= (*it)->maxPolyCount;

	if (needPolygons <= 0)
		return;

	if (needPolygons < 100 && mMeshes.size() > 0 &&
		needPolygons + mMeshes.back()->maxPolyCount < nMeshMaxPolyCount)
	{
		mMeshes.back()->resize(mMeshes.back()->maxVertexCount + (uint)needPolygons*2,
							   mMeshes.back()->maxPolyCount + (uint)needPolygons);
		return;
	}

	for (int i = 0; i < charactersCount/needPolygons + 1; i++)
		mMeshes.push_back(mnew Mesh(mFont->mTexture, nMeshMaxPolyCount*2, nMeshMaxPolyCount));
}

void Text::TransformMesh(const Basis& bas)
{
	mNeedTransformMesh = false;

	FOREACH(MeshesArr, mMeshes, it)
	{
		Mesh* mesh = *it;
		for (unsigned int i = 0; i < mesh->mVertexCount; i++)
		{
			Vertex2* vx = &mesh->mVerticies[i];
			bas.Transform(vx->x, vx->y);
		}
	}
}

void Text::PositionChanged()
{
	mTransformDef.mPosition = mPosition - mPivot;
	mNeedTransformMesh = true;
}

void Text::SizeChanged()
{
	mNeedUpdateMesh = true;
}

void Text::PivotChanged()
{
	mTransformDef.mPosition = mPosition - mPivot;
	mNeedTransformMesh = true;
}

void Text::ColorChanged()
{
	mNeedUpdateMesh = true;
}

CLOSE_O2_NAMESPACE