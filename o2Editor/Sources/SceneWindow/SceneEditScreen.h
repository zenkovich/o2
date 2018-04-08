#pragma once

#include "Core/UI/ScrollView.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "Render/Camera.h"
#include "Render/IDrawable.h"
#include "Render/TextureRef.h"
#include "Utils/Editor/DragAndDrop.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class Actor;
	class DrawableComponent;
	class UITree;
}

// Editor scene screen accessor macros
#define o2EditorSceneScreen SceneEditScreen::Instance()

namespace Editor
{
	class SceneDragHandle;
	class IEditTool;
	class UIActorsTree;

	// --------------------
	// Scene editing screen
	// --------------------
	class SceneEditScreen: public DragDropArea, public KeyboardEventsListener,
		public Singleton<SceneEditScreen>, public UIScrollView
	{
	public:
		typedef Vector<Actor*> ActorsVec;
		typedef Vector<DrawableComponent*> DrawableComponentsVec;
		typedef Vector<SceneDragHandle*> DragHandlesVec;
		typedef Vector<IEditTool*> ToolsVec;

	public:
		Function<void(const ActorsVec&)> onSelectionChanged; // Actors selection change event

		// Default constructor
		SceneEditScreen();

		// Copy-constructor
		SceneEditScreen(const SceneEditScreen& other);

		// Destructor
		~SceneEditScreen();

		// Draws widget
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Transforms point from screen space to scene space
		Vec2F ScreenToScenePoint(const Vec2F& point);

		// Transforms point from scene space to screen space
		Vec2F SceneToScreenPoint(const Vec2F& point);

		// Transforms point from screen space to scene space
		Vec2F ScreenToSceneVector(const Vec2F& point);

		// Transforms point from scene space to screen space
		Vec2F SceneToScreenVector(const Vec2F& point);

		// Draws actor selection
		void DrawActorSelection(Actor* actor, const Color4& color);

		// Selects actors
		void SelectActors(ActorsVec actors, bool additive = true);

		// Selects actor
		void SelectActor(Actor* actor, bool additive = true);

		// Selects all actors
		void SelectAllActors();

		// Clears actors selection
		void ClearSelection();

		// Selects tool with type
		template<typename _type>
		void SelectTool();

		// Returns selected actors array
		const ActorsVec& GetSelectedActors() const;

		// Returns top selected actors in hierarchy
		const ActorsVec& GetTopSelectedActors() const;

		// Returns color for single selected actor
		const Color4& GetSingleActorSelectionColor() const;

		// Return color for multiple selected actors
		const Color4& GetManyActorsSelectionColor() const;

		// It is called when scene was changed and needs to redraw
		void OnSceneChanged();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(SceneEditScreen);

	protected:
		Color4         mSelectedActorColor = Color4(220, 220, 220, 255);      // Selected actor color
		Color4         mMultiSelectedActorColor = Color4(220, 220, 220, 100); // Selected actor color
		float          mActorMinimalSelectionSize = 10.0f;                    // Minimal actor size on pixels

		UIActorsTree*  mActorsTree;								              // Pointer to actors tree widget
		ActorsVec      mSelectedActors;							              // Current selected actors
		ActorsVec      mTopSelectedActors;                                    // Current selected actors most top in hierarchy
		bool           mSelectedFromThis = false;				              // True if selection changed from this, needs to break recursive selection update

		ToolsVec       mTools;									              // Available tools
		IEditTool*     mEnabledTool = nullptr;					              // Current enabled tool

		DragHandlesVec mDragHandles; 			                              // Dragging handles array
		
	protected:
		// Initializes tools
		void InitializeTools(const Type* toolType = nullptr);

		// Returns true if some handle hovered or pressed by cursor
		bool IsHandleWorking(const Input::Cursor& cursor) const;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when middle mouse button was pressed on this
		void OnCursorMiddleMousePressed(const Input::Cursor& cursor) override;

		// It is called when middle mouse button stay down on this
		void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when middle mouse button was released (only when middle mouse button pressed this at previous time)
		void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// It is called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key) override;

		// It is called when changed selected actors from this
		void OnActorsSelectedFromThis();

		// Redraws scene texture
		void RedrawContent();

		// Draws actors drawables components
		void DrawActors();

		// Draws selection on actors
		void DrawSelection();

		// Binds to actors tree selection window
		void BindActorsTree();

		// It is called when actors tree selection changed
		void OnTreeSelectionChanged(Vector<Actor*> selectedObjects);

		// Updates top selected actors
		void UpdateTopSelectedActors();

		// Returns actor's index in hierarchy
		int GetActorIdx(Actor* actor);

		// It is called when actors was changed
		void OnSceneChanged(ActorsVec actors);

		// Clears actors selection
		void ClearSelectionWithoutAction(bool sendSelectedMessage = true);

		// Selects actors
		void SelectActorsWithoutAction(ActorsVec actors, bool additive = true);

		// Selects actor
		void SelectActorWithoutAction(Actor* actor, bool additive = true);

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		friend class DeleteActorsAction;
		friend class SelectActorsAction;
		friend class SelectionTool;
		friend class SceneDragHandle;
		friend class SceneWindow;
		friend class TreeWindow;
		friend class CreateActorsAction;
	};

