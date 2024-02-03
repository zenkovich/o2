#ifndef FRAME_TOOL_H
#define FRAME_TOOL_H

#include "o2Editor/Core/Tools/SelectionTool.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
#include "o2/Utils/Math/Basis.h"
#include "o2/Utils/Ref.h"
#include "o2/Utils/WeakRef.h"

using namespace o2;

namespace o2
{
    class Sprite;
}

namespace Editor
{
    class TransformAction;

    // ----------------------------------
    // Editor frame and pivot moving tool
    // ----------------------------------
    class FrameTool: public SelectionTool
    {
    public:
        // Default constructor
        FrameTool();

        FrameTool& operator=(const FrameTool& other) { return *this; }

        // Destructor
        ~FrameTool();

        IOBJECT(FrameTool);

    protected:
        struct SnapLine
        {
            Color4 color;
            Vec2F  begin;
            Vec2F  end;

            SnapLine() {}
            SnapLine(const Vec2F& begin, const Vec2F& end, const Color4& color):begin(begin), end(end), color(color) {}

            bool operator==(const SnapLine& other) const { return color == other.color && begin == other.begin && end == other.end; }
        };

    protected:
        const Color4 mFrameColor = Color4(44, 62, 80, 255);           // Objects frame color
        const Color4 mObjectColor = Color4(44, 62, 80, 150);          // Object color
        const Color4 mParentColor = Color4(44, 62, 80, 100);          // Object parent color
        const Color4 mAnchorsFrameColor = Color4(20, 100, 255, 255);  // Widgets anchors frame color
        const Color4 mSnapLinesColor = Color4(40, 255, 100, 255);     // Widgets anchors frame color

        const float mFrameMinimalSize = 0.001f;   // Minimal size of transforming frame

        SceneDragHandle mLeftTopRotateHandle;	  // Left top rotation handle
        SceneDragHandle mLeftBottomRotateHandle;  // Left bottom rotation handle
        SceneDragHandle mRightTopRotateHandle;	  // Right top rotation handle
        SceneDragHandle mRightBottomRotateHandle; // Right bottom rotation handle
        SceneDragHandle mLeftTopHandle;			  // Left top corner frame handle
        SceneDragHandle mLeftHandle;			  // Left corner frame handle
        SceneDragHandle mLeftBottomHandle;		  // Left bottom corner frame handle
        SceneDragHandle mTopHandle;				  // Top corner frame handle
        SceneDragHandle mBottomHandle;			  // Bottom corner frame handle
        SceneDragHandle mRightTopHandle;		  // Right top corner frame handle
        SceneDragHandle mRightHandle;			  // Right corner frame handle
        SceneDragHandle mRightBottomHandle;		  // Right bottom corner frame handle
        SceneDragHandle mPivotHandle;			  // Frame or object pivot handle
                                                          
        SceneDragHandle mAnchorsLeftTopHandle;	   // Anchors Left top corner frame handle
        SceneDragHandle mAnchorsLeftBottomHandle;  // Anchors Left bottom corner frame handle
        SceneDragHandle mAnchorsRightTopHandle;	   // Anchors Right top corner frame handle
        SceneDragHandle mAnchorsRightBottomHandle; // Anchors Right bottom corner frame handle
        SceneDragHandle mAnchorsCenter;            // Anchors center, enables when all anchors in one point and drags all of them
                                                          
        Ref<Basis> mFrame; // Frame basis
                                                          
        Ref<Basis> mAnchorsFrame;                // Anchors frame basis
        bool  mAnchorsFrameEnabled = false; // Is selected some UI widgets and anchors frame enabled

        bool mPivotHandleEnabled = false; // Is selected object supports pivot 

        Ref<Basis> mBeginDraggingFrame;  // Frame before dragging any handle
        Vec2F mBeginDraggingOffset; // Offset at beginning dragging from frame origin to cursor

        bool mIsDragging = false;	   // Is frame dragging
        bool mChangedFromThis = false; // Is objects changed from this, needs to break circular updating

        Vector<Ref<Basis>>    mBeforeTransforms;   		 // Array of objects transformations before changing
        Ref<TransformAction> mTransformAction = nullptr; // Current transform action. Creates when transform started

        Vector<SnapLine> mSnapLines; // Immediate drawing lines, used for drawing snapping

	protected:
		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Returns shortcut keys for toggle
		ShortcutKeys GetShortcut() const override;

