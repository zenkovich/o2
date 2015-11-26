#pragma once

#include "O2.h"
using namespace o2;

// -----------------------------
// Dockable editor window widget
// -----------------------------
class UIDockableWindow: public UIWindow
{
public:
	// Default constructor
	UIDockableWindow();

	// Copy-constructor
	UIDockableWindow(const UIDockableWindow& other);

	// Destructor
	~UIDockableWindow();

	// Copy-operator
	UIDockableWindow& operator=(const UIDockableWindow& other);

	// Updates drawables, states and widget
	void Update(float dt);

	// Draws widget
	void Draw();

	// Sets window docked
	void SetDocked(bool docked);

	// Returns is window docked
	bool IsDocked() const;

	// Return pointer to docking sample sprite
	Ptr<Sprite> GetDockingFrameSample() const;

	SERIALIZABLE_IMPL(UIDockableWindow);

	IOBJECT(UIDockableWindow)
	{
		BASE_CLASS(UIWindow);

		SRLZ_FIELD(mDockingFrameSample);
	}

protected:
	const float          mDockSizeCoef = 0.2f;
	const float          mDockBorder = 3.0f;

	bool                 mDocked;                 // Is window is docked
	Ptr<Sprite>          mDockingFrameSample;     // Docking frame sample
	Animation            mDockingFrameAppearance; // Docking frame appearance tween
	RectF                mDockingFrameCurrent;    // Docking sample current frame
	RectF                mDockingFrameTarget;     // Docking sample target  frame
	Vec2F                mNonDockSize;            // Size of widget before docking
	Vec2F                mDragOffset;

protected:
	//Initialize animation for frame appearance
	void InitializeDockFrameAppearanceAnim();

	// Initializes drag handles
	void InitializeDragHandles();

	// Calls when window was moved
	void OnMoved(const Input::Cursor& cursor);

	// Calls when window moving was completed
	void OnMoveCompleted();

	// Calls when window going to move
	void OnMoveBegin();
};