#pragma once

#include "Utils/Math/Transform.h"

namespace o2
{
	class Actor;

	// ---------------
	// Actor transform
	// ---------------
	class ActorTransform: public ISerializable
	{
	public:
		class Data: public ISerializable
		{
		public:
			Vec2F  mPosition;                   // Position @SERIALIZABLE
			Vec2F  mSize;                       // Size @SERIALIZABLE
			Vec2F  mScale;                      // Scale, (1; 1) is default @SERIALIZABLE
			Vec2F  mPivot;                      // Pivot: (0; 0) is left bottom corner - (1; 1) is right top corner @SERIALIZABLE
			float  mAngle;                      // Rotation angle in radians @SERIALIZABLE
			float  mShear;                      // Shear @SERIALIZABLE
									            
			Basis  mTransform;                  // Final transform basis
			Basis  mNonSizedTransform;          // Final transform basis without size

			Basis  mWorldNonSizedTransform;     // World transform without size
			Basis  mWorldTransform;             // Result world basis
			Basis  mParentInvertedTransform;    // Parent world transform inverted
			Basis  mParentTransform;            // Parent world transform
			bool   mIsParentInvTransformActual; // Is mParentInvertedTransform is actual
			Actor* mOwner;                      // Owner actor

			SERIALIZABLE(Data);
		};

	public:
		Property<Vec2F>  position;           // Position property
		Property<Vec2F>  size;               // Size property
		Property<Vec2F>  scale;              // Scale property
		Property<Vec2F>  pivot;              // Pivot property, in local space
		Property<Vec2F>  worldPivot;         // Pivot property, in world space
		Property<Vec2F>  szPivot;            // Pivot in size space property
		Property<RectF>  rect;               // Rectangle property. Sets the position and size
		Property<float>  angle;              // Rotation angle in radians
		Property<float>  angleDegree;        // Rotation angle in degrees
		Property<float>  shear;              // Shear property
		Property<Basis>  basis;              // Transformation basis property
		Property<Basis>  nonSizedBasis;      // Non sizes transformation basis property
		Property<RectF>  AABB;               // Axis aligned rectangle
		Property<Vec2F>  leftTop;            // Left top corner property
		Property<Vec2F>  leftBottom;         // Left bottom corner property
		Property<Vec2F>  rightTop;           // Left top corner property
		Property<Vec2F>  rightBottom;        // Left top corner property
		Property<Vec2F>  right;              // X Axis direction property
		Property<Vec2F>  left;               // Negative X Axis direction property
		Property<Vec2F>  up;                 // Y Axis direction property
		Property<Vec2F>  down;               // Negative Y Axis direction property
		Setter<Vec2F>    lookAtPoint;        // Look at point setter
		Getter<Actor*>   actor;              // Owner actor getter
		Property<Vec2F>  worldPosition;      // World position property
		Property<RectF>  worldRect;          // World rectangle property. Sets the position and size
		Property<float>  worldAngle;         // World rotation angle in radians
		Property<Basis>  worldBasis;         // World transformation basis
		Property<Basis>  worldNonSizedBasis; // World transformation basis without size
		Property<RectF>  worldAABB;          // World axis aligned rectangle

		ActorTransform(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
					   const Vec2F& scale = Vec2F(1.0f, 1.0f), const Vec2F& pivot = Vec2F(0.5f, 0.5f));

		// Copy-constructor
		ActorTransform(const ActorTransform& other);

		// Assign operator
		ActorTransform& operator=(const ActorTransform& other);

		// Assign operator
		ActorTransform& operator=(const Transform& other);

		// Check EqualSid operator
		bool operator==(const ActorTransform& other) const;

		// Sets position
		void SetPosition(const Vec2F& position);

		// Returns position
		Vec2F GetPosition() const;

		// Sets size
		void SetSize(const Vec2F& size);

		// Return size
		Vec2F GetSize() const;

		// Sets pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		void SetPivot(const Vec2F& pivot);

		// Return pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		Vec2F GetPivot() const;

		// Sets pivot by world coordinates
		void SetWorldPivot(const Vec2F& pivot);

		// Returns pivot position in world coordinates
		Vec2F GetWorldPivot() const;

		// Sets size pivot, in local space, where (0, 0) - left down corner, (mSize.x, mSize.y) - right top
		void SetSizePivot(const Vec2F& relPivot);

		// Returns size pivot, in local space, where (0, 0) - left down corner, (mSize.x, mSize.y) - right top
		Vec2F GetSizePivot() const;

		// Sets rect
		void SetRect(const RectF& rect);

		// Returns rect
		RectF GetRect() const;

		// Sets scale
		void SetScale(const Vec2F& scale);

		// Returns scale
		Vec2F GetScale() const;

		// Sets rotation angle, in radians
		void SetAngle(float rad);

		// Returns rotation angle in radians
		float GetAngle() const;

		// Sets rotation angle, in degrees
		void SetAngleDegrees(float deg);

