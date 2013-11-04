#include "mesh.h"

#include "render_system.h"
#include "texture.h"

OPEN_O2_NAMESPACE

grMesh::grMesh( grRenderSystem* renderSystem, grTexture* texture /*= NULL*/, unsigned int vertexCount /*= 4*/, 
                unsigned int polyCount /*= 2*/ ):
	mRenderSystem(renderSystem), mTexture(NULL), mVerticies(NULL), mIndexes(NULL), mMaxPolyCount(0), mMaxVertexCount(0),
	mVertexCount(0), mPolyCount(0)
{
	setTexture(texture);
	resize(vertexCount, polyCount);
}

grMesh::grMesh( const grMesh& mesh ):
	mTexture(NULL), mVerticies(NULL), mIndexes(NULL), mMaxVertexCount(0), mMaxPolyCount(0)
{
	mRenderSystem = mesh.mRenderSystem;
	setTexture(mesh.mTexture);

	resize(mesh.mMaxVertexCount, mesh.mMaxPolyCount);

	mVertexCount = mesh.mVertexCount;
	mPolyCount = mesh.mPolyCount;

	memcpy(mVerticies, mesh.mVerticies, mesh.mVertexCount*sizeof(vertex2));
	memcpy(mIndexes, mesh.mIndexes, mesh.mPolyCount*3*sizeof(unsigned short));
}

grMesh::~grMesh()
{
	setTexture(NULL);
	safe_release_arr(mVerticies);
	safe_release_arr(mIndexes);
}

void grMesh::setTexture( grTexture* texture )
{
	if (mTexture)
		mTexture->decrRefCount();

	mTexture = texture;

	if (mTexture)
		mTexture->incRefCount();
}

grTexture* grMesh::getTexture() const
{
	return mTexture;
}

void grMesh::resize( unsigned int vertexCount, unsigned int polyCount )
{
	safe_release_arr(mVerticies);
	safe_release_arr(mIndexes);

	mVerticies = new vertex2[vertexCount];
	mIndexes = new unsigned short[polyCount*3];

	mMaxVertexCount = vertexCount;
	mMaxPolyCount = polyCount;

	mVertexCount = 0;
	mPolyCount = 0;
}

void grMesh::draw()
{
	mRenderSystem->drawMesh(this);
}

unsigned int grMesh::getMaxVertexCount() const
{
	return mMaxVertexCount;
}

unsigned int grMesh::getMaxPolyCount() const
{
	return mMaxPolyCount;
}

CLOSE_O2_NAMESPACE