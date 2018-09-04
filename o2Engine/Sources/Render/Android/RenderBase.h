#pragma once

#ifdef PLATFORM_ANDROID

#include "Render/Android/OpenGL.h"

namespace o2
{
	class RenderBase
	{
	protected:
		GLuint   mStdShader;                      // Standard shader program
		GLint    mStdShaderMvpUniform;            // Standard shader matrix input parameter
		GLuint   mStdShaderTextureSample;         // Standard shader texture sample input parameter

		GLuint   mVertexBufferObject;             // Batch vercities buffer
		GLuint   mIndexBufferObject;              // Batch polygons indexes buffer

		UInt8*   mVertexData = nullptr;           // Vertex data buffer
		UInt16*  mVertexIndexData = nullptr;      // Index data buffer
		UInt     mVertexBufferSize;               // Maximum size of vertex buffer
		UInt     mIndexBufferSize;                // Maximum size of index buffer

	protected:
		// Builds vertex and fragment shaders
		GLuint BuildShaderProgram(const char* vertexSource, const char* fragmentSource);

		// Initializes standard shader
		void InitializeStdShader();
	};
};

#endif // PLATFORM_ANDROID
