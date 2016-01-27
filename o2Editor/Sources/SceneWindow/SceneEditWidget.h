#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/Camera.h"
#include "UI/Widget.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

class SceneEditWidget: public UIWidget, public CursorEventsListener
{
public:
	SceneEditWidget();
	SceneEditWidget(const SceneEditWidget& other);
	~SceneEditWidget();

	SceneEditWidget& operator=(const SceneEditWidget& other);

	// Draws widget
	void Draw();

	// Updates drawables, states and widget
	void Update(float dt);

	// Returns true if point is in this object
	bool IsUnderPoint(const Vec2F& point);

	// Returns depth (event system will catch listener with highest depth)
	float Depth();

	// Returns is listener scrollable
	bool IsScrollable() const;

	SERIALIZABLE(SceneEditWidget);

protected:
	Camera     mViewCamera;
	float      mViewCameraTargetScale = 1.0f;
	float      mViewCameraScaleSence = 0.1f/120.0f;
	float      mViewCameraScaleElasticyCoef = 30.0f;
	Vec2F      mViewCameraTargetPos;
	Vec2F      mViewCameraVelocity;
	float      mViewCameraPosElasticyCoef = 40.0f;
	float      mViewCameraVelocityDampingCoef = 10.0f;
	float      mViewCameraMinScale = 0.001f;
	float      mViewCameraMaxScale = 10000.0f;

	Color4     mBackColor = Color4(170, 170, 170, 255);
	Color4     mGridColor = Color4(190, 190, 190, 255);

	TextureRef mRenderTarget;
	Sprite*    mRenderTargetSprite;
	bool       mNeedRedraw;
	float      mDrawDepth;

protected:
	// Updates layout
	void UpdateLayout(bool forcible = false);

	// Calls when scrolling
	void OnScrolled(float scroll);

	// Calls when cursor stay down during frame
	void OnCursorStillDown(const Input::Cursor& cursor);

	// Updates camera
	void UpdateCamera(float dt);

	void RedrawScene();

	void DrawGrid();

};