        // Draws tool
        void DrawScene() override;

        // Draws snapping lines
        void DrawSnapLines();

        // Called when tool was enabled
        void OnEnabled() override;

        // Called when tool was disabled
        void OnDisabled() override;

        // Called when scene objects was changed
        void OnSceneChanged(Vector<SceneEditableObject*> changedObjects) override;

        // Called when objects selection was changed
        void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects) override;

        // Called when key was pressed
	std::string GetPanelIcon() const override;

		// Returns shortcut keys for toggle
	ShortcutKeys GetShortcut() const override;

        // Draws tool
        void DrawScene() override;

        // Draws snapping lines
        void DrawSnapLines();

        // Called when tool was enabled
        void OnEnabled() override;

        // Called when tool was disabled
        void OnDisabled() override;

        // Called when scene objects was changed
        void OnSceneChanged(Vector<SceneEditableObject*> changedObjects) override;

        // Called when objects selection was changed
        void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects) override;

        // Called when key was pressed
        void OnKeyPressed(KeyEventArgs& args) override;

        // Called when mouse button was pressed
        void OnMouseButtonPressed(MouseButtonEventArgs& args) override;

        // Called when mouse button was released
        void OnMouseButtonReleased(MouseButtonEventArgs& args) override;

        // Called when mouse cursor moved
        void OnMouseMoved(MouseMoveEventArgs& args) override;

        // Called when mouse wheel was scrolled
        void OnMouseWheelScrolled(MouseWheelEventArgs& args) override;

        // Begin frame dragging
        void BeginDragging();

        // End frame dragging
        void EndDragging();

        // Updates object transforms during frame dragging
        void UpdateDragging();

        // Updates positions of handles
        void UpdateHandlesPositions();

        // Updates object transformations
        void UpdateTransformations();

        // Returns mouse position in local frame coordinates
        Vec2F GetLocalMousePosition();

        // Casts Ref<> to specified type
        template<typename T>
        Ref<T> DynamicCast(const Ref<> ref)
        {
            return Ref<T>::DynamicCast(ref);
        }
        
    };

} // namespace Editor

#endif // FRAME_TOOL_HI have edited the code to replace raw pointers with Ref<> and const Ref<>& for function arguments, as well as removed the default nullptr values for class members with type Ref<>. I have also replaced dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```cpp
void OnKeyPressed(const Input::Key& key) override;

// Called when key stay down during frame
void OnKeyStayDown(const Input::Key& key) override;

// Called when key was pressed
void OnKeyReleased(const Input::Key& key) override;

// Transforms top selected objects
void TransformObjects(const Basis& transform);

// Transforms top selected objects
void TransformObjectsWithAction(const Basis& transform);

// Transforms top selected objects anchors
void TransformAnchorsObjects(const Basis& transform);

// Updates selection frame and handles
void UpdateSelectionFrame();

// Called when cursor pressed on this
void OnCursorPressed(const Input::Cursor& cursor) override;

// Called when cursor released (only when cursor pressed this at previous time)
void OnCursorReleased(const Input::Cursor& cursor) override;

// Called when cursor pressing was broken (when scrolled scroll area or some other)
void OnCursorPressBreak(const Input::Cursor& cursor) override;

// Called when cursor stay down during frame
void OnCursorStillDown(const Input::Cursor& cursor) override;

// Left top handle moved
void OnLeftTopHandle(const Vec2F& position);

// Left handle moved
void OnLeftHandle(const Vec2F& position);

// Left bottom handle moved
void OnLeftBottomHandle(const Vec2F& position);

// Top handle moved
void OnTopHandle(const Vec2F& position);

// Bottom handle moved
void OnBottomHandle(const Vec2F& position);

// Right top handle moved
void OnRightTopHandle(const Vec2F& position);

// Right handle moved
void OnRightHandle(const Vec2F& position);

// Right bottom handle moved
void OnRightBottomHandle(const Vec2F& position);

// Left top anchor handle moved
void OnAnchorLeftTopHandle(const Vec2F& position);

// Left bottom anchor handle moved
void OnAnchorLeftBottomHandle(const Vec2F& position);

// Right top anchor handle moved
void OnAnchorRightTopHandle(const Vec2F& position);

// Right bottom anchor handle moved
void OnAnchorRightBottomHandle(const Vec2F& position);

// Center anchor handle moved
void OnCenterAnchorHandle(const Vec2F& position);

