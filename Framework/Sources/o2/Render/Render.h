#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#if defined PLATFORM_WINDOWS
#include "o2/Render/Windows/RenderBase.h"
#elif defined PLATFORM_ANDROID
#include "o2/Render/Android/RenderBase.h"
#elif defined PLATFORM_MAC
#include "o2/Render/Mac/RenderBase.h"
#elif defined PLATFORM_IOS
#include "o2/Render/iOS/RenderBase.h"
#endif

#include "o2/Render/Camera.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Math/Vertex.h"
#include "o2/Utils/Singleton.h"

// Render access macros
#define o2Render o2::Render::Instance()

namespace o2
{
	class Mesh;
	class Font;
	class Sprite;
	class CursorAreaEventListenersLayer;

	// ------------------
	// 2D Graphics render
	// ------------------
	class Render: public RenderBase, public Singleton<Render>
	{
	public:
		// ---------------------
		// Scissor clipping info
		// ---------------------
		struct ScissorInfo
		{
			float mBeginDepth;   // Drawing depth on enabling clipping
			float mEndDepth;     // Drawing depth on disabling clipping
			RectI mScissorRect;  // Scissor clipping rectangle

			ScissorInfo();
			ScissorInfo(const RectI& rect, float beginDepth);

			bool operator==(const ScissorInfo& other) const;
		};

		// --------------------------------
		// Scissor clipping stack info item
		// --------------------------------
		struct ScissorStackEntry
		{
			RectI mScrissorRect;       // Clipping scissor rectangle
			RectI mSummaryScissorRect; // Real clipping rectangle: summary of top clipping rectangles
			bool  mRenderTarget;       // Is render target turned on this step

			ScissorStackEntry();
			ScissorStackEntry(const RectI& rect, const RectI& summaryRect, bool renderTarget = false);

			bool operator==(const ScissorStackEntry& other) const;
		};

	public:
		PROPERTIES(Render);
		PROPERTY(Camera, camera, SetCamera, GetCamera);                          // Current camera property
		PROPERTY(RectI, scissorRect, EnableScissorTest, GetScissorRect);         // Scissor rect property
		PROPERTY(TextureRef, renderTexture, BindRenderTexture, GetRenderTexture); // Render target texture property
		GETTER(Vec2I, resolution, GetResolution);                                // Screen resolution getter
		GETTER(bool, renderTextureAvailable, IsRenderTextureAvailable);          // Render textures available getter
		GETTER(Vec2I, maxTextureSize, GetMaxTextureSize);                        // Maximal texture size getter

	public:
		Function<void()> preRender;  // Pre rendering event. Call after beginning drawing. Clearing every frame
		Function<void()> postRender; // Post rendering event. Call before ending drawing. Clearing every frame

	public:
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

		// Returns current buffer resolution
		Vec2I GetCurrentResolution() const;

		// Returns device's screen dpi
		Vec2I GetDPI() const;

		// Returns draw calls count at last frame
		int GetDrawCallsCount();

		// Binding camera. NULL - standard camera
		void SetCamera(const Camera& camera);

		// Returns current camera
		Camera GetCamera() const;

		// Draws polygon
		void DrawFilledPolygon(const Vector<Vec2F>& points, const Color4& color = Color4::White());

		// Draws polygon
		void DrawFilledPolygon(const Vertex* points, int vertexCount);

		// Draws single line with color
		void DrawLine(const Vec2F& a, const Vec2F& b, const Color4& color = Color4::White());

		// Draws single line with color
		void DrawArrow(const Vec2F& a, const Vec2F& b, const Color4& color = Color4::White(),
					   const Vec2F& arrowSize = Vec2F(10, 10));

		// Draws single line with color
		void DrawLine(const Vector<Vec2F>& points, const Color4& color = Color4::White());

		// Draws rect frame with color
		void DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color = Color4::White());

		// Draws rect frame with color
		void DrawRectFrame(const RectF& rect, const Color4& color = Color4::White());

		// Draws basis frame
		void DrawBasis(const Basis& basis, const Color4& xcolor = Color4::Red(), const Color4& ycolor = Color4::Blue(),
					   const Color4& color = Color4::White());

