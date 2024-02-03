#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Camera.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/UI/ScrollView.h"
#include "o2/Core/Ref.h"
#include "o2/Core/WeakRef.h"
#include "o2/Core/MMake.h"

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
	class IEditTool;
	class SceneDragHandle;
	class SceneEditorLayer;
	class SceneHierarchyTree;

	// --------------------
	// Scene editing screen
	// --------------------
	class SceneEditScreen: public DragDropArea, public KeyboardEventsListener,
		public Singleton<SceneEditScreen>, public ScrollView
	{
	public:
		Function<void(const Vector<Ref<SceneEditableObject>>&)> onSelectionChanged; // Actors selection change event

		// Default constructor
		SceneEditScreen();

		// Copy-constructor
		SceneEditScreen(const SceneEditScreen& other);

		// Destructor
		~SceneEditScreen();

		// Draws widget
		void Draw() override;

		// Called when required to redraw content. Sets flag and redraws at next frame
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
		void DrawObjectSelection(const Ref<SceneEditableObject>& object, const Color4& color);

		// Selects objects
		void SelectObjects(const Vector<Ref<SceneEditableObject>>& objects, bool additive = true);

		// Selects object
		void SelectObject(const Ref<SceneEditableObject>& object, bool additive = true);

		// Selects all objects
		void SelectAllObjects();

		// Clears objects selection
		void ClearSelection();

		// Returns left top widgets container, can be used for tools additional controls
		Ref<HorizontalLayout> GetLeftTopWidgetsContainer();

		// Returns right top widgets container, can be used for tools additional controls
		Ref<HorizontalLayout> GetRightTopWidgetsContainer();

		// Returns left bottom widgets container, can be used for tools additional controls
		Ref<HorizontalLayout> GetLeftBottomWidgetsContainer();

		// Returns right bottom widgets container, can be used for tools additional controls
		Ref<HorizontalLayout> GetRightBottomWidgetsContainer();

		// Adds editable layer
		void AddEditorLayer(const Ref<SceneEditorLayer>& layer);

		// Removes editable layer
		void RemoveEditorLayer(const Ref<SceneEditorLayer>& layer);

		// Sets layers with name enabled
		void SetLayerEnabled(const String& name, bool enabled);

		// Returns is layer enabled
		bool IsLayerEnabled(const String& name) const;

		// Selects tool with type
		template<typename _type>
		void SelectTool();

		// Selects tool
		void SelectTool(const Ref<IEditTool>& tool);

		// Returns selected tool
		Ref<IEditTool> GetSelectedTool() const;

		// Adds tool
		void AddTool(const Ref<IEditTool>& tool);

		// Removes tool
		void RemoveTool(const Ref<IEditTool>& tool);

		// Returns tool by type, or null if it doesn't exists
		template<typename _type>
		Ref<_type> GetTool();

		// Returns all registered tools
		const Vector<Ref<IEditTool>>& GetTools() const;

		// Returns selected objects array
		const Vector<Ref<SceneEditableObject>>& GetSelectedObjects() const;

		// Returns top selected objects in hierarchy
		const Vector<Ref<SceneEditableObject>>& GetTopSelectedObjects() const;

		// Returns color for single selected object
		const Color4& GetSingleObjectSelectionColor() const;

		// Return color for multiple selected objects
		const Color4& GetManyObjectsSelectionColor() const;

		// Called when scene was changed and needs to redraw
		void OnSceneChanged();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(SceneEditScreen);

	protected:
		Color4 mSelectedObjectColor = Color4(220, 220, 220, 255);      // Selected object color
		Color4 mMultiSelectedObjectColor = Color4(220, 220, 220, 100); // Selected object color
		float  mObjectMinimalSelectionSize = 10.0f;                    // Minimal object size on pixels

		Vector<Ref<SceneEditableObject>> mSelectedObjects;          // Current selected objects
		Vector<Ref<SceneEditableObject>> mTopSelectedObjects;       // Current selected objects most top in hierarchy
		bool                         mSelectedFromThis = false; // True if selection changed from this, needs to break recursive selection update

		Vector<Ref<IEditTool>> mTools;                 // Available tools
		Ref<IEditTool>         mEnabledTool;
	};
}{
	class SceneEditorLayer;
	class SceneWindow;
	class TreeWindow;
	
	class SceneEditor : public IEditorTool, public ISelectableDropTarget, public IDragTarget
	{
	public:
		SceneEditor();
		virtual ~SceneEditor() = default;

		// Initializes the scene editor
		void Initialize() override;

		// Sets the scene window for the editor
		void SetSceneWindow(SceneWindow* sceneWindow);

		// Sets the tree window for the editor
		void SetTreeWindow(TreeWindow* treeWindow);

		// Adds an editable layer to the editor
		void AddEditorLayer(SceneEditorLayer* layer);

		// Removes an editable layer from the editor
		void RemoveEditorLayer(SceneEditorLayer* layer);

		// Returns the list of editable layers in the editor
		const Vector<Ref<SceneEditorLayer>>& GetEditorLayers() const;

		// Enables or disables the specified layer
		void SetEditorLayerEnabled(const String& layerName, bool enabled);

		// Returns whether the specified layer is enabled or disabled
		bool IsEditorLayerEnabled(const String& layerName) const;

		// Updates the scene editor
		void Update() override;

		// Called when the editor tool becomes the active tool
		void OnActivated() override;

		// Called when the editor tool is deactivated
		void OnDeactivated() override;

		// Called when the editor tool is selected or deselected
		void OnSelectedChanged(bool selected) override;

		// Handler for mouse press events
		bool OnMousePress(const Input::MouseData& mouseData) override;

		// Handler for mouse release events
		bool OnMouseRelease(const Input::MouseData& mouseData) override;

		// Handler for mouse move events
		bool OnMouseMove(const Input::MouseData& mouseData) override;

		// Handler for mouse wheel events
		bool OnMouseWheel(float wheelDelta) override;

		// Handler for key press events
		bool OnKeyPress(const Input::KeyData& keyData) override;

		// Handler for key release events
		bool OnKeyRelease(const Input::KeyData& keyData) override;

		// Handler for key repeat events
		bool OnKeyRepeat(const Input::KeyData& keyData) override;

		// Handler for scene changed events
		void OnSceneChanged(const Ref<Scene>& scene) override;

		// Handler for object selected events
		void OnObjectSelected(const Ref<SceneObject>& sceneObject) override;

		// Handler for object deselected events
		void OnObjectDeselected(const Ref<SceneObject>& sceneObject) override;

		// Handler for object hovering events
		void OnObjectHover(const Ref<SceneObject>& sceneObject) override;

		// Handler for object stop hovering events
		void OnObjectStopHover(const Ref<SceneObject>& sceneObject) override;

		// Handler for object pressed events
		void OnObjectPress(const Ref<SceneObject>& sceneObject) override;

		// Handler for object release events
		void OnObjectRelease(const Ref<SceneObject>& sceneObject) override;

		// Handler for object drag enter events
		void OnObjectDragEnter(const Ref<SceneObject>& sceneObject) override;

		// Handler for object drag leave events
		void OnObjectDragLeave(const Ref<SceneObject>& sceneObject) override;

		// Handler for object drag events
		void OnObjectDrag(const Ref<SceneObject>& sceneObject) override;

		// Handler for object drop events
		void OnObjectDrop(const Ref<SceneObject>& sceneObject) override;

		// Returns the current active layer
		const Ref<SceneEditorLayer>& GetActiveLayer() const;

		// Returns the last hovered object
		const Ref<SceneObject>& GetHoveredObject() const;

		// Returns the last pressed object
		const Ref<SceneObject>& GetPressedObject() const;

		// Returns the last dragged object
		const Ref<SceneObject>& GetDraggedObject() const;

		// Returns a list of selected objects
		const Vector<Ref<SceneObject>>& GetSelectedObjects() const;

		// Returns the list of top selected objects
		const Vector<Ref<SceneObject>>& GetTopSelectedObjects() const;

		// Returns the number of selected objects
		uint32_t GetSelectedObjectCount() const;

	private:
		// Initializes the tools
		void InitializeTools();

		// Creates and configures the widgets container with the specified base corner
		Ref<HorizontalLayout> InitializeWidgetsContainer(BaseCorner baseCorner);

		// Returns true if some handle is being dragged or hovered by cursor
		bool IsHandleWorking(const Ref<Input::Cursor>& cursor) const;

		// Called when the cursor is pressed on this
		void OnCursorPressed(const Ref<Input::Cursor>& cursor) override;

		// Called when the cursor is released (only when the cursor is pressed on this at the previous time)
		void OnCursorReleased(const Ref<Input::Cursor>& cursor) override;

		// Called when the cursor press was interrupted (e.g. by scrolling the scroll area or some other event)
		void OnCursorPressInterrupted(const Ref<Input::Cursor>& cursor) override;

		// Called when the cursor stays down during the frame
		void OnCursorStillDown(const Ref<Input::Cursor>& cursor) override;

		// Called when the cursor is moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Ref<Input::Cursor>& cursor) override;

		// Called when the cursor enters this object
		void OnCursorEnter(const Ref<Input::Cursor>& cursor) override;

		// Called when the cursor exits this object
		void OnCursorExit(const Ref<Input::Cursor>& cursor) override;

		// Called when the right mouse button is pressed on this
		void OnCursorRightMousePressed(const Ref<Input::Cursor>& cursor) override;

		// Called when the right mouse button is held down on this
		void OnCursorRightMouseHeld(const Ref<Input::Cursor>& cursor) override;

		// Called when the right mouse button is released (only when the right mouse button was pressed on this at the previous time)
		void OnCursorRightMouseReleased(const Ref<Input::Cursor>& cursor) override;

		// Called when the middle mouse button is pressed on this
		void OnCursorMiddleMousePressed(const Ref<Input::Cursor>& cursor) override;

		// Called when the middle mouse button is held down on this
		void OnCursorMiddleMouseHeld(const Ref<Input::Cursor>& cursor) override;

		// Called when the middle mouse button is released (only when the middle mouse button was pressed on this at the previous time)
		void OnCursorMiddleMouseReleased(const Ref<Input::Cursor>& cursor) override;

		// Called when scrolling
		void OnScrolled(float scroll) override;

		// Called when a key is pressed
		void OnKeyPressed(const Ref<Input::Keyboard>& keyboard) override;

		// Called when a key is released
		void OnKeyReleased(const Ref<Input::Keyboard>& keyboard) override;

		// Called when a key is held down during the frame
		void OnKeyHeld(const Ref<Input::Keyboard>& keyboard) override;

		// Called when the selection of objects from this has changed
		void OnSelectionChangedFromThis();

		// Redraws the scene texture
		void RedrawContent() override;

		// Draws the drawable components of objects
		void DrawObjects();

		// Draws the selection on objects
		void DrawSelection();

		// Binds to the scene tree selection window
		void BindSceneTree();

		// Called when the selection in the tree window has changed
		void OnTreeSelectionChanged(const Vector<Ref<SceneEditableObject>>& selectedObjects);

		// Updates the top selected objects
		void UpdateTopSelectedObjects();

		// Called when objects have been changed
		void OnObjectsChanged(const Vector<Ref<SceneEditableObject>>& objects);

		// Clears the object selection without triggering any action
		void ClearSelectionWithoutAction(bool sendSelectedMessage = true);

		// Selects the specified objects without triggering any action
		void SelectObjectsWithoutAction(const Vector<Ref<SceneEditableObject>>& objects, bool additive = true);

		// Selects the specified object without triggering any action
		void SelectObjectWithoutAction(const Ref<SceneEditableObject>& object, bool additive = true);

		// Called when a group of selectable and draggable objects is dropped on this
		void OnGroupDropped(Ref<ISelectableDraggableObjectsGroup> group) override;

		// Called when a group of draggable objects enters the area of this
		void OnGroupDragEnter(Ref<ISelectableDraggableObjectsGroup> group) override;

		// Called when a group of draggable objects is dragged over this
		void OnGroupDragOver(Ref<ISelectableDraggableObjectsGroup> group) override;

		// Called when a group of draggable objects exits the area of this
		void OnGroupDragExit(Ref<ISelectableDraggableObjectsGroup> group) override;

		// Returns whether the input of this is transparent
		bool IsInputTransparent() const override;

		friend class DeleteAction;
		friend class SelectAction;
		friend class SelectionTool;
		friend class SceneDragHandle;
		friend class SceneWindow;
		friend class TreeWindow;
		friend class CreateAction;
	};

	template <typename T>
	class DynamicCast
	{
	public:
		template <typename U>
		static Ref<T> Cast(const Ref<U>& object)
		{
			return Ref<T>(dynamic_cast<T*>(object.get()), false);
		}
	};

	// Helper function to create a Ref object
	template <typename T, typename... Args>
	Ref<T> mmake(Args&&... args)
	{
		return Ref<T>(new T(std::forward<Args>(args)...));
	}
}

