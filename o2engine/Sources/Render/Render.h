#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Render/Camera.h"
#include "Render/TextureRef.h"
#include "Render/Windows/RenderBase.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Singleton.h"

// Render access macros
#define o2Render o2::Render::Instance()

namespace o2
{
	class Mesh;
	class Font;

	// ------------------
	// 2D Graphics render
	// ------------------
	class Render: public RenderBase, public Singleton<Render>
	{
	public:
		Getter<Vec2I>        resolution;             // Screen resolution getter
		Property<Camera>     camera;          // Current camera property
		Property<RectI>      scissorRect;            // Scissor rect property
		Property<TextureRef> renderTexture;          // Render target texture property
		Getter<bool>         renderTextureAvailable; // Render textures available getter
		Getter<Vec2I>        maxTextureSize;         // Maximal texture size getter

		// Default constructor
		Render();

		// Destructor
		~Render();

		// Beginning rendering
		void Begin();

		// Finishing rendering
		void End();

		// Clearing current frame buffer with color
		void Clear(const Color4& color = Color4::Gray());

		// Returns resolution of rendering frame
		Vec2I GetResolution() const;

		// Returns device's screen dpi
		Vec2I GetDPI() const;

		// Binding camera. NULL - standard camera
		void SetCamera(const Camera& camera);

		// Returns current camera
		Camera GetCamera() const;

		// Draw single line with color
		void DrawLine(const Vec2F& a, const Vec2F& b, const Color4& color = Color4::White());

		// Draw single line with color
		void DrawLine(const Vector<Vec2F>& points, const Color4& color = Color4::White());

		// Draw rect frame with color
		void DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color = Color4::White());

		// Draw rect frame with color
		void DrawRectFrame(const RectF& rect, const Color4& color = Color4::White());

		// Draw basis frame
		void DrawBasis(const Basis& basis, const Color4& xcolor = Color4::Red(), const Color4& ycolor = Color4::Blue(),
					   const Color4& color = Color4::White());

		// Draw cross with color
		void DrawCross(const Vec2F& pos, float size = 5, const Color4& color = Color4::White());

		// Draw circle with color
		void DrawCircle(const Vec2F& pos, float radius = 5, const Color4& color = Color4::White());

		// Beginning render to stencil buffer
		void BeginRenderToStencilBuffer();

		// Finishing rendering in stencil buffer
		void EndRenderToStencilBuffer();

		// Enabling stencil test
		void EnableStencilTest();

		// Disabling stencil test
		void DisableStencilTest();

		// Returns true, if stencil test enabled
		bool IsStencilTestEnabled() const;

		// Clearing stencil buffer
		void ClearStencil();

		// Sets scissor rect
		void SetupScissorRect(const RectI& rect);

		// Returns scissor rect
		RectI GetScissorRect() const;

		// Enabling scissor test
		void EnableScissorTest();

		// Disabling scissor test
		void DisableScissorTest();

		// Returns true, if scissor test enabled
		bool IsScissorTestEnabled() const;

		// Drawing mesh
		bool DrawMesh(Ptr<Mesh> mesh);

		// Drawing mesh wire
		bool DrawMeshWire(Ptr<Mesh> mesh, const Color4& color = Color4::White());

		// Drawing lines
		bool DrawLines(Vertex2* verticies, int count);

		// Binding render target
		void SetRenderTexture(TextureRef renderTarget);

		// Unbinding render target
		void UnbindRenderTexture();

		// Returns current render target. Returns NULL if no render target
		TextureRef GetRenderTexture() const;

		// Returns true, if render target is can be used with current device
		bool IsRenderTextureAvailable() const;

		// Returns maximum texture size
		Vec2I GetMaxTextureSize() const;

	protected:
		typedef Vector<Ptr<Texture>> TexturesVec;
		typedef Vector<Ptr<Font>> FontsVec;

		Ptr<LogStream> mLog;                    // Render log stream

		TexturesVec    mTextures;               // Loaded textures
		FontsVec       mFonts;                  // Loaded fonts

		Camera         mCamera;                 // Camera transformation
		Vec2I          mResolution;             // Current back buffer size
		Vec2I          mDPI;                    // Current device screen DPI

		bool           mRenderTargetsAvailable; // True, if render targets is available
		Vec2I          mMaxTextureSize;         // Max texture size

		bool           mStencilDrawing;         // True, if drawing in stencil buffer
		bool           mStencilTest;            // True, if drawing with stencil test

		RectI          mScissorRect;            // Scissor rect, in screen space
		bool           mScissorTest;            // True, if scissor test enabled

		TextureRef     mCurrentRenderTarget;    // Current render target. NULL if rendering in back buffer

		float          mDrawingDepth;           // Current drawing depth, increments after each drawing drawables

		FT_Library     mFreeTypeLib;            // FreeType library, for rendering fonts

		bool           mReady;                  // True, if render system initialized

	protected:
		// Don't copy
		Render(const Render& other);

		// Don't copy
		Render& operator=(const Render& other);

		// Initializes free type library
		void InitializeFreeType();

		// Deinitializes free type livrary
		void DeinitializeFreeType();

		// Send buffers to draw
		void DrawPrimitives();

		// Sets orthographic view matrix by view size
		void SetupViewMatrix(const Vec2I& viewSize);

		// Updates render transformations for camera
		void UpdateCameraTransforms();

		// Checks render compatibles
		void CheckCompatibles();

		// Check textures for unloading
		void CheckTexturesUnloading();

		// Checks font for unloading
		void CheckFontsUnloading();

		// initializes properties
		void InitializeProperties();

		friend class Texture;
		friend class TextureRef;
		friend class BaseApplication;
		friend class Font;
		friend class BitmapFont;
		friend class VectorFont;
		friend class BitmapFontAsset;
		friend class VectorFontAsset;
	};
}
