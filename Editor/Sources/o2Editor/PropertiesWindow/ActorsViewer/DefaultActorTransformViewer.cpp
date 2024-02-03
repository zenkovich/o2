#include "o2Editor/stdafx.h"
#include "DefaultActorTransformViewer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/Vector2FloatProperty.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	DefaultActorTransformViewer::DefaultActorTransformViewer()
	{
		PushEditorScopeOnStack scope;

		// Position
		auto positionPropertyContainer = mmake<Widget>();
		positionPropertyContainer->name = "position";
		positionPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(positionPropertyContainer);

		auto positionIcon = o2UI.CreateImage("ui/UI4_position_icon.png");
		*positionIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		positionPropertyContainer->AddChild(positionIcon);

		mPositionProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPositionProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		positionPropertyContainer->AddChild(mPositionProperty);

		// Pivot
		auto pivotPropertyContainer = mmake<Widget>();
		pivotPropertyContainer->name = "pivot";
		pivotPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(pivotPropertyContainer);

		auto pivotIcon = o2UI.CreateImage("ui/UI4_pivot_icon.png");
		*pivotIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		pivotPropertyContainer->AddChild(pivotIcon);

		mPivotProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPivotProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPivotProperty->SetValuePath("transform/pivot");
		mPivotProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		pivotPropertyContainer->AddChild(mPivotProperty);

		// Size
		auto sizePropertyContainer = mmake<Widget>();
		sizePropertyContainer->name = "size";
		sizePropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(sizePropertyContainer);

		auto sizeIcon = o2UI.CreateImage("ui/UI4_icon_size.png");
		*sizeIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 0));
		sizePropertyContainer->AddChild(sizeIcon);

		mSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mSizeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		sizePropertyContainer->AddChild(mSizeProperty);

		// Scale
		auto scalePropertyContainer = mmake<Widget>();
		scalePropertyContainer->name = "scale";
		scalePropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(scalePropertyContainer);

		auto scaleIcon = o2UI.CreateImage("ui/UI4_scale_icon.png");
		*scaleIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		scalePropertyContainer->AddChild(scaleIcon);

		mScaleProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mScaleProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mScaleProperty->SetValuePath("transform/scale");
		mScaleProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		scalePropertyContainer->AddChild(mScaleProperty);

		// Rotation
		auto rotationAndShearPropertyContainer = mmake<Widget>();
		rotationAndShearPropertyContainer->name = "rotation and depth";
		rotationAndShearPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rotationAndShearPropertyContainer);

		auto rotateIcon = o2UI.CreateImage("ui/UI4_rotate_icon.png");
		*rotateIcon->layout = WidgetLayout(Vec2F(0, 0), Vec2F(0.0f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		rotationAndShearPropertyContainer->AddChild(rotateIcon);

		mRotationProperty = o2UI.CreateWidget<FloatProperty>();
		*mRotationProperty->layout = WidgetLayout(Vec2F(0, 0), Vec2F(0.5f, 1.0f), Vec2F(40, 0), Vec2F(10, 0));
		mRotationProperty->SetValuePath("transform/angleDegree");
		mRotationProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		rotationAndShearPropertyContainer->AddChild(mRotationProperty);

		// Shear
		auto shearIcon = o2UI.CreateImage("ui/UI4_shear_icon.png");
		*shearIcon->layout = WidgetLayout(Vec2F(0.5f, 0), Vec2F(0.5f, 1.0f), Vec2F(10, 0), Vec2F(30, 0));
		rotationAndShearPropertyContainer->AddChild(shearIcon);

		mShearProperty = o2UI.CreateWidget<FloatProperty>();
		*mShearProperty->layout = WidgetLayou#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename Type>
class DynamicCast
{
public:
    template<typename CastType>
    static Ref<CastType> cast(const Ref<Type>& ptr)
    {
        return std::dynamic_pointer_cast<CastType>(ptr);
    }
};

class Vec2F
{
public:
    Vec2F(float x, float y) : mX(x), mY(y) {}

private:
    float mX;
    float mY;
};

void OnPropertyChangeCompleted() {}

class WidgetLayout
{
public:
    enum class BaseCorner { LeftTop };

    static WidgetLayout Based(BaseCorner corner, const Vec2F& size, const Vec2F& offset)
    {
        return WidgetLayout();
    }

    static WidgetLayout HorStretch(VerAlign align, float height, float left, float right, float margin)
    {
        return WidgetLayout();
    }

private:
    WidgetLayout() {}
};

class Widget
{
public:
    std::string name;
    Ref<WidgetLayout> layout;
};

class Label
{
public:
    std::string text;
};

template<typename T>
class WidgetChild
{
public:
    T* GetChildByType(const std::string& type) { return nullptr; }
};

class Spoiler : public Widget
{
public:
    std::string caption;
    int spacing;
    bool fitByChildren;
    bool expandWidth;
    bool expandHeight;
    template<typename T>
    void AddChild(const Ref<T>& child) {}
};

class Image : public Widget
{
public:
    void* layout;
    Image(const std::string& path) {}
};

template<typename T>
class Property : public Widget
{
public:
    std::string name;
    const char* caption;
    std::function<void()> onChangeCompleted;
};

class Vec2FProperty : public Property<Vec2F>
{
public:
    Vec2FProperty(const std::string& color) {}
    void SetValuePath(const std::string& path) {}
};

template<typename T>
class o2UI
{
public:
    static Ref<T> CreateWidget(const std::string& type) { return mmake<T>(); }
    static Ref<Image> CreateImage(const std::string& path) { return mmake<Image>(path); }
};

class MainWindow
{
public:
    MainWindow()
    {
        mShearProperty = mmake<Vec2FProperty>("colored");
        mShearProperty->SetValuePath("drawDepth");
        mShearProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);

        rotationAndShearPropertyContainer->AddChild(mShearProperty);

        mLayoutSpoiler = o2UI<Spoiler>::CreateWidget("expand with caption");
        mLayoutSpoiler->name = "Layout";
        mLayoutSpoiler->caption = "Layout";
        mLayoutSpoiler->spacing = 5;
        mLayoutSpoiler->fitByChildren = true;
        mLayoutSpoiler->expandWidth = true;
        mLayoutSpoiler->expandHeight = false;
        mSpoiler->AddChild(mLayoutSpoiler);

        auto rightTopAnchorPropertyContainer = mmake<Widget>();
        rightTopAnchorPropertyContainer->name = "right top anchor";
        rightTopAnchorPropertyContainer->layout->minHeight = 20;
        mLayoutSpoiler->AddChild(rightTopAnchorPropertyContainer);

        auto anchorIcon = o2UI<Image>::CreateImage("ui/UI4_icon_anchor.png");
        *anchorIcon->layout = WidgetLayout::Based(WidgetLayout::BaseCorner::LeftTop,
                                                    Vec2F(20, 20), Vec2F(0, 0));
        rightTopAnchorPropertyContainer->AddChild(anchorIcon);

        mAnchorRightTopProperty = o2UI<Vec2FProperty>::CreateWidget("colored");
        *mAnchorRightTopProperty->layout = WidgetLayout::HorStretch(WidgetLayout::VerAlign::Top,
                                                                    20, 0, 20, 0);
        mAnchorRightTopProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "R";
        mAnchorRightTopProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "T";
        mAnchorRightTopProperty->SetValuePath("layout/anchorMax");
        mAnchorRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
        rightTopAnchorPropertyContainer->AddChild(mAnchorRightTopProperty);

        auto leftBottomAnchorPropertyContainer = mmake<Widget>();
        leftBottomAnchorPropertyContainer->name = "left bottom anchor";
        leftBottomAnchorPropertyContainer->layout->minHeight = 20;
        mLayoutSpoiler->AddChild(leftBottomAnchorPropertyContainer);

        mAnchorLeftBottomProperty = o2UI<Vec2FProperty>::CreateWidget("colored");
        *mAnchorLeftBottomProperty->layout = WidgetLayout::HorStretch(WidgetLayout::VerAlign::Top,
                                                                      20, 0, 20, 0);
        mAnchorLeftBottomProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "L";
        mAnchorLeftBottomProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "B";
        mAnchorLeftBottomProperty->SetValuePath("layout/anchorMin");
        mAnchorLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
        leftBottomAnchorPropertyContainer->AddChild(mAnchorLeftBottomProperty);

        auto rightTopOffsetPropertyContainer = mmake<Widget>();
        rightTopOffsetPropertyContainer->name = "right top offset";
        rightTopOffsetPropertyContainer->layout->minHeight = 20;
        mLayoutSpoiler->AddChild(rightTopOffsetPropertyContainer);

        auto offsetIcon = o2UI<Image>::CreateImage("ui/UI4_icon_offsets.png");
        *offsetIcon->layout = WidgetLayout::Based(WidgetLayout::BaseCorner::LeftTop,
                                                  Vec2F(20, 20), Vec2F(0, 0));
        rightTopOffsetPropertyContainer->AddChild(offsetIcon);

        moffsetRightTopProperty = o2UI<Vec2FProperty>::CreateWidget("colored");
        *moffsetRightTopProperty->layout = WidgetLayout::HorStretch(WidgetLayout::VerAlign::Top,
                                                                    20, 0, 20, 0);
        moffsetRightTopProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "R";
        moffsetRightTopProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "T";
        moffsetRightTopProperty->SetValuePath("layout/offsetMax");
        moffsetRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
        rightTopOffsetPropertyContainer->AddChild(moffsetRightTopProperty);

        auto leftBottomOffsetPropertyContainer = mmake<Widget>();
        leftBottomOffsetPropertyContainer->name = "left bottom offset";
        leftBottomOffsetPropertyContainer->layout->minHeight = 20;
        mLayoutSpoiler->AddChild(leftBottomOffsetPropertyContainer);

        mOffsetLeftBottomProperty = o2UI<Vec2FProperty>::CreateWidget("colored");
        *mOffsetLeftBottomProperty->layout = WidgetLayout::HorStretch(WidgetLayout::VerAlign::Top,
                                                                      20, 0, 20, 0);
        mOffsetLeftBottomProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "L";
        mOffsetLeftBottomProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "B";
        mOffsetLeftBottomProperty->SetValuePath("layout/offsetMin");
        mOffsetLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
        leftBottomOffsetPropertyContainer->AddChild(mOffsetLeftBottomProperty);

        auto minSizePropertyContainer = mmake<Widget>();
        minSizePropertyContainer->name = "right top anchor";
        minSizePropertyContainer->layout->minHeight = 20;
        mLayoutSpoiler->AddChild(minSizePropertyContainer);

        auto minSizeIcon = o2UI<Image>::CreateImage("ui/UI4_icon_min_size.png");
        *minSizeIcon->layout = WidgetLayout::Based(WidgetLayout::BaseCorner::LeftTop,
                                                   Vec2F(20, 20), Vec2F(0, 0));
        minSizePropertyContainer->AddChild(minSizeIcon);

        mMinSizeProperty = o2UI<Vec2FProperty>::CreateWidget("colored");
        *mMinSizeProperty->layout = WidgetLayout::HorStretch(WidgetLayout::VerAlign::Top,
                                                             20, 0, 20, 0);
        mMinSizeProperty->SetValuePath("layout/minSize");
        mMinSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
        minSizePropertyContainer->AddChild(mMinSizeProperty);
    }

private:
    Ref<Vec2FProperty> mShearProperty;
    Ref<Spoiler> mLayoutSpoiler;
    Ref<Widget> rotationAndShearPropertyContainer;
    Ref<Widget> mSpoiler;
    Ref<Vec2FProperty> mAnchorRightTopProperty;
    Ref<Widget> mAnchorLeftBottomProperty;
    Ref<Widget> moffsetRightTopProperty;
    Ref<Widget> mOffsetLeftBottomProperty;
    Ref<Vec2FProperty> mMinSizeProperty;
};

int main()
{
    MainWindow mainWindow;
    return 0;
}#include <memory>

// Define Ref class
template<typename T>
class Ref
{
public:
    Ref(T* ptr)
        : m_ptr(ptr)
    {
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }

private:
    T* m_ptr;
};

// Define WeakRef class
template<typename T>
class WeakRef
{
public:
    WeakRef(T* ptr)
        : m_ptr(ptr)
    {
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }

private:
    T* m_ptr;
};

// Define mmake() function
template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

// Define DynamicCast function
template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return Ref<T>(dynamic_cast<T*>(ptr.operator->()));
}

