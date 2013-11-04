#ifndef MESH_H
#define MESH_H

#include "public.h"

#include "util/math/vertex.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class grTexture;
class grRenderSystem;

/** Triangles mesh. Containing verticies, indeces of polygons, texture. */
class grMesh
{
	friend class grRenderSystem;
	friend class grSprite;

public:
	vertex2*        mVerticies;      /**< Vertex buffer. */
	unsigned short* mIndexes;        /**< Index buffer. */

	unsigned int    mVertexCount;    /**< Current verticies count. */
	unsigned int    mPolyCount;      /**< Current polygons in mesh. */

protected:
	grRenderSystem* mRenderSystem;   /**< Render system ptr. */

	grTexture*      mTexture;        /**< Texture ptr. */

	unsigned int    mMaxVertexCount; /**< Max size of vertex buffer. */
	unsigned int    mMaxPolyCount;   /**< Max polygons count, mMaxPolyCount*3 - os index buffer max size. */

public:
	/** ctor. */
	grMesh(grRenderSystem* renderSystem, grTexture* texture = NULL, unsigned int vertexCount = 4, 
		   unsigned int polyCount = 2);

	/** copy ctor. */
	grMesh(const grMesh& mesh);

	/** dtor. */
	~grMesh();

	/** Resising mesh buffers, losts data. */
	void resize(unsigned int vertexCount, unsigned int polyCount);

	/** Drawing mesh. */
	void draw();

	/** Sets texture, NULL - no texture. */
	void setTexture(grTexture* texture);

	/** Returns texture ptr. */
	grTexture* getTexture() const;

	/** Returns max vertex buffer size. */
	unsigned int getMaxVertexCount() const;

	/** Returns max polygons count. */
	unsigned int getMaxPolyCount() const;
};

CLOSE_O2_NAMESPACE

#endif //MESH_H