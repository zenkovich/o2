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
	class IRectDrawable: public Transform, public virtual IDrawable
	{
	public:
		PROPERTIES(IRectDrawable);
		PROPERTY(Color4, color, SetColor, GetColor);                     // Color property
		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Transparency property, changing alpha in color
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);                  // Enable property

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
		// It is called when color was changed
		virtual void ColorChanged() {}

		// It is called when enabling changed
		virtual void EnableChanged() {}
	};
}

CLASS_BASES_META(o2::IRectDrawable)
{
	BASE_CLASS(o2::Transform);
	BASE_CLASS(o2::IDrawable);
}
END_META;
CLASS_FIELDS_META(o2::IRectDrawable)
{
	FIELD().NAME(color).PUBLIC();
	FIELD().NAME(transparency).PUBLIC();
	FIELD().NAME(enabled).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mColor).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::IRectDrawable)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetColor);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, ColorChanged);
	PROTECTED_FUNCTION(void, EnableChanged);
}
END_META;
