#include "sprite.h"

#include "render_system.h"
#include "texture.h"
#include "mesh.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

grSprite::grSprite( grRenderSystem* render, 
	                grTexture*      texture        /*= NULL*/, 
	                const fRect&    textureSrcRect /*= fRect(-1.0f, 0.0f, 0.0f, 0.0f)*/, 
	                const vec2f&    position       /*= vec2f(0.0f, 0.0f)*/, 
					const vec2f&    size           /*= vec2f(-1.0f, 0.0f)*/, 
					const vec2f&    scale          /*= vec2f(1.0f, 1.0f)*/, 
					float           angle          /*= 0*/, 
					const vec2f&    rotationCenter /*= vec2f(0.0f, 0.0f)*/, 
					const color4&   color          /*= color4(1.0f, 1.0f, 1.0f, 1.0f)*/ )
{
	//create mesh
	mMesh = new grMesh(render, texture, 4, 2);

	mMesh->mIndexes[0] = 0; mMesh->mIndexes[1] = 1; mMesh->mIndexes[2] = 2;
	mMesh->mIndexes[3] = 0; mMesh->mIndexes[4] = 2; mMesh->mIndexes[5] = 3;

	mMesh->mVerticies[0].z = 1.0f; mMesh->mVerticies[1].z = 1.0f; 
	mMesh->mVerticies[2].z = 1.0f; mMesh->mVerticies[3].z = 1.0f;

	mMesh->mVertexCount = 4;
	mMesh->mPolyCount = 2;
	
	if (textureSrcRect.left < 0)
	{
		vec2f textureSize(0.0f, 0.0f);

		if (texture)
			textureSize = texture->getSize();

		mTextureSrcRect = fRect(vec2f(0.0f, 0.0f), textureSize);
	}
	else 
	{
		mTextureSrcRect = textureSrcRect;
	}

	if (size.x < 0)
	{
		mSize = mTextureSrcRect.getSize();
	}
	else
	{
		mSize = size;
	}

	mPosition = position;
	mScale = scale;
	mAngle = angle;
	mRotationCenter = rotationCenter;

	setColor(color);

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
}

grSprite::grSprite( const grSprite& sprite )
{
	//create mesh
	mMesh = new grMesh(*sprite.mMesh);

	mTextureSrcRect = sprite.mTextureSrcRect;
	mPosition = sprite.mPosition;
	mSize = sprite.mSize;
	mScale = sprite.mScale;
	mAngle = sprite.mAngle;
	mRotationCenter = sprite.mRotationCenter;

	mNeedUpdateMeshVerticies = true;
	mNeedUpdateMeshTexCoords = true;
}

/*grSprite::grSprite( grRender* render, cDataObject& dataObject )
{
	//create mesh
	mMesh = new grRender2DObjectMesh(render, 4, 2);
	mMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mMesh->mVertexBuffer[0].z = 1.0f; mMesh->mVertexBuffer[1].z = 1.0f; 
	mMesh->mVertexBuffer[2].z = 1.0f; mMesh->mVertexBuffer[3].z = 1.0f;

	serialize_(dataObject, AT_INPUT);
}*/

grSprite::grSprite(grRenderSystem* render, pugi::xml_node& xmlNode)
{
	//create mesh
	mMesh = new grMesh(render, NULL, 4, 2);

	mMesh->mIndexes[0] = 0; mMesh->mIndexes[1] = 1; mMesh->mIndexes[2] = 2;
	mMesh->mIndexes[3] = 0; mMesh->mIndexes[4] = 2; mMesh->mIndexes[5] = 3;

	mMesh->mVerticies[0].z = 1.0f; mMesh->mVerticies[1].z = 1.0f; 
	mMesh->mVerticies[2].z = 1.0f; mMesh->mVerticies[3].z = 1.0f;

	mMesh->mVertexCount = 4;
	mMesh->mPolyCount = 2;

	serialize(xmlNode, cSerializeType::INPUT);

	/*cDataObject* spriteDataObject = 
		getDataObjectsManager().loadDataObject(file, cDataObjectsManager::DOT_XML)->getChild(path);

	serialize_(*spriteDataObject, AT_INPUT);*/
}

grSprite::~grSprite()
{
	safe_release(mMesh);
}

