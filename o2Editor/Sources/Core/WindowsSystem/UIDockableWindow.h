#pragma once

#include "UI/Window.h"

using namespace o2;

namespace Editor
{
	class UIDockWindowPlace;

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

		// Returns is window docked
		bool IsDocked() const;

		// Return pointer to docking sample sprite
		Sprite* GetDockingFrameSample() const;

		SERIALIZABLE(UIDockableWindow);

	protected:
		const float mDockSizeCoef = 0.2f;
		const float mDockBorder = 1.5f;

		bool        mDocked = false;                 // Is window is docked
		Sprite*     mDockingFrameSample = nullptr;   // Docking frame sample @SERIALIZABLE
		Animation   mDockingFrameAppearance;         // Docking frame appearance tween
		RectF       mDockingFrameCurrent;            // Docking sample current frame
		RectF       mDockingFrameTarget;             // Docking sample target  frame
		Vec2F       mNonDockSize;                    // Size of widget before docking
		Vec2F       mDragOffset;                     // Offset from cursor to dragging anchor point

	protected:
		// It is called when visible was changed, undocks when hiding
		void OnVisibleChanged();

		//Initialize animation for frame appearance
		void InitializeDockFrameAppearanceAnim();

		// Initializes drag handles
		void InitializeDragHandles();

		// It is called when head handle double clicked, trying to dock window
		void OnHeadDblCKicked(const Input::Cursor& cursor);

		// It is called when window was moved
		void OnMoved(const Input::Cursor& cursor);

		// It is called when window moving was completed
		void OnMoveCompleted(const Input::Cursor&);

		// It is called when window going to move
		void OnMoveBegin(const Input::Cursor&);

		// Searches dock place under cursor and returns parameters of them
		bool TraceDock(UIDockWindowPlace*& targetDock, Side& dockPosition, RectF& dockZoneRect);

		// Places this into empty dock
		void PlaceDock(UIDockWindowPlace* targetDock);

		// Places this into target dock as non line arranged
		void PlaceNonLineDock(UIDockWindowPlace* targetDock, Side dockPosition);

		// Places this into target dock as line arranged
		void PlaceLineDock(UIDockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect);

		// Undocks this window, using when window moved out from dock
		void Undock();

		// Sets window docked
		void SetDocked(bool docked);

		friend class WindowsLayout;
	};
}
