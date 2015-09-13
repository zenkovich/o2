#pragma once

#include "Utils/Math/Basis.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"

namespace o2
{
	class Transform
	{
	public:
		Property<Vec2F>  Position;     // Position property
		Property<Vec2F>  Size;         // Size property
		Property<Vec2F>  Scale;        // Scale property
		Property<Vec2F>  Pivot;        // Pivot property, in local space
		Property<Vec2F>  WorldPivot;   // Pivot property, in world space
		Property<Vec2F>  RelPivot;     // Relative pivot property, in local scale, where (0, 0) - left down corner, (1, 1) - right top
		Property<RectF>  Rect;         // Rect property. Sets the position and size
		Property<float>  Angle;        // Rotation angle in radians
		Property<float>  Shear;        // Shear
		Property<Basis>  ResBasis;     // Transformation basis
		Property<RectF>  AABB;         // Axis aligned rectangle
		Property<Vec2F>  Right;        // X Axis direction property
		Property<Vec2F>  Left;         // Negative X Axis direction property
		Property<Vec2F>  Up;           // Y Axis direction property
		Property<Vec2F>  Down;         // Negative Y Axis direction property
		Setter<Vec2F>    LookAtPoint;  // Look at point setter

									   // Constructor
		Transform(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
				  const Vec2F& scale = Vec2F(1.0f, 1.0f), const Vec2F& pivot = Vec2F());

		// Copy-constructor
		Transform(const Transform& other);

		// Virtual destructor
		virtual ~Transform() {}

		// Assign operator
		Transform& operator=(const Transform& other);
		
		// Sets position
		virtual void SetPosition(const Vec2F& position);

		// Returns position
		virtual Vec2F GetPosition() const;

		// Sets size
		virtual void SetSize(const Vec2F& size);

		// Return size
		virtual Vec2F GetSize() const;

		// Sets pivot, in local space
		virtual void SetPivot(const Vec2F& pivot);

		// Return pivot, in local space
		virtual Vec2F GetPivot() const;

		// Sets pivot by world coordinates
		virtual void SetWorldPivot(const Vec2F& pivot);

		// Returns pivot position in world coordinates
		virtual Vec2F GetWorldPivot() const;

		// Sets relative pivot, in local space, where (0, 0) - left down corner, (1, 1) - right top
		virtual void SetRelativePivot(const Vec2F& relPivot);

		// Returns relative pivot, in local space, where (0, 0) - left down corner, (1, 1) - right top
		virtual Vec2F GetRelativePivot() const;

		// Sets rect
		virtual void SetRect(const RectF& rect);

		// Returns rect
		virtual RectF GetRect() const;

		// Sets scale
		virtual void SetScale(const Vec2F& scale);

		// Returns scale
		virtual Vec2F GetScale() const;

		// Sets rotation angle, in radians
		virtual void SetAngle(float rad);

		// Returns rotation angle in radians
		virtual float GetAngle() const;

		// Sets shear
		virtual void SetShear(float shear);

		// Returns shear
		virtual float GetShear() const;

		// Sets basis
		virtual void SetBasis(const Basis& basis);

		// Returns basis
		virtual Basis GetBasis() const;

		// Sets axis aligned rectangle transformation
		virtual void SetAxisAlignedRect(const RectF& rect);

		// Returns axis aligned rectangle transformation
		virtual RectF GetAxisAlignedRect() const;

		// Sets left top corner position
		virtual void SetLeftTop(const Vec2F& position);

		// Returns left top corner position
		virtual Vec2F GetLeftTop() const;

		// Sets right top corner position
		virtual void SetRightTop(const Vec2F& position);

		// Returns right top corner position
		virtual Vec2F GetRightTop() const;

		// Sets left down corner position
		virtual void SetLeftBottom(const Vec2F& position);

		// Returns left down corner position
		virtual Vec2F GetLeftBottom() const;

		// Sets left right bottom position
		virtual void SetRightBottom(const Vec2F& position);

		// Returns right bottom corner position
		virtual Vec2F GetRightBottom() const;

		// Sets center position
		virtual void SetCenter(const Vec2F& position);

		// Returns center position
		virtual Vec2F GetCenter() const;

		// Set local x axis direction
		virtual void SetRight(const Vec2F& dir);

		// Returns local x axis direction
		virtual Vec2F GetRight() const;

		// Set negative local x axis direction
		virtual void SetLeft(const Vec2F& dir);

		// Returns negative local x axis direction
		virtual Vec2F GetLeft() const;

		// Set local y axis direction
		virtual void SetUp(const Vec2F& dir);

		// Returns local y axis direction
		virtual Vec2F GetUp() const;

		// Set negative local y axis direction
		virtual void SetDown(const Vec2F& dir);

		// Returns negative local y axis direction
		virtual Vec2F GetDown() const;

		// Sets x axis directed to worldPoint
		virtual void LookAt(const Vec2F& worldPoint);

		// Transforms point from world space into local
		Vec2F World2LocalPoint(const Vec2F& worldPoint) const;

		// Transforms point from local space into world
		Vec2F Local2WorldPoint(const Vec2F& localPoint) const;

		// Transforms direction from world space into local
		Vec2F World2LocalDir(const Vec2F& worldDir) const;

		// Transforms direction from local space into world
		Vec2F Local2WorldDir(const Vec2F& localDir) const;

	protected:
		Vec2F  mPosition;  // Position
		Vec2F  mSize;      // Size
		Vec2F  mScale;     // Scale, (1; 1) is default
		Vec2F  mPivot;     // Pivot, in local space
		float  mAngle;     // Rotation angle in radians
		float  mShear;     // Shear

		Basis  mTransform; // Final transform basis

	protected:
		// Calls when basis changed
		virtual void BasisChanged() {}

		// Updates mTransform 
		void UpdateTransform();

		// Initializing properties
		void InitializeProperties();
	};
}
