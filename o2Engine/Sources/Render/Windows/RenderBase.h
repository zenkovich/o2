#pragma once

#include "Render/Windows/OpenGL.h"
#include "Utils/Types/CommonTypes.h"
#include "Utils/Math/Vector2.h"


namespace o2
{
	class Texture;

	class RenderBase
	{
	protected:
		HGLRC         mGLContext;                // OpenGL context
		HDC           mHDC;                      // Windows frame device context

		UInt8*        mVertexData;               // Vertex data buffer
		UInt16*       mVertexIndexData;          // Index data buffer
		UInt          mVertexBufferSize = 6000;  // Maximum size of vertex buffer
		UInt          mIndexBufferSize = 6000*3; // Maximum size of index buffer
		PrimitiveType mCurrentPrimitiveType;     // Type of drawing primitives for next DIP
				     
		Texture*      mLastDrawTexture;          // Stored texture ptr from last DIP
		UInt          mLastDrawVertex;           // Last vertex idx for next DIP
		UInt          mLastDrawIdx;              // Last vertex index for next DIP
		UInt          mTrianglesCount;           // Triangles count for next DIP
		UInt          mFrameTrianglesCount;      // Total triangles at current frame
		UInt          mDIPCount;                 // DrawIndexedPrimitives calls count
	};
};