grSprite& grSprite::setPosition( const vec2f& position )
{
	if (!(fabs(position.x - mPosition.x) > FLT_EPSILON || fabs(position.y - mPosition.y) > FLT_EPSILON))
		return *this;

	mPosition = position;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f grSprite::getPosition() const
{
	return mPosition;
}

grSprite& grSprite::setScale( const vec2f& scale )
{
	if (!(fabs(scale.x - mScale.x) > FLT_EPSILON || fabs(scale.y - mScale.y) > FLT_EPSILON))
		return *this;

	mScale = scale;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f grSprite::getScale() const
{
	return mScale;
}

grSprite& grSprite::setAngle( float angle )
{
	if (fabs(angle - mAngle) < FLT_EPSILON)
		return *this;

	mAngle = angle;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

float grSprite::getAngle() const
{
	return mAngle;
}

grSprite& grSprite::setRotationCenter( const vec2f& center )
{
	if (!(fabs(center.x - mRotationCenter.x) > FLT_EPSILON || fabs(center.y - mRotationCenter.y) > FLT_EPSILON))
		return *this;

	mRotationCenter = center;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f grSprite::getRotationCenter() const
{
	return mRotationCenter;
}

grSprite& grSprite::setTextureSrcRect( const fRect& rect )
{
	mTextureSrcRect = rect;
	mNeedUpdateMeshTexCoords = true;
	return *this;
}

const fRect& grSprite::getTextureSrcRect() const
{
	return mTextureSrcRect;
}

grSprite* grSprite::clone() const
{
	return new grSprite(*this);
}

grSprite& grSprite::setTexture( grTexture* texture )
{
	mMesh->setTexture(texture);
	return *this;
}

grTexture* grSprite::getTexture() const
{
	return mMesh->getTexture();
}

grSprite& grSprite::setSize( const vec2f& size )
{
	if (!(fabs(size.x - mSize.x) > FLT_EPSILON || fabs(size.y - mSize.y) > FLT_EPSILON))
		return *this;

	mSize = size;
	mNeedUpdateMeshVerticies = true;
	return *this;
}

vec2f grSprite::getSize() const
{
	return mSize;
}

grSprite& grSprite::setColor( const color4& color, int vertexId /*= -1*/ )
{
	unsigned long ccolor = color.dword();

	if (vertexId < 0)
	{
		for (unsigned int i = 0; i < 4; i++)
			mMesh->mVerticies[i].color = ccolor;
	}
	else
	{
		mMesh->mVerticies[min(vertexId, 3)].color = ccolor;
	}

	return *this;
}

color4 grSprite::getColor( int vertexId /*= 0*/ )
{
	color4 res;
	res.setDword(mMesh->mVerticies[max(min(vertexId, 4), 0)].color);
	return res;
}

void grSprite::draw()
{
	if (mNeedUpdateMeshVerticies) 
		updateMeshVerticies();
	if (mNeedUpdateMeshTexCoords)
		updateMeshTexCoords();

	mMesh->draw();
}

void grSprite::updateMeshVerticies()
{
	mNeedUpdateMeshVerticies = false;

	if (mAngle > FLT_EPSILON || mAngle < -FLT_EPSILON)
	{
		float radAngle = deg2rad(mAngle);

		float sn = sinf(radAngle);
		float cs = cosf(radAngle);

		vec2f realSize = mSize.scale(mScale);

		vec2f xvec(cs,  sn);
		vec2f yvec(-sn, cs);

		vec2f offs = mPosition;

		if (fabs(mRotationCenter.x) > FLT_EPSILON || fabs(mRotationCenter.y) > FLT_EPSILON)
			offs = offs + xvec*(-mRotationCenter.x) - yvec*mRotationCenter.y;

		vec2f xsize = xvec*realSize.x;
		vec2f ysize = yvec*realSize.y;

		//vertex positions
		mMesh->mVerticies[0].x = offs.x;                     mMesh->mVerticies[0].y = offs.y;
		mMesh->mVerticies[1].x = offs.x + xsize.x;           mMesh->mVerticies[1].y = offs.y + xsize.y;
		mMesh->mVerticies[2].x = offs.x + xsize.x + ysize.x; mMesh->mVerticies[2].y = offs.y + xsize.y + ysize.y;
		mMesh->mVerticies[3].x = offs.x + ysize.x;           mMesh->mVerticies[3].y = offs.y + ysize.y;
	}
	else
	{
		vec2f realSize = mSize.scale(mScale);

		mMesh->mVerticies[0].x = mPosition.x;              mMesh->mVerticies[0].y = mPosition.y;
		mMesh->mVerticies[1].x = mPosition.x + realSize.x; mMesh->mVerticies[1].y = mPosition.y;
		mMesh->mVerticies[2].x = mPosition.x + realSize.x; mMesh->mVerticies[2].y = mPosition.y + realSize.y;
		mMesh->mVerticies[3].x = mPosition.x;              mMesh->mVerticies[3].y = mPosition.y + realSize.y;
	}
}

void grSprite::updateMeshTexCoords()
{
	mNeedUpdateMeshTexCoords = false;

	if (!mMesh->getTexture()) return;

	vec2f texSize = mMesh->getTexture()->getSize();
	vec2f invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

	mMesh->mVerticies[0].tu = mTextureSrcRect.left*invTexSize.x;
	mMesh->mVerticies[0].tv = mTextureSrcRect.top*invTexSize.y;

	mMesh->mVerticies[1].tu = mTextureSrcRect.right*invTexSize.x;
	mMesh->mVerticies[1].tv = mTextureSrcRect.top*invTexSize.y;

	mMesh->mVerticies[2].tu = mTextureSrcRect.right*invTexSize.x;
	mMesh->mVerticies[2].tv = mTextureSrcRect.down*invTexSize.y;

	mMesh->mVerticies[3].tu = mTextureSrcRect.left*invTexSize.x;
	mMesh->mVerticies[3].tv = mTextureSrcRect.down*invTexSize.y;
}

SERIALIZE_METHOD_IMPL(grSprite)
{
	if (!SERIALIZE_ID(mPosition, "position"))
		mPosition = vec2f(0, 0);

	if (!SERIALIZE_ID(mScale, "scale"))
		mScale = vec2f(1, 1);

	if (!SERIALIZE_ID(mAngle, "angle"))
		mAngle = 0;

	if (!SERIALIZE_ID(mRotationCenter, "rotationCenter"))
		mRotationCenter = vec2f(0, 0);

	if (type == cSerializeType::OUTPUT) 
	{
		if (mMesh->mTexture)
		{
			std::string texFilename = mMesh->mTexture->getFileName();
			SERIALIZE_ID(texFilename, "texture");
			SERIALIZE_ID(mTextureSrcRect, "textureSrcRect");
		}
	}
	else
	{
		mMesh->setTexture(NULL);

		std::string textureName;
		if (SERIALIZE_ID(textureName, "texture"))
		{
			grTexture* texture = mMesh->mRenderSystem->createTexture(textureName);
			mMesh->setTexture(texture);
			texture->decrRefCount();

			if (!SERIALIZE_ID(mTextureSrcRect, "textureSrcRect"))
				mTextureSrcRect = fRect(vec2f(0, 0), texture->getSize());
		}
	}

	if (!SERIALIZE_ID(mSize, "size"))
		mSize = mTextureSrcRect.getSize();

	if (type == cSerializeType::INPUT)
	{
		color4 vertColors[4];

		if (!SERIALIZE_ARR_ID(vertColors, 4, "vertColors"))
		{
			color4 spriteColor;
			if (SERIALIZE_ID(spriteColor, "color"))
			{
				setColor(spriteColor);
			}
			else 
				setColor(color4(255, 255, 255, 255));
		}
		else
		{
			for (unsigned int i = 0; i < 4; i++)
				mMesh->mVerticies[i].color = vertColors->dword();
		}
	}
	else
	{
		color4 vertColors[4];

		bool different = false;
		for (unsigned int i = 0; i < 4; i++)
		{
			vertColors[i].setDword( mMesh->mVerticies[i].color );

			if (i > 0)
			{
				if (abs(vertColors[i].r - vertColors[i - 1].r) > 1 ||
					abs(vertColors[i].g - vertColors[i - 1].g) > 1 ||
					abs(vertColors[i].b - vertColors[i - 1].b) > 1 ||
					abs(vertColors[i].a - vertColors[i - 1].a) > 1)
				{
					different = true;
				}
			}
		}

		if (different)
			SERIALIZE_ARR_ID(vertColors, 4, "vertColors");
		else
			SERIALIZE_ID(vertColors[0], "color");
	}
	
	return true;
}

CLOSE_O2_NAMESPACE