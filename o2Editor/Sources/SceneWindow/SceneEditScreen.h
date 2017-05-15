#pragma once

#include "Core/UI/ScrollView.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "Render/Camera.h"
#include "Render/IDrawable.h"
#include "Render/TextureRef.h"
#include "Utils/DragAndDrop.h"
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
		void Draw();

		// Updates drawables, states and widget
		void Update(float dt);

		// Returns is listener scrollable
		bool IsScrollable() const;

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

		// Calls when scene was changed and needs to redraw
		void OnSceneChanged();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

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

		DragHandlesVec                         mDragHandles; 			      // Dragging handles array
		
	protected:
		// Initializes tools
		void InitializeTools(const Type* toolType = nullptr);

		// Returns true if some handle hovered or pressed by cursor
		bool IsHandleWorking(const Input::Cursor& cursor) const;

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor);

		// Calls when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Calls when middle mouse button was pressed on this
		void OnCursorMiddleMousePressed(const Input::Cursor& cursor);

		// Calls when middle mouse button stay down on this
		void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor);

		// Calls when middle mouse button was released (only when middle mouse button pressed this at previous time)
		void OnCursorMiddleMouseReleased(const Input::Cursor& cursor);

		// Calls when scrolling
		void OnScrolled(float scroll);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// Calls when key was released
		void OnKeyReleased(const Input::Key& key);

		// Calls when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// Calls when changed selected actors from this
		void OnActorsSelectedFromThis();

		// Redraws scene texture
		void RedrawContent();

		// Draws actors drawables components
		void DrawActors();

		// Draws selection on actors
		void DrawSelection();

		// Binds to actors tree selection window
		void BindActorsTree();

		// Calls when actors tree selection changed
		void OnTreeSelectionChanged(Vector<Actor*> selectedObjects);

		// Updates top selected actors
		void UpdateTopSelectedActors();

		// Returns actor's index in hierarchy
		int GetActorIdx(Actor* actor);

		// Calls when actors was changed
		void OnSceneChanged(ActorsVec actors);

		// Clears actors selection
		void ClearSelectionWithoutAction(bool sendSelectedMessage = true);

		// Selects actors
		void SelectActorsWithoutAction(ActorsVec actors, bool additive = true);

		// Selects actor
		void SelectActorWithoutAction(Actor* actor, bool additive = true);

		// Calls when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group);

		friend class DeleteActorsAction;
		friend class SelectionAction;
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
 