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

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(DockableWindow);

	protected:
		static const char* mTabLayerPath;
		static const char* mTabIconLayerPath;
		static const char* mTabCaptionLayerPath;
		static const char* mIconLayerPath;
		static const char* mCaptionLayerPath;

		const float mDockSizeCoef = 0.2f;
		const float mDockBorder = 1.5f;

		bool            mDocked = false;               // Is window docked
		Sprite*         mDockingFrameSample = nullptr; // Docking frame sample @SERIALIZABLE
		AnimationPlayer mDockingFrameAppearance;       // Docking frame appearance tween
		RectF           mDockingFrameCurrent;          // Docking sample current frame
		RectF           mDockingFrameTarget;           // Docking sample target  frame
		Vec2F           mNonDockSize;                  // Size of widget before docking
		Vec2F           mDragOffset;                   // Offset from cursor to dragging anchor point

		bool  mTabState = false;             // Is window as tab
		int   mTabPosition = 0;              // Tab index
		bool  mTabActive = false;            // Is active tab
		float mTabWidth = 150.0f;            // Width of tab layer "tab/main"
		bool  mAutoCalculateTabWidth = true; // Automatically calculating tab width when changing caption

	protected:
		// It is called when visible was changed, undocks when hiding
		void OnEnableInHierarchyChanged() override;

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
	FIELD().PROTECTED().DEFAULT_VALUE(0.2f).NAME(mDockSizeCoef);
	FIELD().PROTECTED().DEFAULT_VALUE(1.5f).NAME(mDockBorder);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDocked);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mDockingFrameSample);
	FIELD().PROTECTED().NAME(mDockingFrameAppearance);
	FIELD().PROTECTED().NAME(mDockingFrameCurrent);
	FIELD().PROTECTED().NAME(mDockingFrameTarget);
	FIELD().PROTECTED().NAME(mNonDockSize);
	FIELD().PROTECTED().NAME(mDragOffset);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mTabState);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mTabPosition);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mTabActive);
	FIELD().PROTECTED().DEFAULT_VALUE(150.0f).NAME(mTabWidth);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mAutoCalculateTabWidth);
}
END_META;
CLASS_METHODS_META(Editor::DockableWindow)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const DockableWindow&);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsDocked);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetDockingFrameSample);
	FUNCTION().PUBLIC().SIGNATURE(void, SetIcon, Sprite*);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetIcon);
	FUNCTION().PUBLIC().SIGNATURE(void, SetIconLayout, const Layout&);
	FUNCTION().PUBLIC().SIGNATURE(Layout, GetIconLayout);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCaption, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetCaption);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTabWidth, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetTabWidth);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTabActive);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsTabActive);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAutoCalcuclatingTabWidth, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsAutoCalcuclatingTabWidth);
	FUNCTION().PUBLIC().SIGNATURE(void, PlaceDock, DockWindowPlace*);
	FUNCTION().PUBLIC().SIGNATURE(void, Undock);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFocused);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeDockFrameAppearanceAnim);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeDragHandles);
	FUNCTION().PROTECTED().SIGNATURE(void, OnHeadDblCKicked, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMoved, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMoveCompleted, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMoveBegin, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(bool, TraceDock, DockWindowPlace*&, Side&, RectF&);
	FUNCTION().PROTECTED().SIGNATURE(void, PlaceNonLineDock, DockWindowPlace*, Side);
	FUNCTION().PROTECTED().SIGNATURE(void, PlaceLineDock, DockWindowPlace*, Side, RectF);
	FUNCTION().PROTECTED().SIGNATURE(void, SetTabState, float, int, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, SetNonTabState);
	FUNCTION().PROTECTED().SIGNATURE(void, SetActiveTab);
	FUNCTION().PROTECTED().SIGNATURE(void, SetDocked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, RecalculateTabWidth);
}
END_META;