		// Returns rotation angle in degrees
		float GetAngleDegrees() const;

		// Sets shear
		void SetShear(float shear);

		// Returns shear
		float GetShear() const;

		// Sets basis
		void SetBasis(const Basis& basis);

		// Returns basis
		Basis GetBasis() const;

		// Sets basis without size
		void SetNonSizedBasis(const Basis& basis);

		// Returns basis without size
		Basis GetNonSizedBasis() const;

		// Sets axis aligned rectangle transformation
		void SetAxisAlignedRect(const RectF& rect);

		// Returns axis aligned rectangle transformation
		RectF GetAxisAlignedRect() const;

		// Sets left top corner position
		void SetLeftTop(const Vec2F& position);

		// Returns left top corner position
		Vec2F GetLeftTop() const;

		// Sets right top corner position
		void SetRightTop(const Vec2F& position);

		// Returns right top corner position
		Vec2F GetRightTop() const;

		// Sets left down corner position
		void SetLeftBottom(const Vec2F& position);

		// Returns left down corner position
		Vec2F GetLeftBottom() const;

		// Sets left right bottom position
		void SetRightBottom(const Vec2F& position);

		// Returns right bottom corner position
		Vec2F GetRightBottom() const;

		// Sets center position
		void SetCenter(const Vec2F& position);

		// Returns center position
		Vec2F GetCenter() const;

		// Set local x axis direction
		void SetRight(const Vec2F& dir);

		// Returns local x axis direction
		Vec2F GetRight() const;

		// Set negative local x axis direction
		void SetLeft(const Vec2F& dir);

		// Returns negative local x axis direction
		Vec2F GetLeft() const;

		// Set local y axis direction
		void SetUp(const Vec2F& dir);

		// Returns local y axis direction
		Vec2F GetUp() const;

		// Set negative local y axis direction
		void SetDown(const Vec2F& dir);

		// Returns negative local y axis direction
		Vec2F GetDown() const;

		// Sets x axis directed to worldPoint
		void LookAt(const Vec2F& worldPoint);

		// Transforms point from world space into local
		Vec2F World2LocalPoint(const Vec2F& worldPoint) const;

		// Transforms point from local space into world
		Vec2F Local2WorldPoint(const Vec2F& localPoint) const;

		// Transforms direction from world space into local
		Vec2F World2LocalDir(const Vec2F& worldDir) const;

		// Transforms direction from local space into world
		Vec2F Local2WorldDir(const Vec2F& localDir) const;

		// Returns true when point inside this
		bool IsPointInside(const Vec2F& point) const;

		// Returns owner actor
		Actor* GetOwnerActor() const;

		// Sets world position
		void SetWorldPosition(const Vec2F& position);

		// Returns world position
		Vec2F GetWorldPosition() const;

		// Sets world rect
		void SetWorldRect(const RectF& rect);

		// Returns world rect
		RectF GetWorldRect() const;

		// Sets world rotation angle, in radians
		void SetWorldAngle(float rad);

		// Returns world rotation angle in radians
		float GetWorldAngle() const;

		// Sets world basis
		void SetWorldBasis(const Basis& basis);

		// Returns world basis
		Basis GetWorldBasis() const;

		// Sets world basis without size
		void SetWorldNonSizedBasis(const Basis& basis);

		// Returns world basis without size
		Basis GetWorldNonSizedBasis() const;

		// Sets world axis aligned rectangle transformation
		void SetWorldAxisAlignedRect(const RectF& rect);

		// Returns world axis aligned rectangle transformation
		RectF GetWorldAxisAlignedRect() const;

		// Sets world left top corner position
		void SetWorldLeftTop(const Vec2F& position);

		// Returns world left top corner position
		Vec2F GetWorldLeftTop() const;

		// Sets world right top corner position
		void SetWorldRightTop(const Vec2F& position);

		// Returns world right top corner position
		Vec2F GetWorldRightTop() const;

		// Sets world left down corner position
		void SetWorldLeftBottom(const Vec2F& position);

		// Returns world left down corner position
		Vec2F GetWorldLeftBottom() const;

		// Sets world left right bottom position
		void SetWorldRightBottom(const Vec2F& position);

		// Returns world right bottom corner position
		Vec2F GetWorldRightBottom() const;

		// Sets world center position
		void SetWorldCenter(const Vec2F& position);

		// Returns world center position
		Vec2F GetWorldCenter() const;

		SERIALIZABLE(ActorTransform);

	protected:
		Data* mData; // @SERIALIZABLE

	protected:
		// Sets owner and updates transform
		void SetOwner(Actor* actor);

		// Updates mTransform 
		void UpdateTransform();

		// Check mParentInvertedTransform for actual
		void CheckParentInvTransform();

		// It is called when object was deserialized
		void OnDeserialized(const DataNode& node);

		// Initializes properties
		void InitializeProperties();

		friend class Actor;
	};
}
