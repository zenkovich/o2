#include "stretch_rect.h" 

#include "render_system/mesh.h"
#include "render_system/render_system.h"
#include "render_system/texture.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

StretchRect::Part::Part()
{
}

StretchRect::Part::Part(const Vec2F& LTPercent, const Vec2F& LTPixel, const Vec2F& RBPercent, const Vec2F& RBPixel,
						const RectF& texRect, bool wrapTexture /*= false*/,
						const Color4& vertex0Color /*= color4::white()*/,
						const Color4& vertex1Color /*= color4::white()*/,
						const Color4& vertex2Color /*= color4::white()*/,
						const Color4& vertex3Color /*= color4::white()*/):
						mLTPosPercent(LTPercent), mLTPosPixel(LTPixel), mRBPosPercent(RBPercent), mRBPosPixel(RBPixel), mTextureSrcRect(texRect),
						mWrapTexture(wrapTexture)
{
	mVertexColors[0] = vertex0Color; mVertexColors[1] = vertex1Color;
	mVertexColors[2] = vertex2Color; mVertexColors[3] = vertex3Color;

	const Vec2F idSize(100, 100);

	Vec2F lt = mLTPosPercent.Scale(idSize) + mLTPosPixel;
	Vec2F rb = mRBPosPercent.Scale(idSize) + mRBPosPixel;

	mClampLeft = lt.x > 0; mClampRight = rb.x < idSize.x;
	mClampTop = lt.y > 0;  mClampBottom = rb.y < idSize.y;
}


StretchRect::StretchRect(int parts /*= 0*/, const TextureRef& texture /*= grTexture()*/):
mMesh(NULL), mNeedUpdateMesh(true), IRectDrawable(), mNeedUpdateColors(false)
{
	CreateMesh(Max(parts, 1), texture);
	mMinSize = Vec2F(0, 0);
}

StretchRect::StretchRect(const StretchRect& stretchRect):
IRectDrawable(stretchRect)
{
	mMesh = mnew Mesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mMinSize = stretchRect.mMinSize;

	mNeedUpdateMesh = true;
	mNeedUpdateColors = false;
}

StretchRect::StretchRect(const TextureRef& texture, int left, int top, int right, int bottom,
						 const RectF& texRect /*= fRect()*/, const Color4& color /*= color4::white()*/):
						 mNeedUpdateMesh(false), IRectDrawable(), mNeedUpdateColors(false), mMesh(NULL)
{
	CreateMesh(9, texture);

	RectF rt = texRect;
	if (rt.IsZero())
		rt = RectF(Vec2F(), texture.GetSize());

	float _left = (float)left;
	float _top = (float)top;
	float _right = (float)right;
	float _bottom = (float)bottom;

	mSize = rt.Size();
	mColor = color;
	mMinSize = Vec2F((float)(left + right), (float)(top + bottom));

	Color4 wc = Color4::White();

	// left top
	AddPart(Vec2F(0, 0), Vec2F(0, 0), Vec2F(0, 0), Vec2F(_left, _top),
			RectF(rt.left, rt.top, rt.left + _left, rt.top + _top));

	// top
	AddPart(Vec2F(0, 0), Vec2F(_left, 0), Vec2F(1, 0), Vec2F(-_right, _top),
			RectF(rt.left + _left, rt.top, rt.right - _right, rt.top + _top));

	//right top
	AddPart(Vec2F(1, 0), Vec2F(-_right, 0), Vec2F(1, 0), Vec2F(0, _top),
			RectF(rt.right - _right, rt.top, rt.right, rt.top + _top));


	//left
	AddPart(Vec2F(0, 0), Vec2F(0, _top), Vec2F(0, 1), Vec2F(_left, -_bottom),
			RectF(rt.left, rt.top + _top, rt.left + _left, rt.bottom - _bottom));

	//center
	AddPart(Vec2F(0, 0), Vec2F(_left, _top), Vec2F(1, 1), Vec2F(-_right, -_bottom),
			RectF(rt.left + _left, rt.top + _top, rt.right - _right, rt.bottom - _bottom));

	//right
	AddPart(Vec2F(1, 0), Vec2F(-_right, _top), Vec2F(1, 1), Vec2F(0, -_bottom),
			RectF(rt.right - _right, rt.top + _top, rt.right, rt.bottom - _bottom));


	//left bottom
	AddPart(Vec2F(0, 1), Vec2F(0, -_bottom), Vec2F(0, 1), Vec2F(_left, 0),
			RectF(rt.left, rt.bottom - _bottom, rt.left + _left, rt.bottom));

	//bottom
	AddPart(Vec2F(0, 1), Vec2F(_left, -_bottom), Vec2F(1, 1), Vec2F(-_right, 0),
			RectF(rt.left + _left, rt.bottom - _bottom, rt.right - _right, rt.bottom));

	//right bottom
	AddPart(Vec2F(1, 1), Vec2F(-_right, -_bottom), Vec2F(1, 1), Vec2F(0, 0),
			RectF(rt.right - _right, rt.bottom - _bottom, rt.right, rt.bottom));

	UpdateMesh();
}