class DefaultActorTransformViewer
{
public:
    DefaultActorTransformViewer()
    {
        // Constructor implementation
    }

    ~DefaultActorTransformViewer()
    {
        delete mSpoiler;
    }

    void SetTargetActors(const Vector<Actor*>& actors)
    {
        mTargetActors = actors;

        auto prototypes = actors.Convert<Actor*>([](Actor* x) { return x->GetPrototypeLink().Get(); });

        mPositionProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::position)>(
            actors, prototypes, [](Actor* x) { return &x->transform->position; });

        mPivotProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::pivot)>(
            actors, prototypes, [](Actor* x) { return &x->transform->pivot; });

        mScaleProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::scale)>(
            actors, prototypes, [](Actor* x) { return &x->transform->scale; });

        mSizeProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::size)>(
            actors, prototypes, [](Actor* x) { return &x->transform->size; });

        mRotationProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::angleDegree)>(
            actors, prototypes, [](Actor* x) { return &x->transform->angleDegree; });

        mShearProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::shear)>(
            actors, prototypes, [](Actor* x) { return &x->transform->shear; });

        Vector<Widget*> targetWidgets = mTargetActors
            .FindAll([](Actor* x) { return dynamic_cast<Widget*>(x) != nullptr; })
            .Convert<Widget*>([](Actor* x) { return dynamic_cast<Widget*>(x); });

        mLayoutEnabled = !targetWidgets.IsEmpty();
        mLayoutSpoiler->enabled = mLayoutEnabled;

        if (mLayoutEnabled)
        {
            auto widgetPrototypes = targetWidgets.Convert<Widget*>(
				[](Actor* x) { return dynamic_cast<Widget*>(x->GetPrototypeLink().Get()); });

            mAnchorRightTopProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::anchorMax)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->anchorMax; });

            mAnchorLeftBottomProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::anchorMin)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->anchorMin; });

            moffsetRightTopProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::offsetMax)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->offsetMax; });

            mOffsetLeftBottomProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::offsetMin)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->offsetMin; });

            mMinSizeProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::minSize)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->minSize; });

            mMaxSizeProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::maxSize)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->maxSize; });

            mWeightProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::weight)>(
                targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->weight; });
        }
    }

    void Refresh()
    {
        mPositionProperty->Refresh();
        mPivotProperty->Refresh();
        mScaleProperty->Refresh();
    }

