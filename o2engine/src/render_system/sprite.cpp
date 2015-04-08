#include "sprite.h"

#include "mesh.h"
#include "render_system.h"
#include "texture.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

Sprite::Sprite(TextureRef       texture        /*= NULL*/,
const RectF&    textureSrcRect /*= fRect(-1.0f, 0.0f, 0.0f, 0.0f)*/,
const Vec2F&    position       /*= vec2f()*/,
const Vec2F&    size           /*= vec2f(-1.0f, 0.0f)*/,
const Vec2F&    scale          /*= vec2f(1.0f, 1.0f)*/,
float           angle          /*= 0*/,
const Vec2F&    pivot          /*= vec2f()*/,
const Color4&   color          /*= color4::white()*/):
IRectDrawable(size, position, color, pivot)
{
	//create mesh
	mMesh = mnew Mesh(texture, 4, 2);

	mMesh->mIndexes[0] = 0; mMesh->mIndexes[1] = 1; mMesh->mIndexes[2] = 2;
	mMesh->mIndexes[3] = 0; mMesh->mIndexes[4] = 2; mMesh->mIndexes[5] = 3;

	mMesh->mVerticies[0].z = 1.0f; mMesh->mVerticies[1].z = 1.0f;
	mMesh->mVerticies[2].z = 1.0f; mMesh->mVerticies[3].z = 1.0f;

	mMesh->mVertexCount = 4;
	mMesh->mPolyCount = 2;

	if (textureSrcRect.left < 0)
		mTextureSrcRect = RectF(Vec2F(), texture.getSize());
	else
		mTextureSrcRect = textureSrcRect;

	if (size.x < 0)
		mSize = mTextureSrcRect.getSize();
	else
		mSize = size;

	for (int i = 0; i < 4; i++)
		mVertexColors[i] = Color4::White();

	mScale = scale;
	mAngle = angle;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
	mNeedUpdateMeshColors = true;

	InitializeProperties();
}

Sprite::Sprite(const Sprite& sprite):
IRectDrawable(sprite)
{
	//create mesh
	mMesh = mnew Mesh(*sprite.mMesh);

	mTextureSrcRect = sprite.mTextureSrcRect;
	mScale = sprite.mScale;
	mAngle = sprite.mAngle;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
	mNeedUpdateMeshColors = true;

	InitializeProperties();
}

Sprite::~Sprite()
{
	SafeRelease(mMesh);
}

IRectDrawable* Sprite::Clone() const
{
	return mnew Sprite(*this);
}

Sprite& Sprite::operator=(const Sprite& sprite)
{
	mMesh->SetTexture(sprite.GetTexture());
	mTextureSrcRect = sprite.mTextureSrcRect;
	mPosition = sprite.mPosition;
	mSize = sprite.mSize;
	mScale = sprite.mScale;
	mAngle = sprite.mAngle;
	mPivot = sprite.mPivot;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
	mNeedUpdateMeshColors = true;

	return *this;
}

void Sprite::SetScale(const Vec2F& scale)
{
	if (Equals(mScale, scale))
		return;

	mScale = scale;
	mNeedUpdateMeshVerticies = true;
}

Vec2F Sprite::GetScale() const
{
	return mScale;
}

void Sprite::SetAngle(const float& angle)
{
	if (Equals(angle, mAngle))
		return;

	mAngle = angle;
	mNeedUpdateMeshVerticies = true;
}

float Sprite::GetAngle() const
{
	return mAngle;
}

void Sprite::SetTextureSrcRect(const RectF& rect)
{
	if (Equals(mTextureSrcRect, rect))
		return;

	mTextureSrcRect = rect;
	mNeedUpdateMeshTexCoords = true;
}

RectF Sprite::GetTextureSrcRect() const
{
	return mTextureSrcRect;
}

void Sprite::SetTexture(const TextureRef& texture)
{
	mMesh->SetTexture(texture);
}

TextureRef Sprite::GetTexture() const
{
	return mMesh->GetTexture();
}

void Sprite::SetVertexColor(const Color4& color, int vertexId)
{
	mVertexColors[Clamp(vertexId, 0, 3)] = color;
	mNeedUpdateMeshColors = true;
}

Color4 Sprite::GetVertexColor(int vertexId) const
{
	return mVertexColors[Clamp(vertexId, 0, 3)];
}