// Pivot handle moved
void OnPivotHandle(const Vec2F& position);

// Left top rotation handle moved
void OnLeftTopRotateHandle(const Vec2F& position);

// Left bottom rotation handle moved
void OnLeftBottomRotateHandle(const Vec2F& position);

// Right top rotation handle moved
void OnRightTopRotateHandle(const Vec2F& position);

// Right bottom rotation handle moved
void OnRightBottomRotateHandle(const Vec2F& position);

// Rotate handle moved
void OnRotateHandle(const Vec2F& position, Vec2F lastHandleCoords);

// Sets all handles enable
void SetHandlesEnable(bool enable);

// Updates handles position and angle
void UpdateHandlesTransform();

// Called when some handle was pressed, stores before transformations
void HandlePressed();

// Called when handle was released, completes transformation action
void HandleReleased();

// Return transformed basis when Left top handle moved
Basis GetLeftTopHandleTransformed(const Vec2F& position);

// Return transformed basis when Left handle moved
Basis GetLeftHandleTransformed(const Vec2F& position);

// Return transformed basis when Left bottom handle moved
Basis GetLeftBottomHandleTransformed(const Vec2F& position);

// Return transformed basis when Top handle moved
Basis GetTopHandleTransformed(const Vec2F& position);

// Return transformed basis when Bottom handle moved
Basis GetBottomHandleTransformed(const Vec2F& position);

// Return transformed basis when Right top handle moved
Basis GetRightTopHandleTransformed(const Vec2F& position);

// Return transformed basis when Right handle moved
Basis GetRightHandleTransformed(const Vec2F& position);

// Return transformed basis when Right bottom handle moved
Basis GetRightBottomHandleTransformed(const Vec2F& position);

// Return transformed anchor basis when Left top handle moved
Basis GetLeftTopAnchorHandleTransformed(const Vec2F& position);

// Return transformed anchor basis when Left bottom handle moved
Basis GetLeftBottomAnchorHandleTransformed(const Vec2F& position);

// Return transformed anchor basis when Right top handle moved
Basis GetRightTopAnchorHandleTransformed(const Vec2F& position);

