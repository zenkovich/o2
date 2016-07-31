#pragma once

#include "Render/IDrawable.h"
#include "Utils/Math/Transform.h"
#include "Utils/Math/Color.h"
#include "Utils/Property.h"

namespace o2
{
	// -----------------------------
	// Basic rect drawable interface
	// -----------------------------
	class IRectDrawable: public Transform, public virtual IDrawable
	{
	public:
		Property<Color4> color;        // Color property
		Property<float>  transparency; // Transparency property, changing alpha in color
		Property<bool>   enabled;      // Enable property

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

		// Drawing
		virtual void Draw() {}

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
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(IRectDrawable);

	protected:
		Color4 mColor;   // Color @SERIALIZABLE
		bool   mEnabled; // True, when drawable enabled and needs to draw @SERIALIZABLE

	protected:
		// Calls when color was changed
		virtual void ColorChanged() {}

		// Calls when enabling changed
		virtual void EnableChanged() {}

		// Initializing properties
		void InitializeProperties();
	};
}