StretchRect& StretchRect::operator=(const StretchRect& stretchRect)
{
	SafeRelease(mMesh);
	mMesh = mnew Mesh(*stretchRect.mMesh);

	mParts = stretchRect.mParts;
	mPosition = stretchRect.mSize;
	mSize = stretchRect.mSize;
	mPivot = stretchRect.mPivot;
	mColor = stretchRect.mColor;

	mNeedUpdateMesh = true;
	mNeedUpdateColors = false;

	return *this;
}

IRectDrawable* StretchRect::Clone() const
{
	return mnew StretchRect(*this);
}

void StretchRect::CreateMesh(int partsCount, const TextureRef& texture)
{
	mMesh = mnew Mesh(texture, partsCount*4, partsCount*2);
	InitMeshPolygons();
}

void StretchRect::InitMeshPolygons(int startIdx /*= 0*/)
{
	int partsCount = mMesh->GetMaxPolyCount()/2;
	for (int i = startIdx; i < partsCount; i++)
	{
		mMesh->mIndexes[i*2*3]     = i*4;
		mMesh->mIndexes[i*2*3 + 1] = i*4 + 1;
		mMesh->mIndexes[i*2*3 + 2] = i*4 + 2;
		mMesh->mIndexes[i*2*3 + 3] = i*4;
		mMesh->mIndexes[i*2*3 + 4] = i*4 + 2;
		mMesh->mIndexes[i*2*3 + 5] = i*4 + 3;
	}
}

int StretchRect::AddPart(const Vec2F& LTPercent, const Vec2F& LTPixel, const Vec2F& RBPercent, const Vec2F& RBPixel,
						 const RectF& texRect, bool wrapTexture /*= false*/,
						 const Color4& vertex0Color /*= color4::white()*/,
						 const Color4& vertex1Color /*= color4::white()*/,
						 const Color4& vertex2Color /*= color4::white()*/,
						 const Color4& vertex3Color /*= color4::white()*/)
{
	mParts.Add(Part(LTPercent, LTPixel, RBPercent, RBPixel, texRect, wrapTexture,
		vertex0Color, vertex1Color, vertex2Color, vertex3Color));

	int partsCount = mParts.Size();

	if (mMesh->GetMaxPolyCount() < mParts.Size()*2)
	{
		mMesh->Resize(partsCount*4, partsCount*2);
		InitMeshPolygons(partsCount - 1);
	}

	mNeedUpdateMesh = true;

	return partsCount - 1;
}

void StretchRect::RemovePart(int idx)
{
	if (idx < 0 || idx >(int)mParts.Size() - 1)
		return;

	mParts.RemoveAt(idx);

	mNeedUpdateMesh = true;
}

void StretchRect::SetMinSize(const Vec2F& minSize)
{
	mMinSize = minSize;
	mNeedUpdateMesh = true;
}

Vec2F StretchRect::GetMinSize() const
{
	return mMinSize;
}

void StretchRect::PositionChanged()
{
	mNeedUpdateMesh = true;
}

void StretchRect::SizeChanged()
{
	mNeedUpdateMesh = true;
}

void StretchRect::PivotChanged()
{
	mNeedUpdateMesh = true;
}

void StretchRect::ColorChanged()
{
	mNeedUpdateColors = true;
}

inline void cStretchRectClamp(float& minSide, float& maxSide, bool clampMin, bool clampMax, float clampCoef, float& minTex, float& maxTex)
{
	float size = maxSide - minSide;
	if (size < 0)
	{
		maxSide = minSide = 0;
		return;
	}

	float leftCoef = (float)clampMin;
	float rightCoef = (float)clampMax;
	float diff = size - size*clampCoef;
	float coefSumm = leftCoef + rightCoef;
	float invCoefSumm = 1.0f/coefSumm;
	float minDiff = leftCoef*invCoefSumm*diff;
	float maxDiff = rightCoef*invCoefSumm*diff;
	minSide += minDiff; minTex += minDiff;
	maxSide -= maxDiff; maxTex -= maxDiff;
}