#include "o2Editor/EditorTools/SceneEditor/SceneEditor.inl"{
    void, OnCursorLeave, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMouseMove, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyboardKey, KeyboardKey, KeyboardKeyState);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyboardKeyChar, const String&, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMouseScroll, const Vec2F&, float);
    FUNCTION().PROTECTED().SIGNATURE(Drawable*, InitializeDragHandle, DragHandleType, BaseCorner);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeEditorLayers);
    FUNCTION().PROTECTED().SIGNATURE(void, OnToolActivated, IEditTool*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnToolDeactivated, IEditTool*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSelectionChanged);
    FUNCTION().PROTECTED().DEFAULT_VALUE(Editor::SceneEditorLayer).SIGNATURE(Vector<IEditTool*>, GetInternalTools);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsRectSelectionEnabled);
    FUNCTION().PROTECTED().DEFAULT_VALUE(Float32Rect()).SIGNATURE(void, SetScreenRectangleSelection);
    FUNCTION().PROTECTED().DEFAULT_VALUE(Float32Rect()).SIGNATURE(Float32Rect, GetScreenRectangleSelection);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateSelectedObjectsHandlePosition);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawRectSelection);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragHandleMoved, const DragHandleEvent& );
}

// Ref and WeakRef classes

template<typename _type>
class Ref
{
public:
    Ref() : mPtr(nullptr) {}

