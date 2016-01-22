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

	TextureRef mRenderTarget;
	Sprite*    mRenderTargetSprite;
	bool       mNeedRedraw;
	float      mDrawDepth;

	Sprite* test;

protected:
	// Updates layout
	void UpdateLayout(bool forcible = false);

	// Calls when scrolling
	void OnScrolled(float scroll);

	// Calls when cursor stay down during frame
	void OnCursorStillDown(const Input::Cursor& cursor);

	void RedrawScene();
};
