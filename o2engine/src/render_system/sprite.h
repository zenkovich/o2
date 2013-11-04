#ifndef SPRITE_H
#define SPRITE_H

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/rect.h"
#include "util/math/color.h"
#include "util/serialization/serialization.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;
class grTexture;
	
/** Sprite, just a quad with texture. */
class grSprite
{
	vec2f   mPosition;
	vec2f   mSize;
	vec2f   mScale;
	float   mAngle;
	vec2f   mRotationCenter;
	fRect   mTextureSrcRect;

	grMesh* mMesh;

	bool    mNeedUpdateMeshVerticies;
	bool    mNeedUpdateMeshTexCoords;

public:
	/** ctor.
	 *  @param texture        - texture ptr, use NULL if no texture 
	 *  @param textureSrcRect - texture source rect, if left is < 0 - use all texture size
	 *  @param position       - position of left top corner of sprite
	 *  @param size           - size of sprite, in pixels. If x < 0 - size as texture source rect
	 *  @param scale          - scale of the sprite
	 *  @param angle          - rotation of sprite, in degrees
	 *  @param rotationCenter - center of sprite rotation
	 *  @param color          - color of the sprite. */
	grSprite(grRenderSystem* render, grTexture* texture = NULL, const fRect& textureSrcRect = fRect(-1.0f, 0.0f, 0.0f, 0.0f), 
		     const vec2f& position = vec2f(0.0f, 0.0f), const vec2f& size = vec2f(-1.0f, 0.0f), 
			 const vec2f& scale = vec2f(1.0f, 1.0f), float angle = 0, const vec2f& rotationCenter = vec2f(0.0f, 0.0f),
			 const color4& color = color4(1.0f, 1.0f, 1.0f, 1.0f));

	//grSprite(grRenderSystem* render, cDataObject& dataObject);
	
	/** ctor. Loads sprite from config file. */
	grSprite(grRenderSystem* render, pugi::xml_node& xmlNode);

	/** copy ctor. */
	grSprite(const grSprite& sprite);

	/** dtor. */
	~grSprite();

	/** Returns cloned sprite. */
	grSprite* clone() const;

	/** Drawing that sprite. */
	void draw();

	/** Sets position of sprite. */
	grSprite& setPosition(const vec2f& position);

	/** Returns position of sprite. */
	vec2f getPosition() const;

	/** Sets scale of sprite. */
	grSprite& setScale(const vec2f& scale);

	/** Returns scale of sprite. */
	vec2f getScale() const;

	/** Sets angle of sprite. */
	grSprite& setAngle(float angle);

	/** Returns angle of sprite. */
	float getAngle() const;

	/** Sets rotation center of sprite. */
	grSprite& setRotationCenter(const vec2f& center);

	/** Returns rotation center of sprite. */
	vec2f getRotationCenter() const;

	/** Sets texture source rect of sprite. */
	grSprite& setTextureSrcRect(const fRect& rect);

	/** Returns texture source rect of sprite. */
	const fRect& getTextureSrcRect() const;

	/** Sets texture of sprite. */
	grSprite& setTexture(grTexture* texture);

	/** Returns texture of sprite. */
	grTexture* getTexture() const;

	/** Sets size of sprite. */
	grSprite& setSize(const vec2f& size);

	/** Returns size of sprite. */
	vec2f getSize() const;

	/** Sets the color of vertex sprite. If vertex id < 0 - setting color for all sprite. */
	grSprite& setColor(const color4& color, int vertexId = -1);

	/** Returns color of the sprite vertex. */
	color4 getColor(int vertexId = 0);

	SERIALIZE_METHOD_DECL();

protected:
	/** Updating mesh verticies positions. */
	void updateMeshVerticies();

	/** Updating mesh verticies texture coords. */
	void updateMeshTexCoords();
};

CLOSE_O2_NAMESPACE

#endif //SPRITE_H