#pragma once

#include "Render/IDrawable.h"
#include "Utils/CommonTypes.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"

namespace o2
{
	class Texture;

	// Triangles mesh. Containing verticies, indexes of polygons, texture
	class Mesh: public virtual IDrawable
	{
		friend class Render;
		friend class Sprite;

	public:
		Vertex2*     mVerticies;      // Vertex buffer
		UInt16*      mIndexes;        // Index buffer
				     
		UInt         mVertexCount;    // Current verticies count
		UInt         mPolyCount;      // Current polygons in mesh

	protected:
		Ptr<Texture> mTexture;        // Texture

		UInt         mMaxVertexCount; // Max size of vertex buffer
		UInt         mMaxPolyCount;   // Max polygons count, mMaxPolyCount*3 - os index buffer max size

	public:
		//properties
		Property<Ptr<Texture>> TexturePtr;     // Texture property, uses set/getTexture
		Property<UInt>         MaxVertexCount; // Max vertex count property, uses set/getMaxVertexCount
		Property<UInt>         MaxPolyCount;   // Max polygons count property, uses set/getMaxPolyCount

		// Constructor
		Mesh(Ptr<Texture> texture = nullptr, UInt vertexCount = 4, UInt polyCount = 2);

		// copy ctor
		Mesh(const Mesh& mesh);

		// dtor
		~Mesh();

		// Resizing mesh buffers, looses data
		void Resize(UInt vertexCount, UInt polyCount);

		// Drawing mesh
		void Draw();

		// Sets texture
		void SetTexture(Ptr<Texture> texture);

		// Returns texture ptr
		Ptr<Texture> GetTexture() const;

		// Sets max vertex count buffer
		void SetMaxVertexCount(const UInt& count);

		// Sets max polygons count buffer
		void SetMaxPolyCount(const UInt& count);

		// Returns max vertex buffer size
		UInt GetMaxVertexCount() const;

		// Returns max polygons count
		UInt GetMaxPolyCount() const;

	protected:
		// Initializing properties
		void InitializeProperties();
	};
}
