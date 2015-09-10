#include "Mesh.h"

#include "Render/Render.h"

namespace o2
{
	Mesh::Mesh(Ptr<Texture> texture /*= nullptr*/, UInt vertexCount /*= 4*/, UInt polyCount /*= 2*/):
		mVerticies(NULL), mIndexes(NULL), mMaxPolyCount(0), mMaxVertexCount(0), mVertexCount(0), mPolyCount(0)
	{
		SetTexture(texture);
		Resize(vertexCount, polyCount);
		InitializeProperties();
	}

	Mesh::Mesh(const Mesh& mesh)
	{
		SetTexture(mesh.mTexture);
		Resize(mesh.mMaxVertexCount, mesh.mMaxPolyCount);

		mVertexCount = mesh.mVertexCount;
		mPolyCount = mesh.mPolyCount;

		memcpy(mVerticies, mesh.mVerticies, mesh.mMaxVertexCount*sizeof(Vertex2));
		memcpy(mIndexes, mesh.mIndexes, mesh.mMaxPolyCount*3*sizeof(UInt16));

		InitializeProperties();
	}

	Mesh::~Mesh()
	{
		delete[] mVerticies;
		delete[] mIndexes;
	}

	void Mesh::Resize(UInt vertexCount, UInt polyCount)
	{
		delete[] mVerticies;
		delete[] mIndexes;

		mVerticies = new Vertex2[vertexCount];
		mIndexes = new UInt16[polyCount*3];

		mMaxVertexCount = vertexCount;
		mMaxPolyCount = polyCount;

		mVertexCount = 0;
		mPolyCount = 0;
	}

	void Mesh::Draw()
	{
		o2Render.DrawMesh(this);
	}

	void Mesh::SetTexture(Ptr<Texture> texture)
	{
		mTexture = texture;
	}

	Ptr<Texture> Mesh::GetTexture() const
	{
		return mTexture;
	}

	void Mesh::SetMaxVertexCount(const UInt& count)
	{
		delete[] mVerticies;
		mVerticies = new Vertex2[count];
		mMaxVertexCount = count;
		mVertexCount = 0;
	}

	void Mesh::SetMaxPolyCount(const UInt& count)
	{
		delete[] mIndexes;
		mIndexes = new UInt16[count*3];
		mMaxPolyCount = count;
		mPolyCount = 0;
	}

	UInt Mesh::GetMaxVertexCount() const
	{
		return mMaxVertexCount;
	}

	UInt Mesh::GetMaxPolyCount() const
	{
		return mMaxPolyCount;
	}

	void Mesh::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Mesh, TexturePtr, SetTexture, GetTexture);
		INITIALIZE_PROPERTY(Mesh, MaxVertexCount, SetMaxVertexCount, GetMaxVertexCount);
		INITIALIZE_PROPERTY(Mesh, MaxPolyCount, SetMaxPolyCount, GetMaxPolyCount);
	}
}