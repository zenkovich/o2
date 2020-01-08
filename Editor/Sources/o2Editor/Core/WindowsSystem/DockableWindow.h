#pragma once

#include "o2/Scene/UI/Widgets/Window.h"

using namespace o2;

namespace Editor
{
	class DockWindowPlace;

	// -----------------------------
	// Dockable editor window widget
	// -----------------------------
	class DockableWindow: public Window
	{
	public:
		// Default constructor
		DockableWindow();

		// Copy-constructor
		DockableWindow(const DockableWindow& other);

		// Destructor
		~DockableWindow();

		// Copy-operator
		DockableWindow& operator=(const DockableWindow& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

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

		// Sets current window tab as active tab
		void SetTabActive();

		// Returns is this window active and tabbed and tab is active
		bool IsTabActive() const;

		// Sets Automatically calculating tab width when changing caption
		void SetAutoCalcuclatingTabWidth(bool enable);

		// Is Automatically calculating tab width when changing caption
		bool IsAutoCalcuclatingTabWidth() const;

		// Places this into empty dock
		void PlaceDock(DockWindowPlace* targetDock);

		// Undocks this window, using when window moved out from dock
		void Undock();

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns true if point is under drawable
		bool IsUnderPoint(const Vec2F& point) override;

		SERIALIZABLE(DockableWindow);

	protected:
		static const char* mTabLayerPath;
		static const char* mTabIconLayerPath;
		static const char* mTabCaptionLayerPath;
		static const char* mIconLayerPath;
		static const char* mCaptionLayerPath;

		const float mDockSizeCoef = 0.2f;
		const float mDockBorder = 1.5f;

		bool      mDocked = false;               // Is window docked
		Sprite*   mDockingFrameSample = nullptr; // Docking frame sample @SERIALIZABLE
		Animation mDockingFrameAppearance;       // Docking frame appearance tween
		RectF     mDockingFrameCurrent;          // Docking sample current frame
		RectF     mDockingFrameTarget;           // Docking sample target  frame
		Vec2F     mNonDockSize;                  // Size of widget before docking
		Vec2F     mDragOffset;                   // Offset from cursor to dragging anchor point

		bool  mTabState = false;             // Is window as tab
		int   mTabPosition = 0;              // Tab index
		bool  mTabActive = false;            // Is active tab
		float mTabWidth = 150.0f;            // Width of tab layer "tab/main"
		bool  mAutoCalculateTabWidth = true; // Automatically calculating tab width when changing caption

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when visible was changed, undocks when hiding
		void OnResEnableInHierarchyChanged() override;

		// It is called when widget was selected, enables active tab
		void OnFocused() override;

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
		bool TraceDock(DockWindowPlace*& targetDock, Side& dockPosition, RectF& dockZoneRect);

		// Places this into target dock as non line arranged
		void PlaceNonLineDock(DockWindowPlace* targetDock, Side dockPosition);

		// Places this into target dock as line arranged
		void PlaceLineDock(DockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect);

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
		friend class DockWindowPlace;
	};
}

CLASS_BASES_META(Editor::DockableWindow)
{
	BASE_CLASS(o2::Window);
}
END_META;
CLASS_FIELDS_META(Editor::DockableWindow)
{
	PROTECTED_FIELD(mDockSizeCoef);
	PROTECTED_FIELD(mDockBorder);
	PROTECTED_FIELD(mDocked);
	PROTECTED_FIELD(mDockingFrameSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDockingFrameAppearance);
	PROTECTED_FIELD(mDockingFrameCurrent);
	PROTECTED_FIELD(mDockingFrameTarget);
	PROTECTED_FIELD(mNonDockSize);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mTabState);
	PROTECTED_FIELD(mTabPosition);
	PROTECTED_FIELD(mTabActive);
	PROTECTED_FIELD(mTabWidth);
	PROTECTED_FIELD(mAutoCalculateTabWidth);
}
END_META;
CLASS_METHODS_META(Editor::DockableWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsDocked);
	PUBLIC_FUNCTION(Sprite*, GetDockingFrameSample);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(void, SetIconLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetIconLayout);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetTabWidth, float);
	PUBLIC_FUNCTION(float, GetTabWidth);
	PUBLIC_FUNCTION(void, SetTabActive);
	PUBLIC_FUNCTION(bool, IsTabActive);
	PUBLIC_FUNCTION(void, SetAutoCalcuclatingTabWidth, bool);
	PUBLIC_FUNCTION(bool, IsAutoCalcuclatingTabWidth);
	PUBLIC_FUNCTION(void, PlaceDock, DockWindowPlace*);
	PUBLIC_FUNCTION(void, Undock);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, InitializeDockFrameAppearanceAnim);
	PROTECTED_FUNCTION(void, InitializeDragHandles);
	PROTECTED_FUNCTION(void, OnHeadDblCKicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveCompleted, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveBegin, const Input::Cursor&);
	PROTECTED_FUNCTION(bool, TraceDock, DockWindowPlace*&, Side&, RectF&);
	PROTECTED_FUNCTION(void, PlaceNonLineDock, DockWindowPlace*, Side);
	PROTECTED_FUNCTION(void, PlaceLineDock, DockWindowPlace*, Side, RectF);
	PROTECTED_FUNCTION(void, SetTabState, float, int, bool);
	PROTECTED_FUNCTION(void, SetNonTabState);
	PROTECTED_FUNCTION(void, SetActiveTab);
	PROTECTED_FUNCTION(void, SetDocked, bool);
	PROTECTED_FUNCTION(void, RecalculateTabWidth);
}
END_META;