void StretchRect::UpdateMesh()
{
	if (!mMesh)
		return;

	Vec2F texSize = mMesh->GetTexture().GetSize();
	Vec2F invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	Vec2F pos = mPosition - mPivot;
	bool clampX = mSize.x < mMinSize.x;
	bool clampY = mSize.y < mMinSize.y;
	Vec2F clampCoef = mSize.InvScale(mMinSize);
	int i = 0;
	for (PartsArr::Iterator it = mParts.Begin(); it != mParts.End(); ++it, i++)
	{
		Vec2F loc_ltPoint = mSize.Scale(it->mLTPosPercent) + it->mLTPosPixel;
		Vec2F loc_rbPoint = mSize.Scale(it->mRBPosPercent) + it->mRBPosPixel;
		RectF texRect = it->mTextureSrcRect;

		if (clampX)
			cStretchRectClamp(loc_ltPoint.x, loc_rbPoint.x, it->mClampLeft, it->mClampRight, clampCoef.x,
			texRect.left, texRect.right);

		if (clampY)
			cStretchRectClamp(loc_ltPoint.y, loc_rbPoint.y, it->mClampTop, it->mClampBottom, clampCoef.y,
			texRect.top, texRect.bottom);

		Vec2F ltPoint = pos + loc_ltPoint;
		Vec2F rbPoint = pos + loc_rbPoint;
		RectF tex = texRect*invTexSize;

		if (it->mWrapTexture)
		{
			tex.right *= (rbPoint.x - ltPoint.x)/it->mTextureSrcRect.Width();
			tex.bottom *= (rbPoint.y - ltPoint.y)/it->mTextureSrcRect.Height();
		}

		mMesh->mVerticies[i*4].set(ltPoint, 1.0f, (it->mVertexColors[0]*mColor).AsULong(), tex.left, tex.top);
		mMesh->mVerticies[i*4 + 1].set(Vec2F(rbPoint.x, ltPoint.y), 1.0f, (it->mVertexColors[1]*mColor).AsULong(), tex.right, tex.top);
		mMesh->mVerticies[i*4 + 2].set(rbPoint, 1.0f, (it->mVertexColors[2]*mColor).AsULong(), tex.right, tex.bottom);
		mMesh->mVerticies[i*4 + 3].set(Vec2F(ltPoint.x, rbPoint.y), 1.0f, (it->mVertexColors[3]*mColor).AsULong(), tex.left, tex.bottom);
	}

	mMesh->mVertexCount = mParts.Size()*4;
	mMesh->mPolyCount = mParts.Size()*2;

	mNeedUpdateMesh = false;
	mNeedUpdateColors = false;
}

void StretchRect::UpdateColors()
{
	int i = 0;
	for (PartsArr::Iterator it = mParts.Begin(); it != mParts.End(); ++it, i++)
	{
		for (int j = 0; j < 4; j++)
			mMesh->mVerticies[i*4 + j].color = (it->mVertexColors[j]*mColor).AsULong();
	}

	mNeedUpdateColors = false;
}

void StretchRect::DrawDebug()
{
	Vec2F pos = mPosition - mPivot;
	int clr = 1;
	bool clampX = mSize.x < mMinSize.x;
	bool clampY = mSize.y < mMinSize.y;
	Vec2F clampCoef = mSize.InvScale(mMinSize);
	int i = 0;
	for (auto it : mParts)
	{
		i++;
		Vec2F loc_ltPoint = mSize.scale(it->mLTPosPercent) + it->mLTPosPixel;
		Vec2F loc_rbPoint = mSize.scale(it->mRBPosPercent) + it->mRBPosPixel;
		RectF texRect = it->mTextureSrcRect;

		if (clampX)
			cStretchRectClamp(loc_ltPoint.x, loc_rbPoint.x, it->mClampLeft, it->mClampRight, clampCoef.x,
			texRect.left, texRect.right);

		if (clampY)
			cStretchRectClamp(loc_ltPoint.y, loc_rbPoint.y, it->mClampTop, it->mClampBottom, clampCoef.y,
			texRect.top, texRect.bottom);

		Vec2F ltPoint = pos + loc_ltPoint + Vec2F(1, 1);
		Vec2F rbPoint = pos + loc_rbPoint - Vec2F(1, 1);

		AppRender()->DrawRectFrame(ltPoint, rbPoint, Color4::SomeColor(clr++));
	}
}

void StretchRect::Draw()
{
	if (!mEnabled)
		return;

	if (mNeedUpdateMesh)
		UpdateMesh();

	if (mNeedUpdateColors)
		UpdateColors();

	if (mMesh)
		mMesh->Draw();
}

CLOSE_O2_NAMESPACE