    explicit Ref(_type* ptr) : mPtr(ptr) {}

    template<typename _otherType,
             typename = std::enable_if_t<std::is_convertible_v<_otherType*, _type*>>>
    Ref(const Ref<_otherType>& other) : mPtr(other.mPtr) {}

    Ref(const Ref& other) : mPtr(other.mPtr) {}

    ~Ref() {}
    
    Ref& operator=(const Ref& other)
    {
        mPtr = other.mPtr;
        return *this;
    }
    
    _type* operator->() const
    {
        return mPtr;
    }

    _type* Get() const
    {
        return mPtr;
    }

    explicit operator bool() const
    {
        return mPtr != nullptr;
    }

private:
    _type* mPtr;
};

template<typename _type>
class WeakRef
{
public:
    WeakRef() : mPtr(nullptr) {}

    explicit WeakRef(_type* ptr) : mPtr(ptr) {}

    template<typename _otherType,
             typename = std::enable_if_t<std::is_convertible_v<_otherType*, _type*>>>
    WeakRef(const WeakRef<_otherType>& other) : mPtr(other.mPtr) {}

    WeakRef(const WeakRef& other) : mPtr(other.mPtr) {}

    ~WeakRef() {}
    
    WeakRef& operator=(const WeakRef& other)
    {
        mPtr = other.mPtr;
        return *this;
    }
    
