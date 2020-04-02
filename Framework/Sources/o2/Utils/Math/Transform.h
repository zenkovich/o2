#pragma once

#include "o2/Animation/AnimationClip.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Editor/Attributes/AnimatableAttribute.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Math/Basis.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Transform : virtual public ISerializable
	{
	public:
		PROPERTIES(Transform);
		PROPERTY(Vec2F, position, SetPosition, GetPosition);            // Position property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, size, SetSize, GetSize);                        // Size property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, scale, SetScale, GetScale);                     // Scale property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, pivot, SetPivot, GetPivot);                     // Pivot property, in local space @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, worldPivot, SetWorldPivot, GetWorldPivot);      // Pivot property, in world space @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, szPivot, SetSizePivot, GetSizePivot);           // Pivot in size space property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(float, angle, SetAngle, GetAngle);                     // Rotation angle in radians @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(float, angleDegree, SetAngleDegrees, GetAngleDegrees); // Rotation angle in degrees @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(float, shear, SetShear, GetShear);                     // Shear property @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(Basis, basis, SetBasis, GetBasis);                         // Transformation basis property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Basis, nonSizedBasis, SetNonSizedBasis, GetNonSizedBasis); // Non sizes transformation basis property @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(RectF, rect, SetRect, GetRect);                       // Rectangle property. Sets the position and size @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(RectF, AABB, SetAxisAlignedRect, GetAxisAlignedRect); // Axis aligned rectangle @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(Vec2F, leftTop, SetLeftTop, GetLeftTop);             // Left top corner property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, leftBottom, SetLeftBottom, GetLeftBottom);    // Left bottom corner property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, rightTop, SetRightTop, GetRightTop);          // Left top corner property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, rightBottom, SetRightBottom, GetRightBottom); // Left top corner property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, center, SetCenter, GetCenter);                // Center property @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(Vec2F, right, SetRight, GetRight); // X Axis direction property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, left, SetLeft, GetLeft);    // Negative X Axis direction property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, up, SetUp, GetUp);          // Y Axis direction property @EDITOR_IGNORE @ANIMATABLE
		PROPERTY(Vec2F, down, SetDown, GetDown);    // Negative Y Axis direction property @EDITOR_IGNORE @ANIMATABLE

		SETTER(Vec2F, lookAtPoint, LookAt); // Look at point setter @EDITOR_IGNORE @ANIMATABLE

	public:
		// Constructor
		Transform(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
				  const Vec2F& scale = Vec2F(1.0f, 1.0f), const Vec2F& pivot = Vec2F(0.5f, 0.5f));

		// Copy-constructor
		Transform(const Transform& other);

		// Virtual destructor
		virtual ~Transform() { }

		// Assign operator
		Transform& operator=(const Transform& other);

		// Check equals operator
		bool operator==(const Transform& other) const;

		// Not equals operator
		bool operator!=(const Transform& other) const;

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

		// Sets rect. If bySIze is true, rectangle adjusting by transform's size, overwise by scale
		virtual void SetRect(const RectF& rect, bool bySize = true);

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
		Vec2F  mPosition; // Position @SERIALIZABLE
		Vec2F  mSize;     // Size @SERIALIZABLE
		Vec2F  mScale;    // Scale, (1; 1) is default @SERIALIZABLE
		Vec2F  mPivot;    // Pivot: (0; 0) is left bottom corner - (1; 1) is right top corner @SERIALIZABLE
		float  mAngle;    // Rotation angle in radians @SERIALIZABLE
		float  mShear;    // Shear @SERIALIZABLE

		Basis  mTransform;         // Final transform basis
		Basis  mNonSizedTransform; // Final transform basis without size

	protected:
		// It is called when basis changed
		virtual void BasisChanged() { }

		// It is called when object was deserialized
		void OnDeserialized(const DataNode& node) override;

		// Updates mTransform 
		virtual void UpdateTransform();
	};
}

CLASS_BASES_META(o2::Transform)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Transform)
{
	PUBLIC_FIELD(position).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(size).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(scale).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(pivot).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(worldPivot).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(szPivot).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(angle).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(angleDegree).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(shear).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(basis).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(nonSizedBasis).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(rect).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(AABB).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(leftTop).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(leftBottom).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(rightTop).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(rightBottom).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(center).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(right).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(left).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(up).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(down).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(lookAtPoint).ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE();
	PROTECTED_FIELD(mPosition).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSize).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScale).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPivot).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAngle).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mShear).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTransform);
	PROTECTED_FIELD(mNonSizedTransform);
}
END_META;
CLASS_METHODS_META(o2::Transform)
{

	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(void, SetWorldPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldPivot);
	PUBLIC_FUNCTION(void, SetSizePivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSizePivot);
	PUBLIC_FUNCTION(void, SetRect, const RectF&, bool);
	PUBLIC_FUNCTION(RectF, GetRect);
	PUBLIC_FUNCTION(void, SetScale, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetScale);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PUBLIC_FUNCTION(float, GetAngle);
	PUBLIC_FUNCTION(void, SetAngleDegrees, float);
	PUBLIC_FUNCTION(float, GetAngleDegrees);
	PUBLIC_FUNCTION(void, SetShear, float);
	PUBLIC_FUNCTION(float, GetShear);
	PUBLIC_FUNCTION(void, SetBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetBasis);
	PUBLIC_FUNCTION(void, SetNonSizedBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetNonSizedBasis);
	PUBLIC_FUNCTION(void, SetAxisAlignedRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetAxisAlignedRect);
	PUBLIC_FUNCTION(void, SetLeftTop, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeftTop);
	PUBLIC_FUNCTION(void, SetRightTop, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRightTop);
	PUBLIC_FUNCTION(void, SetLeftBottom, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeftBottom);
	PUBLIC_FUNCTION(void, SetRightBottom, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRightBottom);
	PUBLIC_FUNCTION(void, SetCenter, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetCenter);
	PUBLIC_FUNCTION(void, SetRight, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRight);
	PUBLIC_FUNCTION(void, SetLeft, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeft);
	PUBLIC_FUNCTION(void, SetUp, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetUp);
	PUBLIC_FUNCTION(void, SetDown, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetDown);
	PUBLIC_FUNCTION(void, LookAt, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, World2LocalPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, Local2WorldPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, World2LocalDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, Local2WorldDir, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsPointInside, const Vec2F&);
	PROTECTED_FUNCTION(void, BasisChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, UpdateTransform);
}
END_META;
