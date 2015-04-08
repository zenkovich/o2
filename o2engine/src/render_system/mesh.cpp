#include "mesh.h"

#include "render_system.h"
#include "texture.h"

OPEN_O2_NAMESPACE

Mesh::Mesh(TextureRef texture /*= grTexture()*/, uint vertexCount /*= 4*/, uint polyCount /*= 2*/):
mVerticies(NULL), mIndexes(NULL), mMaxPolyCount(0), mMaxVertexCount(0), mVertexCount(0), mPolyCount(0)
{
	SetTexture(texture);
	Resize(vertexCount, polyCount);
	InitializeProperties();
}

Mesh::Mesh(const Mesh& mesh):
mVerticies(NULL), mIndexes(NULL), mMaxVertexCount(0), mMaxPolyCount(0)
{
	SetTexture(mesh.mTexture);

	Resize(mesh.mMaxVertexCount, mesh.mMaxPolyCount);

	mVertexCount = mesh.mVertexCount;
	mPolyCount = mesh.mPolyCount;

	memcpy(mVerticies, mesh.mVerticies, mesh.mMaxVertexCount*sizeof(Vertex2));
	memcpy(mIndexes, mesh.mIndexes, mesh.mMaxPolyCount*3*sizeof(uint16));

	InitializeProperties();
}

Mesh::~Mesh()
{
	SafeReleaseArr(mVerticies);
	SafeReleaseArr(mIndexes);
}

void Mesh::SetTexture(const TextureRef& texture)
{
	mTexture = texture;
}

TextureRef Mesh::GetTexture() const
{
	return mTexture;
}

void Mesh::Resize(uint vertexCount, uint polyCount)
{
	SafeReleaseArr(mVerticies);
	SafeReleaseArr(mIndexes);

	mVerticies = mnew Vertex2[vertexCount];
	mIndexes = mnew uint16[polyCount*3];

	mMaxVertexCount = vertexCount;
	mMaxPolyCount = polyCount;

	mVertexCount = 0;
	mPolyCount = 0;
}

void Mesh::Draw()
{
	AppRender()->DrawMesh(this);
}

uint Mesh::GetMaxVertexCount() const
{
	return mMaxVertexCount;
}

uint Mesh::GetMaxPolyCount() const
{
	return mMaxPolyCount;
}

void Mesh::SetMaxVertexCount(const uint& count)
{
	SafeReleaseArr(mVerticies);
	mVerticies = mnew Vertex2[count];
	mMaxVertexCount = count;
	mVertexCount = 0;
}

void Mesh::SetMaxPolyCount(const uint& count)
{
	SafeReleaseArr(mIndexes);
	mIndexes = mnew uint16[count*3];
	mMaxPolyCount = count;
	mPolyCount = 0;
}

void Mesh::InitializeProperties()
{
	REG_PROPERTY(Mesh, texture, SetTexture, GetTexture);
	REG_PROPERTY(Mesh, maxVertexCount, SetMaxVertexCount, GetMaxVertexCount);
	REG_PROPERTY(Mesh, maxPolyCount, SetMaxPolyCount, GetMaxPolyCount);
}

CLOSE_O2_NAMESPACE