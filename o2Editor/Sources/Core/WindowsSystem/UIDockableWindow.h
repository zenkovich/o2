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

		// Sets icon sprite
		void SetIcon(Sprite* icon);

		// Returns icon sprite
		Sprite* GetIcon() const;

		// Sets icon layer layout
		void SetIconLayout(const Layout& layout);

		// Returns icon layer layout
		Layout GetIconLayout() const;

		// Sets caption text
		void SetCaption(const WString& caption);

		// Returns caption text
		WString GetCaption() const;

		// Sets tab size
		void SetTabWidth(float width);

		// Sets tab width
		float GetTabWidth() const;

		// Sets Automatically calculating tab width when changing caption
		void SetAutoCalcuclatingTabWidth(bool enable);

		// Is Automatically calculating tab width when changing caption
		bool IsAutoCalcuclatingTabWidth() const;

		// Places this into empty dock
		void PlaceDock(UIDockWindowPlace* targetDock);

		// Undocks this window, using when window moved out from dock
		void Undock();

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Returns true if point is under drawable
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UIDockableWindow);

	protected:
		static const char* mTabLayerPath;
		static const char* mTabIconLayerPath;
		static const char* mTabCaptionLayerPath;
		static const char* mIconLayerPath;
		static const char* mCaptionLayerPath;

		const float mDockSizeCoef = 0.2f;
		const float mDockBorder = 1.5f;

		bool        mDocked = false;                 // Is window docked
		Sprite*     mDockingFrameSample = nullptr;   // Docking frame sample @SERIALIZABLE
		Animation   mDockingFrameAppearance;         // Docking frame appearance tween
		RectF       mDockingFrameCurrent;            // Docking sample current frame
		RectF       mDockingFrameTarget;             // Docking sample target  frame
		Vec2F       mNonDockSize;                    // Size of widget before docking
		Vec2F       mDragOffset;                     // Offset from cursor to dragging anchor point

		bool        mTabState = false;               // Is window as tab
		int         mTabPosition = 0;                // Tab index
		bool        mTabActive = false;              // Is active tab
		float       mTabWidth = 150.0f;              // Width of tab layer "tab/main"
		bool        mAutoCalculateTabWidth = true;   // Automatically calculating tab width when changing caption

	protected:
		// It is called when visible was changed, undocks when hiding
		void OnVisibleChanged();

		// It is called when widget was selected, enables active tab
		void OnFocused();

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

		// Places this into target dock as non line arranged
		void PlaceNonLineDock(UIDockWindowPlace* targetDock, Side dockPosition);

		// Places this into target dock as line arranged
		void PlaceLineDock(UIDockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect);

		// Sets window as one of tab
		void SetTabState(float offset, int position, bool isFirst);

		// Sets window regular state, not as tab
		void SetNonTabState();

		// Sets tab as active
		void SetActiveTab();

		// Sets window docked
		void SetDocked(bool docked);

		// Recalculates tab width by caption size
		void RecalculateTabWidth();

		friend class WindowsLayout;
		friend class UIDockWindowPlace;
	};
}