		// Draws cross with color
		void DrawCross(const Vec2F& pos, float size = 5, const Color4& color = Color4::White());

		// Draws circle with color
		void DrawCircle(const Vec2F& pos, float radius = 5, const Color4& color = Color4::White(), int segCount = 20);

		// Draws filled circle with color
		void DrawFilledCircle(const Vec2F& pos, float radius = 5, const Color4& color = Color4::White(), int segCount = 20);

		// Draws bezier curve with color
		void DrawBezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
							 const Color4& color = Color4::White());

		// Draws bezier curve with color
		void DrawBezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
								  const Color4& color = Color4::White(), const Vec2F& arrowSize = Vec2F(10, 10));

		// Draws anti-aliased single line with color
		void DrawAALine(const Vec2F& a, const Vec2F& b, const Color4& color = Color4::White(),
						float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased single line with color
		void DrawAAArrow(const Vec2F& a, const Vec2F& b, const Color4& color = Color4::White(),
						 const Vec2F& arrowSize = Vec2F(10, 10),
						 float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased single line with color
		void DrawAALine(const Vector<Vec2F>& points, const Color4& color = Color4::White(),
						float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased rect frame with color
		void DrawAARectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color = Color4::White(),
							 float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased rect frame with color
		void DrawAARectFrame(const RectF& rect, const Color4& color = Color4::White(),
							 float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased basis frame
		void DrawAABasis(const Basis& basis, const Color4& xcolor = Color4::Red(), const Color4& ycolor = Color4::Blue(),
						 const Color4& color = Color4::White(),
						 float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased cross with color
		void DrawAACross(const Vec2F& pos, float size = 5, const Color4& color = Color4::White(),
						 float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased circle with color
		void DrawAACircle(const Vec2F& pos, float radius = 5, const Color4& color = Color4::White(), int segCount = 20,
						  float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased bezier curve with color
		void DrawAABezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
							   const Color4& color = Color4::White(),
							   float width = 1.0f, LineType lineType = LineType::Solid);

		// Draws anti-aliased bezier curve with color
		void DrawAABezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
									const Color4& color = Color4::White(), const Vec2F& arrowSize = Vec2F(10, 10),
									float width = 1.0f, LineType lineType = LineType::Solid);

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

		// Returns scissor rect
		RectI GetScissorRect() const;

		// Returns result scissor rect of all scissor stack
		RectI GetResScissorRect() const;

		// Returns scissors stack
		const Vector<ScissorStackEntry>& GetScissorsStack() const;

		// Enabling scissor test
		void EnableScissorTest(const RectI& rect);

		// Disabling scissor test
		void DisableScissorTest(bool forcible = false);

		// Returns true, if scissor test enabled
		bool IsScissorTestEnabled() const;

		// Returns true when specified rectangle is fully clipped by current scissor test
		bool IsClippedByScissor(const RectF& rect) const;

		// Returns true when specified point is clipped by current scissor test
		bool IsClippedByScissor(const Vec2F& point) const;

		// Draws mesh
		void DrawMesh(Mesh* mesh);

		// Draws data from buffer with specified texture and primitive type
		void DrawBuffer(PrimitiveType primitiveType, Vertex* vertices, UInt verticesCount,
						UInt16* indexes, UInt elementsCount, const TextureRef& texture);

		// Draws mesh wire
		void DrawMeshWire(Mesh* mesh, const Color4& color = Color4::White());

		// Draws hard poly line. Vertices - buffer of vertex pairs for each line
		void DrawPolyLine(Vertex* vertices, int count, float width = 1.0f);

		// Draws anti-aliased lines
		void DrawAAPolyLine(Vertex* vertices, int count, float width = 1.0f, LineType lineType = LineType::Solid,
							bool scaleToScreenSpace = true);

	    // Binding render target
		void BindRenderTexture(TextureRef renderTarget);

		// Unbinding render target
		void UnbindRenderTexture();

		// Returns current render target. Returns NULL if no render target
		TextureRef GetRenderTexture() const;

		// Returns true, if render target is can be used with current device
		bool IsRenderTextureAvailable() const;

		// Returns maximum texture size
		Vec2I GetMaxTextureSize() const;

		// Returns last draw depth of mesh
		float GetDrawingDepth();

		// Returns scissor infos at current frame
		const Vector<ScissorInfo>& GetScissorInfos() const;

	protected:
		PrimitiveType mCurrentPrimitiveType; // Type of drawing primitives for next DIP

		Texture* mLastDrawTexture = nullptr; // Stored texture ptr from last DIP
		UInt     mLastDrawVertex;            // Last vertex idx for next DIP
		UInt     mLastDrawIdx;               // Last vertex index for next DIP
		UInt     mTrianglesCount;            // Triangles count for next DIP
		UInt     mFrameTrianglesCount;       // Total triangles at current frame
		UInt     mDIPCount;                  // DrawIndexedPrimitives calls count

		Ref<LogStream> mLog; // Render log stream

		Vector<Texture*> mTextures; // Loaded textures
		Vector<Font*>    mFonts;    // Loaded fonts

		Camera mCamera;            // Camera transformation
		Vec2I  mResolution;        // Primary back buffer size
		Vec2I  mCurrentResolution; // Current back buffer size
		Vec2F  mViewScale;         // Current view scale, depends on camera
		Vec2F  mInvViewScale;      // Inverted mViewScale
		Vec2I  mDPI;               // Current device screen DPI

		bool  mRenderTargetsAvailable; // True, if render targets is available
		Vec2I mMaxTextureSize;         // Max texture size

		bool mStencilDrawing; // True, if drawing in stencil buffer
		bool mStencilTest;    // True, if drawing with stencil test

		Vector<ScissorInfo>       mScissorInfos;       // Scissor clipping depth infos vector
		Vector<ScissorStackEntry> mStackScissors;      // Stack of scissors clippings
		bool                      mClippingEverything; // Is everything clipped

		TextureRef mCurrentRenderTarget; // Current render target. NULL if rendering in back buffer

		float mDrawingDepth; // Current drawing depth, increments after each drawing drawables

		FT_Library mFreeTypeLib; // FreeType library, for rendering fonts

		Vector<Sprite*> mSprites; // All sprites

		UInt16*    mHardLinesIndexData; // Index data buffer
		TextureRef mSolidLineTexture;   // Solid line texture
		TextureRef mDashLineTexture;    // Dash line texture

		bool mReady; // True, if render system initialized

	protected:
		// Don't copy
		Render(const Render& other);

		// Don't copy
		Render& operator=(const Render& other);

		// Initializes index buffer for drawing lines - pairs of lines beginnings and ends
		void InitializeLinesIndexBuffer();

		// Initializeslines textures
		void InitializeLinesTextures();

		// Initializes free type library
		void InitializeFreeType();

		// Deinitializes free type library
		void DeinitializeFreeType();

		// Called when target frame or window was resized
		void OnFrameResized();

		// Send buffers to draw
		void DrawPrimitives();

		// Sets orthographic view matrix by view size
		void SetupViewMatrix(const Vec2I& viewSize);

		// Updates render transformations for camera
		void UpdateCameraTransforms();

		// Calculates screen space scissor clipping rectangle from camera space rectangle
		RectI CalculateScreenSpaceScissorRect(const RectF& cameraSpaceScissorRect) const;

		// Checks render compatibles
		void CheckCompatibles();

		// Check textures for unloading
		void CheckTexturesUnloading();

		// Checks font for unloading
		void CheckFontsUnloading();

		// Called when assets was rebuilded
		void OnAssetsRebuilded(const Vector<UID>& changedAssets);

		friend class Application;
		friend class BitmapFont;
		friend class BitmapFontAsset;
		friend class Font;
		friend class Sprite;
		friend class Texture;
		friend class TextureRef;
		friend class VectorFont;
		friend class VectorFontAsset;
		friend class WndProcFunc;
		friend struct ApplicationPlatformWrapper;
	};
}
