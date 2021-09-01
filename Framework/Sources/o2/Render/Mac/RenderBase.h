#pragma once

#ifdef PLATFORM_MAC
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	class RenderBase
	{
	protected:
		UInt8*   mVertexData = nullptr;           // Vertex data buffer
		UInt16*  mVertexIndexData = nullptr;      // Index data buffer
		UInt     mVertexBufferSize;               // Maximum size of vertex buffer
		UInt     mIndexBufferSize;                // Maximum size of index buffer

    protected:

        // Initializes standard shader
        void InitializeStdShader();
	};
};

#endif // PLATFORM_MAC
