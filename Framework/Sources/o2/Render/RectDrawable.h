#pragma once

#include "o2/Render/IDrawable.h"
#include "o2/Utils/Math/Transform.h"
#include "o2/Utils/Math/Color.h"
#include "o2/Utils/Property.h"

namespace o2
{
    // -----------------------------
    // Basic rect drawable interface
    // -----------------------------
    class IRectDrawable: public Transform, public virtual IDrawable, public RefCounterable
    {
    public:
        PROPERTIES(IRectDrawable);
        PROPERTY(Color4, color, SetColor, GetColor);                     // Color property @SCRIPTABLE
        PROPERTY(float, transparency, SetTransparency, GetTransparency); // Transparency property, changing alpha in color @SCRIPTABLE
        PROPERTY(bool, enabled, SetEnabled, IsEnabled);                  // Enable property @SCRIPTABLE

    public:
        // Constructor
        IRectDrawable(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f, 
                      const Vec2F& scale = Vec2F(1.0f, 1.0f), const Color4& color = Color4::White(), 
                      const Vec2F& pivot = Vec2F(0.5f, 0.5f));

        // Copy-constructor
        IRectDrawable(const IRectDrawable& other);

        // Virtual destructor
        virtual ~IRectDrawable() {}

        // Assign operator
        IRectDrawable& operator=(const IRectDrawable& other);

        // Equals operator
        bool operator==(const IRectDrawable& other) const;

        // Not equals operator
        bool operator!=(const IRectDrawable& other) const;

        // Drawing
        virtual void Draw() override {}

        // Sets color
        virtual void SetColor(const Color4& color);

        // Returns color
        virtual Color4 GetColor() const;

        // Sets transparency. Changing color alpha
        virtual void SetTransparency(float transparency);

        // Returns transparency(color alpha)
        virtual float GetTransparency() const;

        // Sets enabled
        virtual void SetEnabled(bool enabled);

        // Returns enabled
        virtual bool IsEnabled() const;

        // Returns true if point is under drawable
        bool IsUnderPoint(const Vec2F& point) override;

        SERIALIZABLE(IRectDrawable);

    protected:
        Color4 mColor;          // Color @SERIALIZABLE
        bool   mEnabled = true; // True, when drawable enabled and needs to draw @SERIALIZABLE

    protected:
        // Called when color was changed
        virtual void ColorChanged() {}

        // Called when enabling changed
        virtual void EnableChanged() {}
    };

    // -----------------------------
    // Functional rectangle drawable
    // -----------------------------
    class FunctionalRectDrawable: public IRectDrawable
    {
    public:
        Function<void(const Basis& transform, const Color4& color)> draw; // Draw function

    public:
        // Default constructor
        FunctionalRectDrawable();

        // Default constructor woth parameters
        FunctionalRectDrawable(const Function<void(const Basis& transform, const Color4& color)>& draw,
                               const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
                               const Vec2F& scale = Vec2F(1.0f, 1.0f), const Color4& color = Color4::White(),
                               const Vec2F& pivot = Vec2F(0.5f, 0.5f));

        // Copy constructor
        FunctionalRectDrawable(const FunctionalRectDrawable& other);

        // Calls draw function to draw
        void Draw() override;

        SERIALIZABLE(FunctionalRectDrawable);
    };
}
// --- META ---

CLASS_BASES_META(o2::IRectDrawable)
{
    BASE_CLASS(o2::Transform);
    BASE_CLASS(o2::IDrawable);
    BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(o2::IRectDrawable)
{
    FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(color);
    FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(transparency);
    FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(enabled);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mColor);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mEnabled);
}
END_META;
CLASS_METHODS_META(o2::IRectDrawable)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(const Vec2F&, const Vec2F&, float, const Vec2F&, const Color4&, const Vec2F&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const IRectDrawable&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, SetColor, const Color4&);
    FUNCTION().PUBLIC().SIGNATURE(Color4, GetColor);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTransparency, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetTransparency);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, ColorChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, EnableChanged);
}
END_META;

CLASS_BASES_META(o2::FunctionalRectDrawable)
{
    BASE_CLASS(o2::IRectDrawable);
}
END_META;
CLASS_FIELDS_META(o2::FunctionalRectDrawable)
{
    FIELD().PUBLIC().NAME(draw);
}
END_META;
CLASS_METHODS_META(o2::FunctionalRectDrawable)
{

    typedef const Function<void(const Basis& transform, const Color4& color)>& _tmp1;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(_tmp1, const Vec2F&, const Vec2F&, float, const Vec2F&, const Color4&, const Vec2F&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const FunctionalRectDrawable&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
}
END_META;
// --- END META ---
