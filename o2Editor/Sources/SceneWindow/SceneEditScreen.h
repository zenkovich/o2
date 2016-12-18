#pragma once

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
		public Singleton<SceneEditScreen>, public IObject
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

		// Sets size
		void SetRect(const RectF& rect);

		// Returns size
		RectF GetRect() const;

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

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		IOBJECT(SceneEditScreen);

	protected:
		RectF          mRectangle;                                            // Current position rectangle
		Camera         mViewCamera;     					                  // Scene view camera
		float          mViewCameraTargetScale = 1.0f;		                  // Camera target scale
		float          mViewCameraScaleSence = 0.1f / 120.0f;                 // Camera scale sense
		float          mViewCameraScaleElasticyCoef = 30.0f;                  // Scale smoothing coefficient
		Vec2F          mViewCameraTargetPos;				                  // Target camera position
		Vec2F          mViewCameraVelocity;					                  // Camera velocity
		float          mViewCameraPosElasticyCoef = 40.0f;	                  // Camera dragging smoothing coefficient
		float          mViewCameraVelocityDampingCoef = 10.0f;                // Camera velocity damping coefficient
		float          mViewCameraMinScale = 0.001f;		                  // Minimal camera scale
		float          mViewCameraMaxScale = 10000.0f;		                  // Maximal camera scale

		Color4         mBackColor = Color4(170, 170, 170, 255);	              // Scene back color
		Color4         mGridColor = Color4(190, 190, 190, 255);	              // Scene grid color
		Color4         mSelectedActorColor = Color4(220, 220, 220, 255);      // Selected actor color
		Color4         mMultiSelectedActorColor = Color4(220, 220, 220, 100); // Selected actor color
		float          mActorMinimalSelectionSize = 10.0f;                    // Minimal actor size on pixels

		Basis          mSceneToScreenTransform;					              // Scene to screen transformation
		Basis          mScreenToSceneTransform;					              // Screen to scene transformation

		TextureRef     mRenderTarget;							              // Scene render target
		Sprite*        mRenderTargetSprite;						              // Render target using sprite
		bool           mNeedRedraw;								              // If scene was changed and needs to redraw
		float          mDrawDepth;								              // Drawing depth

		UIActorsTree*  mActorsTree;								              // Pointer to actors tree widget
		ActorsVec      mSelectedActors;							              // Current selected actors
		ActorsVec      mTopSelectedActors;                                    // Current selected actors most top in hierarchy
		bool           mSelectedFromThis = false;				              // True if selection changed from this, needs to break recursive selection update

		ToolsVec       mTools;									              // Available tools
		IEditTool*     mEnabledTool = nullptr;					              // Current enabled tool

		DragHandlesVec                         mDragHandles; 			      // Dragging handles array
		Dictionary<CursorId, SceneDragHandle*> mPressedHandles;               // Pressed handles for all pressed cursors
		CursorAreaEventsListener*              mRightButtonPressedHandle;     // Right mouse button pressed handle
		CursorAreaEventsListener*              mMiddleButtonPressedHandle;    // Middle mouse button pressed handle
		Dictionary<CursorId, SceneDragHandle*> mUnderCursorHandles;           // Under cursor handles for each cursor
		Dictionary<CursorId, SceneDragHandle*> mLastUnderCursorHandles;       // Under cursor handles for each cursor on last frame

	protected:
		// Initializes tools
		void InitializeTools(const Type* toolType = nullptr);

		// Updates handles interaction
		void UpdateHandles();

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

		// Updates camera
		void UpdateCamera(float dt);

		// Redraws scene texture
		void RedrawScene();

		// Draws scene grid
		void DrawGrid();

		// Draws actors drawables components
		void DrawActors();

		// Draws selection on actors
		void DrawSelection();

		// Updates scene to screen and screen and scene transformations
		void UpdateSceneScreenTransforms();

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
		void ClearSelectionWithoutAction();

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