#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Camera.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/UI/ScrollView.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class SceneEditableObject;
	class DrawableComponent;
	class Tree;
}

// Editor scene screen accessor macros
#define o2EditorSceneScreen SceneEditScreen::Instance()

namespace Editor
{
	class SceneDragHandle;
	class IEditTool;
	class SceneTree;

	// --------------------
	// Scene editing screen
	// --------------------
	class SceneEditScreen: public DragDropArea, public KeyboardEventsListener,
		public Singleton<SceneEditScreen>, public ScrollView
	{
	public:
		Function<void(const Vector<SceneEditableObject*>&)> onSelectionChanged; // Actors selection change event

		// Default constructor
		SceneEditScreen();

		// Copy-constructor
		SceneEditScreen(const SceneEditScreen& other);

		// Destructor
		~SceneEditScreen();

		// Draws widget
		void Draw() override;

		// It is called when required to redraw content. Sets flag and redraws at next frame
		void NeedRedraw();

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

		// Draws object selection
		void DrawObjectSelection(SceneEditableObject* object, const Color4& color);

		// Selects objects
		void SelectObjects(Vector<SceneEditableObject*> objects, bool additive = true);

		// Selects object
		void SelectObject(SceneEditableObject* object, bool additive = true);

		// Selects all objects
		void SelectAllObjects();

		// Clears objects selection
		void ClearSelection();

		// Selects tool with type
		template<typename _type>
		void SelectTool();

		// Returns selected objects array
		const Vector<SceneEditableObject*>& GetSelectedObjects() const;

		// Returns top selected objects in hierarchy
		const Vector<SceneEditableObject*>& GetTopSelectedObjects() const;

		// Returns color for single selected object
		const Color4& GetSingleObjectSelectionColor() const;

		// Return color for multiple selected objects
		const Color4& GetManyObjectsSelectionColor() const;

		// It is called when scene was changed and needs to redraw
		void OnSceneChanged();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(SceneEditScreen);

	protected:
		Color4 mSelectedObjectColor = Color4(220, 220, 220, 255);      // Selected object color
		Color4 mMultiSelectedObjectColor = Color4(220, 220, 220, 100); // Selected object color
		float  mObjectMinimalSelectionSize = 10.0f;                    // Minimal object size on pixels

		SceneTree* mSceneTree; // Pointer to object tree widget

		Vector<SceneEditableObject*> mSelectedObjects;          // Current selected objects
		Vector<SceneEditableObject*> mTopSelectedObjects;       // Current selected objects most top in hierarchy
		bool                         mSelectedFromThis = false; // True if selection changed from this, needs to break recursive selection update

		Vector<IEditTool*> mTools;                 // Available tools
		IEditTool*         mEnabledTool = nullptr; // Current enabled tool

		Vector<SceneDragHandle*> mDragHandles; // Dragging handles array
		
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

		// It is called when changed selected objects from this
		void OnObjectsSelectedFromThis();

		// Redraws scene texture
		void RedrawContent();

		// Draws objects drawables components
		void DrawObjects();

		// Draws selection on objects
		void DrawSelection();

		// Binds to scene tree selection window
		void BindSceneTree();

		// It is called when scene tree selection changed
		void OnTreeSelectionChanged(Vector<SceneEditableObject*> selectedObjects);

		// Updates top selected objects
		void UpdateTopSelectedObjects();

		// It is called when objects was changed
		void OnSceneChanged(Vector<SceneEditableObject*> objects);

		// Clears objects selection
		void ClearSelectionWithoutAction(bool sendSelectedMessage = true);

		// Selects objects
		void SelectObjectsWithoutAction(Vector<SceneEditableObject*> objects, bool additive = true);

		// Selects object
		void SelectObjectWithoutAction(SceneEditableObject* object, bool additive = true);

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		friend class DeleteAction;
		friend class SelectAction;
		friend class SelectionTool;
		friend class SceneDragHandle;
		friend class SceneWindow;
		friend class TreeWindow;
		friend class CreateAction;
	};

	template<typename _type>
	void SceneEditScreen::SelectTool()
	{
		if (mEnabledTool && mEnabledTool->GetType() == TypeOf(_type))
			return;

		if (mEnabledTool)
			mEnabledTool->OnDisabled();

		mEnabledTool = mTools.FindMatchOrDefault([&](auto x) { return x->GetType() == TypeOf(_type); });

		if (mEnabledTool)
			mEnabledTool->OnEnabled();
	}
}

CLASS_BASES_META(Editor::SceneEditScreen)
{
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::Singleton<SceneEditScreen>);
	BASE_CLASS(Editor::ScrollView);
}
END_META;
CLASS_FIELDS_META(Editor::SceneEditScreen)
{
	PUBLIC_FIELD(onSelectionChanged);
	PROTECTED_FIELD(mSelectedObjectColor).DEFAULT_VALUE(Color4(220, 220, 220, 255));
	PROTECTED_FIELD(mMultiSelectedObjectColor).DEFAULT_VALUE(Color4(220, 220, 220, 100));
	PROTECTED_FIELD(mObjectMinimalSelectionSize).DEFAULT_VALUE(10.0f);
	PROTECTED_FIELD(mSceneTree);
	PROTECTED_FIELD(mSelectedObjects);
	PROTECTED_FIELD(mTopSelectedObjects);
	PROTECTED_FIELD(mSelectedFromThis).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mTools);
	PROTECTED_FIELD(mEnabledTool).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mDragHandles);
}
END_META;
CLASS_METHODS_META(Editor::SceneEditScreen)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, NeedRedraw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(Vec2F, ScreenToScenePoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, SceneToScreenPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, ScreenToSceneVector, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, SceneToScreenVector, const Vec2F&);
	PUBLIC_FUNCTION(void, DrawObjectSelection, SceneEditableObject*, const Color4&);
	PUBLIC_FUNCTION(void, SelectObjects, Vector<SceneEditableObject*>, bool);
	PUBLIC_FUNCTION(void, SelectObject, SceneEditableObject*, bool);
	PUBLIC_FUNCTION(void, SelectAllObjects);
	PUBLIC_FUNCTION(void, ClearSelection);
	PUBLIC_FUNCTION(const Vector<SceneEditableObject*>&, GetSelectedObjects);
	PUBLIC_FUNCTION(const Vector<SceneEditableObject*>&, GetTopSelectedObjects);
	PUBLIC_FUNCTION(const Color4&, GetSingleObjectSelectionColor);
	PUBLIC_FUNCTION(const Color4&, GetManyObjectsSelectionColor);
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
	PROTECTED_FUNCTION(void, OnObjectsSelectedFromThis);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawObjects);
	PROTECTED_FUNCTION(void, DrawSelection);
	PROTECTED_FUNCTION(void, BindSceneTree);
	PROTECTED_FUNCTION(void, OnTreeSelectionChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, UpdateTopSelectedObjects);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, ClearSelectionWithoutAction, bool);
	PROTECTED_FUNCTION(void, SelectObjectsWithoutAction, Vector<SceneEditableObject*>, bool);
	PROTECTED_FUNCTION(void, SelectObjectWithoutAction, SceneEditableObject*, bool);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
}
END_META;
