#pragma once

#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Render/Camera.h"
#include "o2/Render/TextureRef.h"
#include "o2/Scene/UI/Widget.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

namespace Editor
{
	// ----------------------------
	// Scrollable and zoomable view
	// ----------------------------
	class ScrollView : public Widget, virtual public CursorAreaEventsListener
	{
	public:
		bool horGridEnabled = true; // Is horizontal lines of grid are enabled
		bool verGridEnabled = true; // Is vertical lines of grid are enabled

	public:
		// Default constructor
		ScrollView();

		// Copy-constructor
		ScrollView(const ScrollView& other);

		// Destructor
		~ScrollView();

		// Copy-operator
		ScrollView& operator=(const ScrollView& other);

		// Draws widget, updates render target 
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Transforms point from screen space to local space
		Vec2F ScreenToLocalPoint(const Vec2F& point);

		// Transforms point from local space to screen space
		Vec2F LocalToScreenPoint(const Vec2F& point);

		// Returns local to screen transform
		const Basis& GetLocalToScreenTransform() const;

		// Returns screen to local transform
		const Basis& GetScreenToLocalTransform() const;

		// Returns camera scale
		Vec2F GetCameraScale() const;

		// Sets view camera
		void SetCamera(const Ref<Camera>& camera);

		// Returns view camera
		const Ref<Camera>& GetCamera() const;

		// Sets back color
		void SetBackColor(const Color4& color);

		// Sets grid color
		void SetGridColor(const Color4& color);

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns is this widget can be focused
		bool IsFocusable() const override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ScrollView);

	protected:
		bool mReady = false; // Is widget initialized and ready to use

		Sprite* mRenderTargetSprite = nullptr; // Render target sprite, using for caching graphics
		Ref<Texture> mRenderTarget;             // Render target texture, using for caching graphics
		bool mNeedRedraw = false;               // Is need to redraw render target

		Color4 mBackColor; // Color of back @SERIALIZABLE
		Color4 mGridColor; // Color of grid @SERIALIZABLE

		RectF mViewArea;                               // View area range
		Ref<Camera> mViewCamera;                       // Scene view camera
		Vec2F mViewCameraTargetScale = Vec2F(1, 1);    // Camera target scale
		float mViewCameraScaleSence = 0.1f / 120.0f;   // Camera scale sense
		float mViewCameraScaleElasticyCoef = 30.0f;    // Scale smoothing coefficient
		Vec2F mViewCameraTargetPos;                    // Target camera position
		Vec2F mViewCameraVelocity;                      // Camera velocity
		float mViewCameraPosElasticyCoef = 30.0f;       // Camera dragging smoothing coefficient
		float mViewCameraVelocityDampingCoef = 10.0f;  // Camera velocity damping coefficient
		float mViewCameraMinScale = 0.001f;             // Minimal camera scale
		float mViewCameraMaxScale = 10000.0f;           // Maximal camera scale

		Basis mLocalToScreenTransform; // Local to screen transformation
		Basis mScreenToLocalTransform; // Screen to local transformation

	protected:
		// Called when transformation was changed and updated, updates render texture and sprite
		void OnTransformUpdated() override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Updates camera
		void UpdateCamera(float dt);

		// Returns transformation basis from screen to local for camera
		Basis GetCameraScreenToLocalTransform(const Ref<Camera>& camera) const;

		// Updates scene to screen and screen and scene transformations
		void UpdateLocalScreenTransforms();

		// Redraws content into render target
		void RedrawRenderTarget();

		// Redraws content into render target
		virtual void RedrawContent();

		// Draws grid
		virtual void DrawGrid();

		// Called when camera position was changed
		virtual void OnCameraTransformChanged();

		// Called when scrolling
		void OnScrolled(float scroll) override;

		// Updates camera's scale and position
		void ChangeCameraScaleRelativeToCursor(const Vec2F& newScale);

		// Called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// Called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// Called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// Returns that this scroll area has transparent input
		bool IsInputTransparent() const override;
	};

	Ref<Camera> DynamicCast(const Ref<Camera>& camera);
}#include <memory>
#include <iostream>
#include <vector>