    _type* operator->() const
    {
        return mPtr;
    }

    _type* Get() const
    {
        return mPtr;
    }
    
    explicit operator bool() const
    {
        return mPtr != nullptr;
    }

private:
    _type* mPtr;
};

// mmake function

template<typename _type, typename... _args>
Ref<_type> mmake(_args&&... args)
{
    return Ref<_type>(new _type(std::forward<_args>(args)...));
}

// DynamicCast function

template<typename _targetType, typename _sourceType>
Ref<_targetType> DynamicCast(Ref<_sourceType> ptr)
{
    return Ref<_targetType>(dynamic_cast<_targetType*>(ptr.Get()));
}

// --- META ---

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
    FIELD().PUBLIC().NAME(onSelectionChanged);
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(220, 220, 220, 255)).NAME(mSelectedObjectColor);
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(220, 220, 220, 100)).NAME(mMultiSelectedObjectColor);
    FIELD().PROTECTED().DEFAULT_VALUE(10.0f).NAME(mObjectMinimalSelectionSize);
    FIELD().PROTECTED().NAME(mSelectedObjects);
    FIELD().PROTECTED().NAME(mTopSelectedObjects);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mSelectedFromThis);
    FIELD().PROTECTED().NAME(mTools);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEnabledTool);
    FIELD().PROTECTED().NAME(mDragHandles);
    FIELD().PROTECTED().NAME(mEditorLayers);
    FIELD().PROTECTED().NAME(mEditorLayersEnabled);
    FIELD().PROTECTED().DEFAULT_VALUE(WeakRef<IEditableWidgetContainer>()).NAME(mLeftTopWidgetsContainer);
    FIELD().PROTECTED().DEFAULT_VALUE(WeakRef<IEditableWidgetContainer>()).NAME(mRightTopWidgetsContainer);
    FIELD().PROTECTED().DEFAULT_VALUE(WeakRef<IEditableWidgetContainer>()).NAME(mLeftBottomWidgetsContainer);
    FIELD().PROTECTED().DEFAULT_VALUE(WeakRef<IEditableWidgetContainer>()).NAME(mRightBottomWidgetsContainer);
}
END_META;
CLASS_METHODS_META(Editor::SceneEditScreen)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SceneEditScreen&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, NeedRedraw);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsScrollable);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToScenePoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, SceneToScreenPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToSceneVector, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, SceneToScreenVector, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, DrawObjectSelection, Ref<SceneEditableObject>, const Color4&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectObjects, Vector<Ref<SceneEditableObject>>, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectObject, Ref<SceneEditableObject>, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAllObjects);
    FUNCTION().PUBLIC().SIGNATURE(void, ClearSelection);
    FUNCTION().PUBLIC().SIGNATURE(Ref<HorizontalLayout>, GetLeftTopWidgetsContainer);
    FUNCTION().PUBLIC().SIGNATURE(Ref<HorizontalLayout>, GetRightTopWidgetsContainer);
    FUNCTION().PUBLIC().SIGNATURE(Ref<HorizontalLayout>, GetLeftBottomWidgetsContainer);
    FUNCTION().PUBLIC().SIGNATURE(Ref<HorizontalLayout>, GetRightBottomWidgetsContainer);
    FUNCTION().PUBLIC().SIGNATURE(void, AddEditorLayer, Ref<SceneEditorLayer>);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveEditorLayer, Ref<SceneEditorLayer>);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLayerEnabled, const String&, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsLayerEnabled, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectTool, const Ref<IEditTool>&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<IEditTool>, GetSelectedTool);
    FUNCTION().PUBLIC().SIGNATURE(void, AddTool, Ref<IEditTool>);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTool, Ref<IEditTool>);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<IEditTool>>&, GetTools);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<SceneEditableObject>>&, GetSelectedObjects);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<SceneEditableObject>>&, GetTopSelectedObjects);
    FUNCTION().PUBLIC().SIGNATURE(const Color4&, GetSingleObjectSelectionColor);
    FUNCTION().PUBLIC().SIGNATURE(const Color4&, GetManyObjectsSelectionColor);
    FUNCTION().PUBLIC().SIGNATURE(void, OnSceneChanged);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeTools);
    FUNCTION().PROTECTED().SIGNATURE(Ref<HorizontalLayout>, InitializeWidgetsContainer, BaseCorner);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsHandleWorking, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorLeave, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMouseMove, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyboardKey, KeyboardKey, KeyboardKeyState);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyboardKeyChar, const String&, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMouseScroll, const Vec2F&, float);
    FUNCTION().PROTECTED().SIGNATURE(Ref<Drawable>, InitializeDragHandle, DragHandleType, BaseCorner);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeEditorLayers);
    FUNCTION().PROTECTED().SIGNATURE(void, OnToolActivated, Ref<IEditTool>);
    FUNCTION().PROTECTED().SIGNATURE(void, OnToolDeactivated, Ref<IEditTool>);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSelectionChanged);
    FUNCTION().PROTECTED().DEFAULT_VALUE(Vector<Ref<IEditTool>>).SIGNATURE(Vector<Ref<IEditTool>>, GetInternalTools);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsRectSelectionEnabled);
    FUNCTION().PROTECTED().DEFAULT_VALUE(Float32Rect()).SIGNATURE(void, SetScreenRectangleSelection);
    FUNCTION().PROTECTED().DEFAULT_VALUE(Float32Rect()).SIGNATURE(Float32Rect, GetScreenRectangleSelection);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateSelectedObjectsHandlePosition);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawRectSelection);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragHandleMoved, const Ref<DragHandleEvent>& );
}#include <memory>
#include <vector>