private:
    Ref<Spoiler> mSpoiler;
    Ref<Vector<Actor*>> mTargetActors;
    Ref<Property> mPositionProperty;
    Ref<Property> mPivotProperty;
    Ref<Property> mScaleProperty;
    Ref<Property> mSizeProperty;
    Ref<Property> mRotationProperty;
    Ref<Property> mShearProperty;
    Ref<Widget> mLayoutSpoiler;
    bool mLayoutEnabled;
    Ref<Property> mAnchorRightTopProperty;
    Ref<Property> mAnchorLeftBottomProperty;
    Ref<Property> moffsetRightTopProperty;
    Ref<Property> mOffsetLeftBottomProperty;
    Ref<Property> mMinSizeProperty;
    Ref<Property> mMaxSizeProperty;
    Ref<Property> mWeightProperty;
};#include <Ref.h>

using namespace Editor;

class DefaultActorTransformViewer
{
private:
    Ref<> mSizeProperty;
    Ref<> mRotationProperty;
    Ref<> mShearProperty;
    Ref<> mAnchorRightTopProperty;
    Ref<> mAnchorLeftBottomProperty;
    Ref<> mOffsetRightTopProperty;
    Ref<> mOffsetLeftBottomProperty;
    Ref<> mMinSizeProperty;
    Ref<> mMaxSizeProperty;
    Ref<> mWeightProperty;
    bool mLayoutEnabled;

public:
    DefaultActorTransformViewer()
        : mSizeProperty(mmake<>()), mRotationProperty(mmake<>()), mShearProperty(mmake<>()), mAnchorRightTopProperty(mmake<>()), mAnchorLeftBottomProperty(mmake<>()), mOffsetRightTopProperty(mmake<>()), mOffsetLeftBottomProperty(mmake<>()), mMinSizeProperty(mmake<>()), mMaxSizeProperty(mmake<>()), mWeightProperty(mmake<>()), mLayoutEnabled(false)
    {}

    void RefreshProperties()
    {
        mSizeProperty->Refresh();
        mRotationProperty->Refresh();
        mShearProperty->Refresh();

        if (mLayoutEnabled)
        {
            mAnchorRightTopProperty->Refresh();
            mAnchorLeftBottomProperty->Refresh();
            mOffsetRightTopProperty->Refresh();
            mOffsetLeftBottomProperty->Refresh();
            mMinSizeProperty->Refresh();
            mMaxSizeProperty->Refresh();
            mWeightProperty->Refresh();
        }
    }

    void OnPropertyChangeCompleted(const String& path,
                                   const Vector<DataDocument>& prevValue,
                                   const Vector<DataDocument>& newValue)
    {
        PropertyChangeAction* action = mnew PropertyChangeAction(
            o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

        o2EditorApplication.DoneAction(action);
    }
};

// --- META ---

DECLARE_CLASS(DefaultActorTransformViewer, Editor__DefaultActorTransformViewer);

// --- END META ---