template<typename T>
class Ref
{
public:
    Ref() : ptr(nullptr) {}

    Ref(T* p) : ptr(p)
    {
        addReference();
    }

    Ref(const Ref<T>& other) : ptr(other.ptr)
    {
        addReference();
    }

    ~Ref()
    {
        releaseReference();
    }

    Ref<T>& operator=(T* p)
    {
        if (ptr == p)
            return *this;

        releaseReference();
        ptr = p;
        addReference();

        return *this;
    }
    
    Ref<T>& operator=(const Ref<T>& other)
    {
        if (ptr == other.ptr)
            return *this;

        releaseReference();
        ptr = other.ptr;
        addReference();

        return *this;
    }

    T* operator->() const
    {
        return ptr;
    }

    T& operator*() const
    {
        return *ptr;
    }

private:
    T* ptr;

    void addReference()
    {
        if (ptr)
            ptr->addReference();
    }

    void releaseReference()
    {
        if (ptr && ptr->releaseReference() == 0)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
};

class WeakRef
{
public:
    WeakRef() : ptr(nullptr) {}

    WeakRef(Ref* p) : ptr(p) {}

    bool expired() const
    {
        return ptr == nullptr;
    }

    Ref lock() const
    {
        if (expired())
            return Ref();

        return Ref(ptr);
    }

private:
    Ref* ptr;
};

template<typename T>
class DynamicCast
{
public:
    DynamicCast(const Ref<T>& obj) : obj(obj) {}

    template<typename U>
    operator Ref<U>()
    {
        return Ref<U>(dynamic_cast<U*>(obj.get()));
    }

private:
    Ref<T> obj;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

class Vec2F
{
public:
    Vec2F(float x, float y) : x(x), y(y) {}

    float x;
    float y;
};

class Basis
{
public:
    Basis() {}

    // implementation goes here
};

class Camera
{
public:
    Camera() {}

    // implementation goes here
};

class Color4
{
public:
    Color4() {}

    // implementation goes here
};

class String
{
public:
    String() {}

    // implementation goes here
};

namespace Editor
{
    class CursorAreaEventsListener
    {
    public:
        CursorAreaEventsListener() {}

        // implementation goes here
    };

    class ScrollView : public o2::Widget, public CursorAreaEventsListener
    {
    public:
        ScrollView() {}

        ScrollView(const ScrollView&) {}

        void Draw() {}

        void Update(float) {}

        Vec2F ScreenToLocalPoint(const Vec2F&) {}

        Vec2F LocalToScreenPoint(const Vec2F&) {}

        const Basis& GetLocalToScreenTransform() {}

        const Basis& GetScreenToLocalTransform() {}

        Vec2F GetCameraScale() {}

        void SetCamera(const Camera&) {}

        const Camera& GetCamera() {}

        void SetBackColor(const Color4&) {}

        void SetGridColor(const Color4&) {}

        void UpdateSelfTransform() {}

        bool IsUnderPoint(const Vec2F&) {}

        bool IsScrollable() {}

        bool IsFocusable() {}

        static String GetCreateMenuCategory() {}

    protected:
        void OnTransformUpdated() {}

        void UpdateTransparency() {}

        void UpdateCamera(float) {}

        Basis GetCameraScreenToLocalTransform(const Camera&) {}

        void UpdateLocalScreenTransforms() {}

        void RedrawRenderTarget() {}

        void RedrawContent() {}

        void DrawGrid() {}

        void OnCameraTransformChanged() {}

        void OnScrolled(float) {}

        void ChangeCameraScaleRelativeToCursor(const Vec2F&) {}

        void OnCursorRightMousePressed(const Input::Cursor&) {}

        void OnCursorRightMouseStayDown(const Input::Cursor&) {}

        void OnCursorRightMouseReleased(const Input::Cursor&) {}

        bool IsInputTransparent() {}