void Sprite::PositionChanged()
{
	mNeedUpdateMeshVerticies = true;
}

void Sprite::SizeChanged()
{
	mNeedUpdateMeshVerticies = true;
}

void Sprite::PivotChanged()
{
	mNeedUpdateMeshVerticies = true;
}

void Sprite::ColorChanged()
{
	mNeedUpdateMeshColors = true;
}

void Sprite::Draw()
{
	if (!mEnabled)
		return;

	if (mNeedUpdateMeshVerticies)
		UpdateMeshVerticies();
	if (mNeedUpdateMeshTexCoords)
		UpdateMeshTexCoords();
	if (mNeedUpdateMeshColors)
		UpdateMeshColors();

	mMesh->Draw();
}

void Sprite::UpdateMeshVerticies()
{
	mNeedUpdateMeshVerticies = false;

	if (!Equals(mAngle, 0.0f))
	{
		float radAngle = Deg2rad(mAngle);

		float sn = sinf(radAngle);
		float cs = cosf(radAngle);

		Vec2F realSize = mSize.scale(mScale);

		Vec2F xvec(cs, sn);
		Vec2F yvec(-sn, cs);

		Vec2F offs = mPosition;
		Vec2F piv = mPivot.scale(mScale);

		if (!Equals(mPivot, Vec2F()))
			offs = offs - xvec*piv.x - yvec*piv.y;

		Vec2F xsize = xvec*realSize.x;
		Vec2F ysize = yvec*realSize.y;

		//vertex positions
		mMesh->mVerticies[0].x = offs.x;                     mMesh->mVerticies[0].y = offs.y;
		mMesh->mVerticies[1].x = offs.x + xsize.x;           mMesh->mVerticies[1].y = offs.y + xsize.y;
		mMesh->mVerticies[2].x = offs.x + xsize.x + ysize.x; mMesh->mVerticies[2].y = offs.y + xsize.y + ysize.y;
		mMesh->mVerticies[3].x = offs.x + ysize.x;           mMesh->mVerticies[3].y = offs.y + ysize.y;
	}
	else
	{
		Vec2F realSize = mSize.scale(mScale);
		Vec2F pos = mPosition - mPivot.scale(mScale);

		mMesh->mVerticies[0].x = pos.x;              mMesh->mVerticies[0].y = pos.y;
		mMesh->mVerticies[1].x = pos.x + realSize.x; mMesh->mVerticies[1].y = pos.y;
		mMesh->mVerticies[2].x = pos.x + realSize.x; mMesh->mVerticies[2].y = pos.y + realSize.y;
		mMesh->mVerticies[3].x = pos.x;              mMesh->mVerticies[3].y = pos.y + realSize.y;
	}
}

void Sprite::UpdateMeshTexCoords()
{
	mNeedUpdateMeshTexCoords = false;

	Vec2F texSize = mMesh->GetTexture().getSize();
	if (Equals(texSize, Vec2F()))
		return;

	Vec2F invTexSize(1.0f/texSize.x, 1.0f/texSize.y);
	RectF tex = mTextureSrcRect*invTexSize;

	mMesh->mVerticies[0].tu = tex.left;  mMesh->mVerticies[0].tv = tex.top;
	mMesh->mVerticies[1].tu = tex.right; mMesh->mVerticies[1].tv = tex.top;
	mMesh->mVerticies[2].tu = tex.right; mMesh->mVerticies[2].tv = tex.down;
	mMesh->mVerticies[3].tu = tex.left;  mMesh->mVerticies[3].tv = tex.down;
}

void Sprite::UpdateMeshColors()
{
	for (int i = 0; i < 4; i++)
		mMesh->mVerticies[i].color = (mVertexColors[i]*mColor).AsULong();

	mNeedUpdateMeshColors = false;
}

void Sprite::InitializeProperties()
{
	REG_PROPERTY(Sprite, scale, SetScale, GetScale);
	REG_PROPERTY(Sprite, angle, SetAngle, GetAngle);
	REG_PROPERTY(Sprite, textureSrcRect, SetTextureSrcRect, GetTextureSrcRect);
	REG_PROPERTY(Sprite, texture, SetTexture, GetTexture);
}

CLOSE_O2_NAMESPACE