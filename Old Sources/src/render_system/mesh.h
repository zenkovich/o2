#pragma once

#include "public.h"
#include "texture.h"
#include "util/objects.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class RenderSystem;

/** Triangles mesh. Containing verticies, indeces of polygons, texture. */
class Mesh: public virtual IDrawable
{
	friend class RenderSystem;
	friend class Sprite;

public:
	Vertex2*   mVerticies;      /**< Vertex buffer. */
	uint16*    mIndexes;        /**< Index buffer. */

	uint       mVertexCount;    /**< Current verticies count. */
	uint       mPolyCount;      /**< Current polygons in mesh. */

protected:
	TextureRef mTexture;        /**< Texture. */

	uint       mMaxVertexCount; /**< Max size of vertex buffer. */
	uint       mMaxPolyCount;   /**< Max polygons count, mMaxPolyCount*3 - os index buffer max size. */

public:
	//properties
	PROPERTY(Mesh, TextureRef) texture;        /** Texture property, uses set/getTexture. */
	PROPERTY(Mesh, uint)       maxVertexCount; /** Max vertex count property, uses set/getMaxVertexCount. */
	PROPERTY(Mesh, uint)       maxPolyCount;   /** Max polygons count property, uses set/getMaxPolyCount. */

	/** ctor. */
	Mesh(TextureRef texture = TextureRef(), uint vertexCount = 4, uint polyCount = 2);

	/** copy ctor. */
	Mesh(const Mesh& mesh);

	/** dtor. */
	~Mesh();

	/** Resizing mesh buffers, losts data. */
	void Resize(uint vertexCount, uint polyCount);

	/** Drawing mesh. */
	void Draw();

	/** Sets texture. */
	void SetTexture(const TextureRef& texture);

	/** Returns texture ptr. */
	TextureRef GetTexture() const;

	/** Sets max vertex count buffer. */
	void SetMaxVertexCount(const uint& count);

	/** Sets max polygons count buffer. */
	void SetMaxPolyCount(const uint& count);

	/** Returns max vertex buffer size. */
	uint GetMaxVertexCount() const;

	/** Returns max polygons count. */
	uint GetMaxPolyCount() const;

protected:
	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE