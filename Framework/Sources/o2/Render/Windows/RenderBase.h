#pragma once

#ifdef PLATFORM_WINDOWS

#include "o2/Render/Windows/OpenGL.h"
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	class RenderBase
	{
	protected:
		HGLRC mGLContext; // OpenGL context
		HDC   mHDC;       // Windows frame device context

		UInt8*  mVertexData;               // Vertex data buffer
		UInt16* mVertexIndexData;          // Index data buffer
		UInt    mVertexBufferSize = 6000;  // Maximum size of vertex buffer
		UInt    mIndexBufferSize = 6000*3; // Maximum size of index buffer
	};
};

#endif // PLATFORM_WINDOWS