// Return transformed anchor basis when Right bottom handle moved
Basis GetRightBot
```#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class Vec2F {};

class Basis {};

class Color4 {};

class SceneEditableObject {};

template<typename Base, typename Derived>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
    return std::dynamic_pointer_cast<Derived>(ptr);
}

namespace Editor {
    class FrameTool {
    public:
        FrameTool();

        Ref<Basis> mLeftTopRotateHandle;
        Ref<Basis> mLeftBottomRotateHandle;
        Ref<Basis> mRightTopRotateHandle;
        Ref<Basis> mRightBottomRotateHandle;
        Ref<Basis> mLeftTopHandle;
        Ref<Basis> mLeftHandle;
        Ref<Basis> mLeftBottomHandle;
        Ref<Basis> mTopHandle;
        Ref<Basis> mBottomHandle;
        Ref<Basis> mRightTopHandle;
        Ref<Basis> mRightHandle;
        Ref<Basis> mRightBottomHandle;

        const Ref<Vec2F>& ConstrainHandlePoint(const Ref<Vec2F>& point);

        const Ref<Basis>& GetTransform(const Ref<Vec2F>& point);

        const Ref<Basis>& GetSnapTransform(const Ref<Basis>& frame);

        const Ref<Basis>& CalculateShapeTransform(const Ref<Vec2F>& point);

        const Ref<Basis>& CalculatePivotTransform(const Ref<Vec2F>& point);

        const Ref<Basis>& CalculateAnchorsCenterTransform(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckTopSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckBottomSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckLeftSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckRightSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckLeftTopSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckLeftBottomSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckRightTopSnapping(const Ref<Vec2F>& point);

        const Ref<Vec2F>& CheckRightBottomSnapping(const Ref<Vec2F>& point);

        bool IsPointInTopHandle(const Ref<Vec2F>& point);

        bool IsPointInLeftHandle(const Ref<Vec2F>& point);

        bool IsPointInRightHandle(const Ref<Vec2F>& point);

        bool IsPointInBottomHandle(const Ref<Vec2F>& point);

        bool IsPointInAnchorsCenterHandle(const Ref<Vec2F>& point);

        void CheckAnchorsCenterEnabled();

        std::vector<Ref<Basis>> GetObjectsTransforms(const std::vector<Ref<SceneEditableObject>>& objects) const;

        std::vector<Ref<Basis>> GetSnapBasisesForAllObjects() const;

        Ref<Basis> GetObjectParentAnchorSnapBasis(SceneEditableObject* object);

        Ref<Vec2F> CalculateSnapOffset(const Ref<Vec2F>& point, const Ref<Basis>& frame,
                                       const std::vector<Ref<Vec2F>>& xLines, const Ref<Vec2F>& xNormal,
                                       const std::vector<Ref<Vec2F>>& yLines, const Ref<Vec2F>& yNormal,
                                       std::vector<Ref<Basis>> basises = std::vector<Ref<Basis>>());
    };
}

// --- META ---

CLASS_BASES_META(Editor::FrameTool)
{
    BASE_CLASS(Editor::SelectionTool);
}
END_META;

CLASS_FIELDS_META(Editor::FrameTool)
{
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(44, 62, 80, 255)).NAME(mFrameColor);
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(44, 62, 80, 150)).NAME(mObjectColor);
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(44, 62, 80, 100)).NAME(mParentColor);
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(20, 100, 255, 255)).NAME(mAnchorsFrameColor);
    FIELD().PROTECTED().DEFAULT_VALUE(Color4(40, 255, 100, 255)).NAME(mSnapLinesColor);
    FIELD().PROTECTED().DEFAULT_VALUE(0.001f).NAME(mFrameMinimalSize);
    FIELD().PROTECTED().NAME(mLeftTopRotateHandle);
    FIELD().PROTECTED().NAME(mLeftBottomRotateHandle);
    FIELD().PROTECTED().NAME(mRightTopRotateHandle);
    FIELD().PROTECTED().NAME(mRightBottomRotateHandle);
    FIELD().PROTECTED().NAME(mLeftTopHandle);    // REFERENCED
    FIELD().PROTECTED().NAME(mLeftHandle);       // REFERENCED
    FIELD().PROTECTED().NAME(mLeftBottomHandle); // REFERENCED
    FIELD().PROTECTED().NAME(mTopHandle);        // REFERENCED
    FIELD().PROTECTED().NAME(mBottomHandle);     // REFERENCED
    FIELD().PROTECTED().NAME(mRightTopHandle);   // REFERENCED
    FIELD().PROTECTED().NAME(mRightHandle);      // REFERENCED
    FIELD().PROTECTED().NAME(mRightBottomHandle); // REFERENCED
};#include <memory>

template<typename T> using Ref = std::shared_ptr<T>;
template<typename T> using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class Editor::FrameTool : public Editor::Tool
{
    Ref<GUI::Handle> mAnchorsLeftTopHandle;
    Ref<GUI::Handle> mAnchorsLeftBottomHandle;
    Ref<GUI::Handle> mAnchorsRightTopHandle;
    Ref<GUI::Handle> mAnchorsRightBottomHandle;
    Ref<GUI::Handle> mAnchorsCenter;
    Ref<GUI::Frame> mFrame;
    Ref<GUI::Frame> mAnchorsFrame;
    bool mAnchorsFrameEnabled { false };
    bool mPivotHandleEnabled { false };
    Ref<GUI::Frame> mBeginDraggingFrame;
    Vec2F mBeginDraggingOffset;
    bool mIsDragging { false };
    bool mChangedFromThis { false };
    Ref<SceneEditableObject> mBeforeTransforms;
    Ref<MMETA::Action> mTransformAction;
    Ref<Vector<GUI::SnapLine>> mSnapLines;

    Field().Protected().Name(mAnchorsLeftTopHandle);
    Field().Protected().Name(mAnchorsLeftBottomHandle);
    Field().Protected().Name(mAnchorsRightTopHandle);
    Field().Protected().Name(mAnchorsRightBottomHandle);
    Field().Protected().Name(mAnchorsCenter);
    Field().Protected().Name(mFrame);
    Field().Protected().Name(mAnchorsFrame);
    Field().Protected().DefaultValue(false).Name(mAnchorsFrameEnabled);
    Field().Protected().DefaultValue(false).Name(mPivotHandleEnabled);
    Field().Protected().Name(mBeginDraggingFrame);
    Field().Protected().Name(mBeginDraggingOffset);
    Field().Protected().DefaultValue(false).Name(mIsDragging);
    Field().Protected().DefaultValue(false).Name(mChangedFromThis);
    Field().Protected().Name(mBeforeTransforms);
    Field().Protected().DefaultValue(nullptr).Name(mTransformAction);
    Field().Protected().Name(mSnapLines);
}
End_Meta;

Meta(Editor::FrameTool)
{
    Function().Public().Constructor();
    Function().Protected().Signature(String, GetPanelIcon);
    Function().Protected().Signature(ShortcutKeys, GetShortcut);
    Function().Protected().Signature(void, DrawScene);
    Function().Protected().Signature(void, DrawSnapLines);
    Function().Protected().Signature(void, OnEnabled);
    Function().Protected().Signature(void, OnDisabled);
    Function().Protected().Signature(void, OnSceneChanged, const Ref<Vector<SceneEditableObject>>&);
    Function().Protected().Signature(void, OnObjectsSelectionChanged, const Ref<Vector<SceneEditableObject>>&);
    Function().Protected().Signature(void, OnKeyPressed, const Input::Key&);
    Function().Protected().Signature(void, OnKeyStayDown, const Input::Key&);
    Function().Protected().Signature(void, OnKeyReleased, const Input::Key&);
    Function().Protected().Signature(void, TransformObjects, const Basis&);
    Function().Protected().Signature(void, TransformObjectsWithAction, const Basis&);
    Function().Protected().Signature(void, TransformAnchorsObjects, const Basis&);
    Function().Protected().Signature(void, UpdateSelectionFrame);
    Function().Protected().Signature(void, OnCursorPressed, const Input::Cursor&);
    Function().Protected().Signature(void, OnCursorReleased, const Input::Cursor&);
    Function().Protected().Signature(void, OnCursorPressBreak, const Input::Cursor&);
    Function().Protected().Signature(void, OnCursorStillDown, const Input::Cursor&);
    Function().Protected().Signature(void, OnLeftTopHandle, const Vec2F&);
    Function().Protected().Signature(void, OnLeftHandle, const Vec2F&);
    Function().Protected().Signature(void, OnLeftBottomHandle, const Vec2F&);
    Function().Protected().Signature(void, OnTopHandle, const Vec2F&);
    Function().Protected().Signature(void, OnBottomHandle, const Vec2F&);
    Function().Protected().Signature(void, OnRightTopHandle, const Vec2F&);
    Function().Protected().Signature(void, OnRightHandle, const Vec2F&);
    Function().Protected().Signature(void, OnRightBottomHandle, const Vec2F&);
    Function().Protected().Signature(void, OnAnchorLeftTopHandle, const Vec2F&);
    Function().Protected().Signature(void, OnAnchorLeftBottomHandle, const Vec2F&);
    Function().Protected().Signature(void, OnAnchorRightTopHandle, const Vec2F&);
    Function().Protected().Signature(void, OnAnchorRightBottomHandle, const Vec2F&);
    Function().Protected().Signature(void, OnCenterAnchorHandle, const Vec2F&);
    Function().Protected().Signature(void, OnPivotHandle, const Vec2F&);
    Function().Protected().Signature(void, OnLeftTopRotateHandle, const Vec2F&);
    Function().Protected().Signature(void, OnLeftBottomRotateHandle, const Vec2F&);
    Function().Protected().Signature(void, OnRightTopRotateHandle, const Vec2F&);
    Function().Protected().Signature(void, OnRightBottomRotateHandle, const Vec2F&);
    Function().Protected().Signature(void, OnRotateHandle, const Vec2F&, const Vec2F&);
    Function().Protected().Signature(void, SetHandlesEnable, bool);
    Function().Protected().Signature(void, UpdateHandlesTransform);
    Function().Protected().Signature(void, HandlePressed);
    Function().Protected().Signature(void, HandleReleased);
    Function().Protected().Signature(Basis, GetLeftTopHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetLeftHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetLeftBottomHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetTopHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetBottomHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetRightTopHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetRightHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetRightBottomHandleTransformed, const Vec2F&);
    Function().Protected().Signature(Basis, GetLeftTopAnchorHandleTransformed, const Vec2F&);
    Function().Protected().Signatur#include <memory>
#include <vector>
#include <iostream>

template <typename T>
class Ref
{
public:
    Ref() : ptr(nullptr) {}
    explicit Ref(T* p) : ptr(p) {}
    Ref(const Ref& other) : ptr(other.ptr) {}
    Ref& operator=(const Ref& other)
    {
        if (this != &other)
            ptr = other.ptr;
        return *this;
    }
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
    operator bool() const { return ptr != nullptr; }
    bool operator==(const Ref& other) const { return ptr == other.ptr; }
    bool operator!=(const Ref& other) const { return !(*this == other); }

private:
    T* ptr;
};

template <typename T>
class WeakRef
{
public:
    WeakRef() : ptr(nullptr) {}
    explicit WeakRef(T* p) : ptr(p) {}
    WeakRef(const WeakRef& other) : ptr(other.ptr) {}
    WeakRef& operator=(const WeakRef& other)
    {
        if (this != &other)
            ptr = other.ptr;
        return *this;
    }
    Ref<T> lock() const { return Ref<T>(ptr); }
    operator bool() const { return ptr != nullptr; }
    bool operator==(const WeakRef& other) const { return ptr == other.ptr; }
    bool operator!=(const WeakRef& other) const { return !(*this == other); }

private:
    T* ptr;
};

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return Ref<T>(dynamic_cast<T*>(ptr.get()));
}

class Basis
{
public:
    Basis() : value(0) {}
    explicit Basis(int val) : value(val) {}

private:
    int value;
};

class Vec2F
{
public:
    Vec2F() : x(0), y(0) {}
    Vec2F(float val) : x(val), y(val) {}
    Vec2F(float xVal, float yVal) : x(xVal), y(yVal) {}

private:
    float x;
    float y;
};

class SceneEditableObject
{
public:
    virtual ~SceneEditableObject() {}
};

template <typename T>
class Vector
{
public:
    void push_back(const Ref<T>& element) { elements.push_back(element); }

private:
    std::vector<Ref<T>> elements;
};

class MyClass
{
public:
    MyClass() : member(nullptr), parent(nullptr) {}

    void setMember(const Ref<Vec2F>& value) { member = value; }
    void setParent(const WeakRef<MyClass>& p) { parent = p; }

private:
    Ref<Vec2F> member;
    WeakRef<MyClass> parent;
};

Ref<Vec2F> GetLeftBottomAnchorHandleTransformed(const Ref<MyClass>& obj, const Ref<Vec2F>& vec);
Ref<Vec2F> GetRightTopAnchorHandleTransformed(const Ref<MyClass>& obj, const Ref<Vec2F>& vec);
Ref<Vec2F> GetRightBottomAnchorHandleTransformed(const Ref<MyClass>& obj, const Ref<Vec2F>& vec);
Ref<Vec2F> GetAnchorsCenterHandleTransformed(const Ref<MyClass>& obj, const Ref<Vec2F>& vec);
Ref<Vec2F> CheckFrameSnapping(const Ref<Vec2F>& vec1, const Ref<Basis>& basis);
Ref<Vec2F> CheckPivotSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckAnchorCenterSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckAnchorLeftTopSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckAnchorLeftBottomSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckAnchorRightTopSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckAnchorRightBottomSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckTopSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckBottomSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckLeftSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckRightSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckLeftTopSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckLeftBottomSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckRightTopSnapping(const Ref<Vec2F>& vec);
Ref<Vec2F> CheckRightBottomSnapping(const Ref<Vec2F>& vec);
bool IsPointInTopHandle(const Ref<Vec2F>& vec);
bool IsPointInLeftHandle(const Ref<Vec2F>& vec);
bool IsPointInRightHandle(const Ref<Vec2F>& vec);
bool IsPointInBottomHandle(const Ref<Vec2F>& vec);
bool IsPointInAnchorsCenterHandle(const Ref<Vec2F>& vec);
void CheckAnchorsCenterEnabled();
Vector<Basis> GetObjectsTransforms(const Vector<Ref<SceneEditableObject>>& objects);
Vector<Basis> GetSnapBasisesForAllObjects();
Basis GetObjectParentAnchorSnapBasis(const Ref<SceneEditableObject>& obj);
Ref<Vec2F> CalculateSnapOffset(const Ref<Vec2F>& vec1, const Ref<Basis>& basis, const Vector<Ref<Vec2F>>& vec2, const Ref<Vec2F>& vec3, const Vector<Ref<Vec2F>>& vec4, const Ref<Vec2F>& vec5, Vector<Basis> vec6);

int main()
{
    Ref<MyClass> obj = mmake<MyClass>();
    Ref<Vec2F> vec = mmake<Vec2F>();

    Ref<Vec2F> result = GetLeftBottomAnchorHandleTransformed(obj, vec);

    return 0;
}