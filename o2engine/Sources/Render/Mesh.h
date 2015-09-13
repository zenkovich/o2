#pragma once

#include "Render/IDrawable.h"
#include "Render/TextureRef.h"
#include "Utils/CommonTypes.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"

namespace o2
{
	// Triangles mesh. Containing vertices, indexes of polygons, texture
	class Mesh: public virtual IDrawable
	{
	public:
		Vertex2*   mVertices;      // Vertex buffer
		UInt16*    mIndexes;        // Index buffer

		UInt       mVertexCount;    // Current vertices count
		UInt       mPolyCount;      // Current polygons in mesh

	protected:
		TextureRef mTexture;        // Texture

		UInt       mMaxVertexCount; // Max size of vertex buffer
		UInt       mMaxPolyCount;   // Max polygons count, mMaxPolyCount*3 - is index buffer max size

	public:
		//properties
		Property<TextureRef> TexturePtr;     // Texture property
		Property<UInt>       MaxVertexCount; // Max vertex count property
		Property<UInt>       MaxPolyCount;   // Max polygons count property

		// Constructor
		Mesh(TextureRef texture = TextureRef(), UInt vertexCount = 4, UInt polyCount = 2);

		// Copy-constructor
		Mesh(const Mesh& mesh);

		// Destructor
		~Mesh();

		// Assign operator
		Mesh& operator=(const Mesh& other);

		// Resizing mesh buffers, looses data
		void Resize(UInt vertexCount, UInt polyCount);

		// Drawing mesh
		void Draw();

		// Sets texture
		void SetTexture(TextureRef texture);

		// Returns texture ptr
		TextureRef GetTexture() const;

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

		friend class Render;
		friend class Sprite;
	};
}