    private:
        Ref<o2::Sprite> mRenderTargetSprite;
        Ref<> mRenderTarget;
        Ref<> mViewArea;
        Ref<> mViewCamera;
        Ref<> mLocalToScreenTransform;
        Ref<> mScreenToLocalTransform;

        bool horGridEnabled;
        bool verGridEnabled;
        bool mReady;
        bool mNeedRedraw;
        Vec2F mViewCameraTargetScale;
        float mViewCameraScaleSence;
        float mViewCameraScaleElasticyCoef;
        Vec2F mViewCameraTargetPos;
        Vec2F mViewCameraVelocity;
        float mViewCameraPosElasticyCoef;
        float mViewCameraVelocityDampingCoef;
        float mViewCameraMinScale;
        float mViewCameraMaxScale;

        Color4 mBackColor;
        Color4 mGridColor;
    };
}

// --- META ---

CLASS_BASES_META(Editor::ScrollView)
{
    BASE_CLASS(o2::Widget);
    BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;

CLASS_FIELDS_META(Editor::ScrollView)
{
    FIELD().PUBLIC().DEFAULT_VALUE(true).NAME(horGridEnabled);
    FIELD().PUBLIC().DEFAULT_VALUE(true).NAME(verGridEnabled);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mReady);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<o2::Sprite>()).NAME(mRenderTargetSprite);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<>()).NAME(mRenderTarget);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNeedRedraw);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mBackColor);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mGridColor);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<>()).NAME(mViewArea);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<>()).NAME(mViewCamera);
    FIELD().PROTECTED().DEFAULT_VALUE(Vec2F(1, 1)).NAME(mViewCameraTargetScale);
    FIELD().PROTECTED().DEFAULT_VALUE(0.1f / 120.0f).NAME(mViewCameraScaleSence);
    FIELD().PROTECTED().DEFAULT_VALUE(30.0f).NAME(mViewCameraScaleElasticyCoef);
    FIELD().PROTECTED().DEFAULT_VALUE(Vec2F()).NAME(mViewCameraTargetPos);
    FIELD().PROTECTED().DEFAULT_VALUE(Vec2F()).NAME(mViewCameraVelocity);
    FIELD().PROTECTED().DEFAULT_VALUE(30.0f).NAME(mViewCameraPosElasticyCoef);
    FIELD().PROTECTED().DEFAULT_VALUE(10.0f).NAME(mViewCameraVelocityDampingCoef);
    FIELD().PROTECTED().DEFAULT_VALUE(0.001f).NAME(mViewCameraMinScale);
    FIELD().PROTECTED().DEFAULT_VALUE(10000.0f).NAME(mViewCameraMaxScale);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<>()).NAME(mLocalToScreenTransform);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<>()).NAME(mScreenToLocalTransform);
}
END_META;

CLASS_METHODS_META(Editor::ScrollView)
{
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Editor::ScrollView&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToLocalPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, LocalToScreenPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(const Basis&, GetLocalToScreenTransform);
    FUNCTION().PUBLIC().SIGNATURE(const Basis&, GetScreenToLocalTransform);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetCameraScale);
    FUNCTION().PUBLIC().SIGNATURE(void, SetCamera, const Camera&);
    FUNCTION().PUBLIC().SIGNATURE(const Camera&, GetCamera);
    FUNCTION().PUBLIC().SIGNATURE(void, SetBackColor, const Color4&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetGridColor, const Color4&);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsScrollable);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsFocusable);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateTransparency);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateCamera, float);
    FUNCTION().PROTECTED().SIGNATURE(Basis, GetCameraScreenToLocalTransform, const Camera&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateLocalScreenTransforms);
    FUNCTION().PROTECTED().SIGNATURE(void, RedrawRenderTarget);
    FUNCTION().PROTECTED().SIGNATURE(void, RedrawContent);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawGrid);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCameraTransformChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
    FUNCTION().PROTECTED().SIGNATURE(void, ChangeCameraScaleRelativeToCursor, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMousePressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsInputTransparent);
}
END_META;
// --- END META ---