template<typename T>
class Ref {
public:
    Ref() : ptr(nullptr) {}

    explicit Ref(T* p) : ptr(p) {}

    template<typename U>
    Ref(const Ref<U>& r) : ptr(r.ptr) {}

    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    template<typename U>
    Ref<U> static mmake() {
        return Ref<U>(new U());
    }

private:
    T* ptr;
};

template<typename T>
class WeakRef {
public:
    WeakRef() : ptr(nullptr) {}

    explicit WeakRef(T* p) : ptr(p) {}

    template<typename U>
    WeakRef(const WeakRef<U>& r) : ptr(r.ptr) {}

    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

private:
    T* ptr;
};

template<typename T>
Ref<T> DynamicCast(const Ref<T>& ptr) {
    return Ref<T>(dynamic_cast<T*>(ptr.get()));
}

class Input::Cursor {};

class Input::Key {};

template<typename T>
using Vector = std::vector<T>;

class SceneEditableObject {};

class ISelectableDragableObjectsGroup {};

class FUNCTION {
public:
    FUNCTION() {}

    FUNCTION& PROTECTED() {
        return *this;
    }
};

// --- BEGIN META ---
void OnCursorExit(const Ref<const Input::Cursor>&);
void OnCursorRightMousePressed(const Ref<const Input::Cursor>&);
void OnCursorRightMouseStayDown(const Ref<const Input::Cursor>&);
void OnCursorRightMouseReleased(const Ref<const Input::Cursor>&);
void OnCursorMiddleMousePressed(const Ref<const Input::Cursor>&);
void OnCursorMiddleMouseStayDown(const Ref<const Input::Cursor>&);
void OnCursorMiddleMouseReleased(const Ref<const Input::Cursor>&);
void OnScrolled(float);
void OnKeyPressed(const Ref<const Input::Key>&);
void OnKeyReleased(const Ref<const Input::Key>&);
void OnKeyStayDown(const Ref<const Input::Key>&);
void OnObjectsSelectedFromThis();
void RedrawContent();
void DrawObjects();
void DrawSelection();
void BindSceneTree();
void OnTreeSelectionChanged(const Vector<Ref<SceneEditableObject>>&);
void UpdateTopSelectedObjects();
void OnSceneChanged(const Vector<Ref<SceneEditableObject>>&);
void ClearSelectionWithoutAction(bool);
void SelectObjectsWithoutAction(const Vector<Ref<SceneEditableObject>>&, bool);
void SelectObjectWithoutAction(const Ref<SceneEditableObject>&, bool);
void OnDropped(ISelectableDragableObjectsGroup*);
void OnDragEnter(ISelectableDragableObjectsGroup*);
void OnDraggedAbove(ISelectableDragableObjectsGroup*);
void OnDragExit(ISelectableDragableObjectsGroup*);
bool IsInputTransparent();

class END_META {};

// --- END META ---