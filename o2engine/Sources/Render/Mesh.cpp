#include "Mesh.h"

#include "Render/Render.h"

namespace o2
{
	Mesh::Mesh(TextureRef texture /*= TextureRef()*/, UInt vertexCount /*= 4*/, UInt polyCount /*= 2*/):
		mVertices(NULL), mIndexes(NULL), mMaxPolyCount(0), mMaxVertexCount(0), mVertexCount(0), mPolyCount(0)
	{
		SetTexture(texture);
		Resize(vertexCount, polyCount);
		InitializeProperties();
	}

	Mesh::Mesh(const Mesh& mesh):
		mVertices(NULL), mIndexes(NULL), mMaxPolyCount(0), mMaxVertexCount(0), mVertexCount(0), mPolyCount(0)
	{
		SetTexture(mesh.mTexture);
		Resize(mesh.mMaxVertexCount, mesh.mMaxPolyCount);

		mVertexCount = mesh.mVertexCount;
		mPolyCount = mesh.mPolyCount;

		memcpy(mVertices, mesh.mVertices, mesh.mMaxVertexCount*sizeof(Vertex2));
		memcpy(mIndexes, mesh.mIndexes, mesh.mMaxPolyCount*3*sizeof(UInt16));

		InitializeProperties();
	}

	Mesh::~Mesh()
	{
		delete[] mVertices;
		delete[] mIndexes;
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		Resize(other.mMaxVertexCount, other.mMaxPolyCount);
		mTexture = other.mTexture;

		mVertexCount = other.mVertexCount;
		mPolyCount = other.mPolyCount;

		memcpy(mVertices, other.mVertices, other.mMaxVertexCount*sizeof(Vertex2));
		memcpy(mIndexes, other.mIndexes, other.mMaxPolyCount*3*sizeof(UInt16));

		return *this;
	}

	void Mesh::Resize(UInt vertexCount, UInt polyCount)
	{
		if (mVertices) delete[] mVertices;
		if (mIndexes) delete[] mIndexes;

		mVertices = new Vertex2[vertexCount];
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

	void Mesh::SetTexture(TextureRef texture)
	{
		mTexture = texture;
	}

	TextureRef Mesh::GetTexture() const
	{
		return mTexture;
	}

	void Mesh::SetMaxVertexCount(const UInt& count)
	{
		delete[] mVertices;
		mVertices = new Vertex2[count];
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
		INITIALIZE_PROPERTY(Mesh, texture, SetTexture, GetTexture);
		INITIALIZE_PROPERTY(Mesh, maxVertexCount, SetMaxVertexCount, GetMaxVertexCount);
		INITIALIZE_PROPERTY(Mesh, maxPolyCount, SetMaxPolyCount, GetMaxPolyCount);
	}
}