	template<typename _type>
	void SceneEditScreen::SelectTool()
	{
		if (mEnabledTool && mEnabledTool->GetType() == TypeOf(_type))
			return;

		if (mEnabledTool)
			mEnabledTool->OnDisabled();

		mEnabledTool = mTools.FindMatch([&](auto x) { return x->GetType() == TypeOf(_type); });

		if (mEnabledTool)
			mEnabledTool->OnEnabled();
	}
}

CLASS_BASES_META(Editor::SceneEditScreen)
{
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::Singleton<SceneEditScreen>);
	BASE_CLASS(Editor::UIScrollView);
}
END_META;
CLASS_FIELDS_META(Editor::SceneEditScreen)
{
	PUBLIC_FIELD(onSelectionChanged);
	PROTECTED_FIELD(mSelectedActorColor);
	PROTECTED_FIELD(mMultiSelectedActorColor);
	PROTECTED_FIELD(mActorMinimalSelectionSize);
	PROTECTED_FIELD(mActorsTree);
	PROTECTED_FIELD(mSelectedActors);
	PROTECTED_FIELD(mTopSelectedActors);
	PROTECTED_FIELD(mSelectedFromThis);
	PROTECTED_FIELD(mTools);
	PROTECTED_FIELD(mEnabledTool);
	PROTECTED_FIELD(mDragHandles);
}
END_META;
CLASS_METHODS_META(Editor::SceneEditScreen)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(Vec2F, ScreenToScenePoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, SceneToScreenPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, ScreenToSceneVector, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, SceneToScreenVector, const Vec2F&);
	PUBLIC_FUNCTION(void, DrawActorSelection, Actor*, const Color4&);
	PUBLIC_FUNCTION(void, SelectActors, ActorsVec, bool);
	PUBLIC_FUNCTION(void, SelectActor, Actor*, bool);
	PUBLIC_FUNCTION(void, SelectAllActors);
	PUBLIC_FUNCTION(void, ClearSelection);
	PUBLIC_FUNCTION(const ActorsVec&, GetSelectedActors);
	PUBLIC_FUNCTION(const ActorsVec&, GetTopSelectedActors);
	PUBLIC_FUNCTION(const Color4&, GetSingleActorSelectionColor);
	PUBLIC_FUNCTION(const Color4&, GetManyActorsSelectionColor);
	PUBLIC_FUNCTION(void, OnSceneChanged);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, InitializeTools, const Type*);
	PROTECTED_FUNCTION(bool, IsHandleWorking, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnActorsSelectedFromThis);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawActors);
	PROTECTED_FUNCTION(void, DrawSelection);
	PROTECTED_FUNCTION(void, BindActorsTree);
	PROTECTED_FUNCTION(void, OnTreeSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, UpdateTopSelectedActors);
	PROTECTED_FUNCTION(int, GetActorIdx, Actor*);
	PROTECTED_FUNCTION(void, OnSceneChanged, ActorsVec);
	PROTECTED_FUNCTION(void, ClearSelectionWithoutAction, bool);
	PROTECTED_FUNCTION(void, SelectActorsWithoutAction, ActorsVec, bool);
	PROTECTED_FUNCTION(void, SelectActorWithoutAction, Actor*, bool);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
}
END_META;
