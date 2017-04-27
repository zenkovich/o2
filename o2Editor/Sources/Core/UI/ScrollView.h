#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/Camera.h"
#include "UI/Widget.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

namespace Editor
{
	// ----------------------------
	// Scrollable and zoomable view
	// ----------------------------
	class UIScrollView: public UIWidget, public CursorAreaEventsListener
	{
	public:
		// Default constructor
		UIScrollView();

		// Copy-constructor
		UIScrollView(const UIScrollView& other);

		// Destructor
		~UIScrollView();

		// Copy-operator
		UIScrollView& operator=(const UIScrollView& other);

		// Draws widget, updates render target 
		void Draw();

		// Updates drawables, states and widget
		void Update(float dt);

		// Transforms point from screen space to local space
		Vec2F ScreenToLocalPoint(const Vec2F& point);

		// Transforms point from local space to screen space
		Vec2F LocalToScreenPoint(const Vec2F& point);

		// Sets back color
		void SetBackColor(const Color4& color);

		// Sets grid color
		void SetGridColor(const Color4& color);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE(UIScrollView);

	protected:

	protected:
		bool       mReady = false;                         // Is widget initialized and ready to use
				   								           
		Sprite*    mRenderTargetSprite = nullptr;          // Render target sprite, using for caching graphics
		TextureRef mRenderTarget;                          // Render target texture, using for caching graphics
		bool       mNeedRedraw = false;                    // Is need to redraw render target
				   								           
		Color4     mBackColor;                             // Color of back @SERIALIZABLE
		Color4     mGridColor;                             // Color of grid @SERIALIZABLE

		RectF      mViewArea;                              // View area range
		Camera     mViewCamera;     					   // Scene view camera
		float      mViewCameraTargetScale = 1.0f;		   // Camera target scale
		float      mViewCameraScaleSence = 0.1f / 120.0f;  // Camera scale sense
		float      mViewCameraScaleElasticyCoef = 30.0f;   // Scale smoothing coefficient
		Vec2F      mViewCameraTargetPos;				   // Target camera position
		Vec2F      mViewCameraVelocity;					   // Camera velocity
		float      mViewCameraPosElasticyCoef = 40.0f;	   // Camera dragging smoothing coefficient
		float      mViewCameraVelocityDampingCoef = 10.0f; // Camera velocity damping coefficient
		float      mViewCameraMinScale = 0.001f;		   // Minimal camera scale
		float      mViewCameraMaxScale = 10000.0f;		   // Maximal camera scale
			       
		Basis      mLocalToScreenTransform;                 // Local to screen transformation
		Basis      mScreenToLocalTransform;                 // Screen to local transformation

	protected:
		// Updates camera
		void UpdateCamera(float dt);

		// Updates scene to screen and screen and scene transformations
		void UpdateLocalScreenTransforms();

		// Redraws content into render target
		void RedrawRenderTarget();

		// Redraws content into render target
		virtual void RedrawContent();

		// Draws grid
		virtual void DrawGrid();

		// Calls when camera position was changed
		virtual void OnCameraPositionChanged();

		// Calls when scrolling
		void OnScrolled(float scroll);

		// Calls when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor);

		// Calls when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);
	};
}
