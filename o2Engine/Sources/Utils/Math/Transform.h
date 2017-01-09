#pragma once

#include "Animation/Animation.h"
#include "Utils/IObject.h"
#include "Utils/Math/Basis.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	class Transform: public ISerializable
	{
	public:
		Property<Vec2F>  position;      // Position property
		Property<Vec2F>  size;          // Size property
		Property<Vec2F>  scale;         // Scale property
		Property<Vec2F>  pivot;         // Pivot property, in local space
		Property<Vec2F>  worldPivot;    // Pivot property, in world space
		Property<Vec2F>  szPivot;       // Pivot in size space property
		Property<RectF>  rect;          // Rectangle property. Sets the position and size
		Property<float>  angle;         // Rotation angle in radians
		Property<float>  angleDegree;   // Rotation angle in degrees
		Property<float>  shear;         // Shear property
		Property<Basis>  basis;         // Transformation basis property
		Property<Basis>  nonSizedBasis; // Non sizes transformation basis property
		Property<RectF>  AABB;          // Axis aligned rectangle
		Property<Vec2F>  leftTop;       // Left top corner property
		Property<Vec2F>  leftBottom;    // Left bottom corner property
		Property<Vec2F>  rightTop;      // Left top corner property
		Property<Vec2F>  rightBottom;   // Left top corner property
		Property<Vec2F>  right;         // X Axis direction property
		Property<Vec2F>  left;          // Negative X Axis direction property
		Property<Vec2F>  up;            // Y Axis direction property
		Property<Vec2F>  down;          // Negative Y Axis direction property
		Setter<Vec2F>    lookAtPoint;   // Look at point setter

									   // Constructor
		Transform(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
				  const Vec2F& scale = Vec2F(1.0f, 1.0f), const Vec2F& pivot = Vec2F(0.5f, 0.5f));

		// Copy-constructor
		Transform(const Transform& other);

		// Virtual destructor
		virtual ~Transform() {}

		// Assign operator
		Transform& operator=(const Transform& other);

		// Check equals operator
		bool operator==(const Transform& other) const;
		
		// Sets position
		virtual void SetPosition(const Vec2F& position);

		// Returns position
		virtual Vec2F GetPosition() const;

		// Sets size
		virtual void SetSize(const Vec2F& size);

		// Return size
		virtual Vec2F GetSize() const;

		// Sets pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		virtual void SetPivot(const Vec2F& pivot);

		// Return pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		virtual Vec2F GetPivot() const;

		// Sets pivot by world coordinates
		virtual void SetWorldPivot(const Vec2F& pivot);

		// Returns pivot position in world coordinates
		virtual Vec2F GetWorldPivot() const;

		// Sets size pivot, in local space, where (0, 0) - left down corner, (mSize.x, mSize.y) - right top
		virtual void SetSizePivot(const Vec2F& relPivot);

		// Returns size pivot, in local space, where (0, 0) - left down corner, (mSize.x, mSize.y) - right top
		virtual Vec2F GetSizePivot() const;

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

		// Sets rotation angle, in degrees
		virtual void SetAngleDegrees(float deg);

		// Returns rotation angle in degrees
		virtual float GetAngleDegrees() const;

		// Sets shear
		virtual void SetShear(float shear);

		// Returns shear
		virtual float GetShear() const;

		// Sets basis
		virtual void SetBasis(const Basis& basis);

		// Returns basis
		virtual Basis GetBasis() const;

		// Sets basis without size
		virtual void SetNonSizedBasis(const Basis& basis);

		// Returns basis without size
		virtual Basis GetNonSizedBasis() const;

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
		virtual Vec2F World2LocalPoint(const Vec2F& worldPoint) const;

		// Transforms point from local space into world
		virtual Vec2F Local2WorldPoint(const Vec2F& localPoint) const;

		// Transforms direction from world space into local
		virtual Vec2F World2LocalDir(const Vec2F& worldDir) const;

		// Transforms direction from local space into world
		virtual Vec2F Local2WorldDir(const Vec2F& localDir) const;

		// Returns true when point inside this
		virtual bool IsPointInside(const Vec2F& point) const;

		SERIALIZABLE(Transform);

	protected:
		Vec2F  mPosition;          // Position @SERIALIZABLE
		Vec2F  mSize;              // Size @SERIALIZABLE
		Vec2F  mScale;             // Scale, (1; 1) is default @SERIALIZABLE
		Vec2F  mPivot;             // Pivot: (0; 0) is left bottom corner - (1; 1) is right top corner @SERIALIZABLE
		float  mAngle;             // Rotation angle in radians @SERIALIZABLE
		float  mShear;             // Shear @SERIALIZABLE
						           
		Basis  mTransform;         // Final transform basis
		Basis  mNonSizedTransform; // Final transform basis without size

	protected:
		// Calls when basis changed
		virtual void BasisChanged() {}

		// Calls when object was deserialized
		void OnDeserialized(const DataNode& node);

		// Updates mTransform 
		virtual void UpdateTransform();

		// Initializing properties
		void InitializeProperties();
	};
}
