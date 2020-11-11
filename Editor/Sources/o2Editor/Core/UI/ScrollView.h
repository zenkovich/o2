#pragma once

#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Render/Camera.h"
#include "o2/Render/TextureRef.h"
#include "o2/Scene/UI/Widget.h"

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
	class ScrollView: public Widget, public CursorAreaEventsListener
	{
	public:
		bool horGridEnabled = true; // Is horizontal lines of grid are enabled
		bool verGridEnabled = true; // Is vertical lines of grid are enabled

	public:
		// Default constructor
		ScrollView();

		// Copy-constructor
		ScrollView(const ScrollView& other);

		// Destructor
		~ScrollView();

		// Copy-operator
		ScrollView& operator=(const ScrollView& other);

		// Draws widget, updates render target 
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Transforms point from screen space to local space
		Vec2F ScreenToLocalPoint(const Vec2F& point);

		// Transforms point from local space to screen space
		Vec2F LocalToScreenPoint(const Vec2F& point);

		// Returns camera scale
		Vec2F GetCameraScale() const;

		// Sets view camera
		void SetCamera(const Camera& camera);

		// Returns view camera
		const Camera& GetCamera() const;

		// Sets back color
		void SetBackColor(const Color4& color);

		// Sets grid color
		void SetGridColor(const Color4& color);

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns is this widget can be focused
		bool IsFocusable() const override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ScrollView);

	protected:
		bool mReady = false; // Is widget initialized and ready to use
				   								           
		Sprite*    mRenderTargetSprite = nullptr; // Render target sprite, using for caching graphics
		TextureRef mRenderTarget;                 // Render target texture, using for caching graphics
		bool       mNeedRedraw = false;           // Is need to redraw render target
				   								           
		Color4 mBackColor; // Color of back @SERIALIZABLE
		Color4 mGridColor; // Color of grid @SERIALIZABLE

		RectF  mViewArea;                              // View area range
		Camera mViewCamera;     					   // Scene view camera
		Vec2F  mViewCameraTargetScale = Vec2F(1, 1);   // Camera target scale
		float  mViewCameraScaleSence = 0.1f / 120.0f;  // Camera scale sense
		float  mViewCameraScaleElasticyCoef = 30.0f;   // Scale smoothing coefficient
		Vec2F  mViewCameraTargetPos;				   // Target camera position
		Vec2F  mViewCameraVelocity;					   // Camera velocity
		float  mViewCameraPosElasticyCoef = 30.0f;	   // Camera dragging smoothing coefficient
		float  mViewCameraVelocityDampingCoef = 10.0f; // Camera velocity damping coefficient
		float  mViewCameraMinScale = 0.001f;		   // Minimal camera scale
		float  mViewCameraMaxScale = 10000.0f;		   // Maximal camera scale
			       
		Basis mLocalToScreenTransform; // Local to screen transformation
		Basis mScreenToLocalTransform; // Screen to local transformation

	protected:
		// It is called when transformation was changed and updated, updates render texture and sprite
		void OnTransformUpdated() override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Updates camera
		void UpdateCamera(float dt);

		// Returns transformation basis from screen to local for camera
		Basis GetCameraScreenToLocalTransform(const Camera& camera) const;

		// Updates scene to screen and screen and scene transformations
		void UpdateLocalScreenTransforms();

		// Redraws content into render target
		void RedrawRenderTarget();

		// Redraws content into render target
		virtual void RedrawContent();

		// Draws grid
		virtual void DrawGrid();

		// It is called when camera position was changed
		virtual void OnCameraTransformChanged();

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// Updates camera's scale and position
		void ChangeCameraScaleRelativeToCursor(const Vec2F& newScale);

		// It is called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(Editor::ScrollView)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::ScrollView)
{
	PUBLIC_FIELD(horGridEnabled).DEFAULT_VALUE(true);
	PUBLIC_FIELD(verGridEnabled).DEFAULT_VALUE(true);
	PROTECTED_FIELD(mReady).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mRenderTargetSprite).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mRenderTarget);
	PROTECTED_FIELD(mNeedRedraw).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mBackColor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mGridColor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mViewArea);
	PROTECTED_FIELD(mViewCamera);
	PROTECTED_FIELD(mViewCameraTargetScale).DEFAULT_VALUE(Vec2F(1, 1));
	PROTECTED_FIELD(mViewCameraScaleSence).DEFAULT_VALUE(0.1f / 120.0f);
	PROTECTED_FIELD(mViewCameraScaleElasticyCoef).DEFAULT_VALUE(30.0f);
	PROTECTED_FIELD(mViewCameraTargetPos);
	PROTECTED_FIELD(mViewCameraVelocity);
	PROTECTED_FIELD(mViewCameraPosElasticyCoef).DEFAULT_VALUE(30.0f);
	PROTECTED_FIELD(mViewCameraVelocityDampingCoef).DEFAULT_VALUE(10.0f);
	PROTECTED_FIELD(mViewCameraMinScale).DEFAULT_VALUE(0.001f);
	PROTECTED_FIELD(mViewCameraMaxScale).DEFAULT_VALUE(10000.0f);
	PROTECTED_FIELD(mLocalToScreenTransform);
	PROTECTED_FIELD(mScreenToLocalTransform);
}
END_META;
CLASS_METHODS_META(Editor::ScrollView)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(Vec2F, ScreenToLocalPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, LocalToScreenPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetCameraScale);
	PUBLIC_FUNCTION(void, SetCamera, const Camera&);
	PUBLIC_FUNCTION(const Camera&, GetCamera);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(void, SetGridColor, const Color4&);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateCamera, float);
	PROTECTED_FUNCTION(Basis, GetCameraScreenToLocalTransform, const Camera&);
	PROTECTED_FUNCTION(void, UpdateLocalScreenTransforms);
	PROTECTED_FUNCTION(void, RedrawRenderTarget);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawGrid);
	PROTECTED_FUNCTION(void, OnCameraTransformChanged);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, ChangeCameraScaleRelativeToCursor, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
}
END_META;
