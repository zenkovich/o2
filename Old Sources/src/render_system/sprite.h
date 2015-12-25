#pragma once

#include "public.h"
#include "texture.h"
#include "util/graphics/rect_drawable.h"
#include "util/math/color.h"
#include "util/math/rect.h"
#include "util/math/vector2.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class Mesh;
class RenderSystem;

/** Sprite, just a quad with texture. */
class Sprite: public IRectDrawable
{
	Vec2F   mScale;           /** Scale of sprite. Real sprite size is mSize*mScale. */
	float   mAngle;           /** Rotation angle. */
	RectF   mTextureSrcRect;  /** texture src rect. */
	Color4  mVertexColors[4]; /** Vertex colors. */

	Mesh*   mMesh;                    /** Mesh. */

	bool    mNeedUpdateMeshVerticies; /** True, when mesh vertex positions needs to update. */
	bool    mNeedUpdateMeshTexCoords; /** True, when mesh vertex tex coords needs to update. */
	bool    mNeedUpdateMeshColors;    /** True, when mesh vertex colors needs to update. */

public:
	//properties
	PROPERTY(Sprite, Vec2F)      scale;          /** Scale property. Uses set/getScale. */
	PROPERTY(Sprite, float)      angle;          /** Angle property. Uses set/getAngle. */
	PROPERTY(Sprite, RectF)      textureSrcRect; /** Texture src rect property. Uses set/getTextureSrcRect. */
	PROPERTY(Sprite, TextureRef) texture;        /** Texture property. uses set/getTexture. */

	/** ctor.
	 *  @param texture        - texture ptr, use NULL if no texture
	 *  @param textureSrcRect - texture source rect, if left is < 0 - use all texture size
	 *  @param position       - position of left top corner of sprite
	 *  @param size           - size of sprite, in pixels. If x < 0 - size as texture source rect
	 *  @param scale          - scale of the sprite
	 *  @param angle          - rotation of sprite, in degrees
	 *  @param rotationCenter - center of sprite rotation
	 *  @param color          - color of the sprite. */
	Sprite(TextureRef texture = TextureRef(), const RectF& textureSrcRect = RectF(-1.0f, 0.0f, 0.0f, 0.0f),
		   const Vec2F& position = Vec2F(), const Vec2F& size = Vec2F(-1.0f, 0.0f),
		   const Vec2F& scale = Vec2F(1.0f, 1.0f), float angle = 0, const Vec2F& pivot = Vec2F(),
		   const Color4& color = Color4::White());

	/** copy ctor. */
	Sprite(const Sprite& sprite);

	/** dtor. */
	~Sprite();

	Sprite& operator=(const Sprite& sprite);

	/** Returns cloned sprite. */
	IRectDrawable* Clone() const;

	/** Drawing that sprite. */
	void Draw();

	/** Sets scale of sprite. */
	void SetScale(const Vec2F& scale);

	/** Returns scale of sprite. */
	Vec2F GetScale() const;

	/** Sets angle of sprite. */
	void SetAngle(const float& angle);

	/** Returns angle of sprite. */
	float GetAngle() const;

	/** Sets texture source rect of sprite. */
	void SetTextureSrcRect(const RectF& rect);

	/** Returns texture source rect of sprite. */
	RectF GetTextureSrcRect() const;

	/** Sets texture of sprite. */
	void SetTexture(const TextureRef& texture);

	/** Returns texture of sprite. */
	TextureRef GetTexture() const;

	/** Sets the color of vertex sprite. */
	void SetVertexColor(const Color4& color, int vertexId);

	/** Returns color of the sprite vertex. */
	Color4 GetVertexColor(int vertexId) const;

protected:
	void PositionChanged();
	void SizeChanged();
	void PivotChanged();
	void ColorChanged();

	/** Initializing properties. */
	void InitializeProperties();

	/** Updating mesh verticies positions. */
	void UpdateMeshVerticies();

	/** Updating mesh verticies texture coords. */
	void UpdateMeshTexCoords();

	/** Updating mesh verticies colors. */
	void UpdateMeshColors();
};

CLOSE_O2_NAMESPACE