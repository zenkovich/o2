#include "Mesh.h"

#include "Render/Render.h"

namespace o2
{
	Mesh::Mesh(TextureRef texture /*= TextureRef()*/, UInt vertexCount /*= 4*/, UInt polyCount /*= 2*/):
		vertices(nullptr), indexes(nullptr), mMaxPolyCount(0), mMaxVertexCount(0), vertexCount(0), polyCount(0)
	{
		SetTexture(texture);
		Resize(vertexCount, polyCount);
		InitializeProperties();
	}

	Mesh::Mesh(const Mesh& mesh):
		vertices(nullptr), indexes(nullptr), mMaxPolyCount(0), mMaxVertexCount(0), vertexCount(0), polyCount(0)
	{
		SetTexture(mesh.mTexture);
		Resize(mesh.mMaxVertexCount, mesh.mMaxPolyCount);

		vertexCount = mesh.vertexCount;
		polyCount = mesh.polyCount;

		memcpy(vertices, mesh.vertices, mesh.mMaxVertexCount*sizeof(Vertex2));
		memcpy(indexes, mesh.indexes, mesh.mMaxPolyCount*3*sizeof(UInt16));

		InitializeProperties();
	}

	Mesh::~Mesh()
	{
		delete[] vertices;
		delete[] indexes;
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		Resize(other.mMaxVertexCount, other.mMaxPolyCount);
		mTexture = other.mTexture;

		vertexCount = other.vertexCount;
		polyCount = other.polyCount;

		memcpy(vertices, other.vertices, other.mMaxVertexCount*sizeof(Vertex2));
		memcpy(indexes, other.indexes, other.mMaxPolyCount*3*sizeof(UInt16));

		return *this;
	}

	void Mesh::Resize(UInt vertexCount, UInt polyCount)
	{
		if (vertices) delete[] vertices;
		if (indexes) delete[] indexes;

		vertices = new Vertex2[vertexCount];
		indexes = new UInt16[polyCount*3];

		mMaxVertexCount = vertexCount;
		mMaxPolyCount = polyCount;

		vertexCount = 0;
		polyCount = 0;
	}

	void Mesh::Draw()
	{
		o2Render.DrawMesh(this);
		OnDrawn();
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
		delete[] vertices;
		vertices = new Vertex2[count];
		mMaxVertexCount = count;
		vertexCount = 0;
	}

	void Mesh::SetMaxPolyCount(const UInt& count)
	{
		delete[] indexes;
		indexes = new UInt16[count*3];
		mMaxPolyCount = count;
		polyCount = 0;
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