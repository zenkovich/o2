#pragma once

#include "Utils/Math/Transform.h"

namespace o2
{
	class Actor;

	// -------------------------------------------------------------------------------------------
	// Actor transform. Represents the position of actor relative to his parent (the local space),
	// and calculates the position relative to world (the world space).
	// Position shows the pivot point, and pivot shows where is the rectangle of actor.
	// Also this rectangle has angle, scale, size and shear.
	//
	// Note: the rectangle of transform here means the rectangle without angle, scale and shear.
	// So, the final transform is rotated, scaled and sheared rectangle.
	// -------------------------------------------------------------------------------------------
	class ActorTransform: public ISerializable
	{
	public:
		Getter<Actor*>   actor;              // Owner actor getter

		Property<Vec2F>  position;           // Position property
		Property<Vec2F>  size;               // Size property
		Property<float>  width;              // Width property
		Property<float>  height;             // Width property
		Property<Vec2F>  scale;              // Scale property
		Property<Vec2F>  pivot;              // Pivot property, in local space
		Property<Vec2F>  szPivot;            // Pivot in size space property
		Property<float>  angle;              // Rotation angle in radians
		Property<float>  angleDegree;        // Rotation angle in degrees
		Property<float>  shear;              // Shear property

		Property<Basis>  basis;              // Transformation basis property
		Property<Basis>  nonSizedBasis;      // Non sizes transformation basis property

		Property<RectF>  AABB;               // Axis aligned rectangle
		Property<RectF>  rect;               // Rectangle property. Rectangle - transform without angle, scale and shear. 
		                                     // Sets the position and size

		Property<Vec2F>  leftTop;            // Left top corner property
		Property<Vec2F>  leftBottom;         // Left bottom corner property
		Property<Vec2F>  rightTop;           // Right top corner property
		Property<Vec2F>  rightBottom;        // Right bottom corner property
		Property<Vec2F>  center;             // Center position property
		Property<Vec2F>  rightDir;           // X axis direction property
		Property<Vec2F>  leftDir;            // Negative X axis direction property
		Property<Vec2F>  upDir;              // Y axis direction property
		Property<Vec2F>  downDir;            // Negative Y axis direction property
		Property<float>  right;              // Right border position property
		Property<float>  left;               // Left border position property
		Property<float>  top;                // Top border position property
		Property<float>  bottom;             // Bottom border position property

		Property<Vec2F>  worldPosition;      // World position property
		Property<Vec2F>  worldPivot;         // Pivot property, in world space
		Property<float>  worldAngle;         // World rotation angle in radians
		Property<float>  worldAngleDegree;   // World rotation angle in degree
		Property<Basis>  worldBasis;         // World transformation basis
		Property<Basis>  worldNonSizedBasis; // World transformation basis without size

		Property<Vec2F>  worldLeftTop;       // World Left top corner property
		Property<Vec2F>  worldLeftBottom;    // World Left bottom corner property
		Property<Vec2F>  worldRightTop;      // World Right top corner property
		Property<Vec2F>  worldRightBottom;   // World Right bottom corner property
		Property<Vec2F>  worldCenter;        // World center property
		Property<Vec2F>  worldRightDir;      // World X axis direction property
		Property<Vec2F>  worldLeftDir;       // World Negative X axis direction property
		Property<Vec2F>  worldUpDir;         // World Y axis direction property
		Property<Vec2F>  worldDownDir;       // World Negative Y axis direction property

		Property<float>  worldRight;         // World Right border position property
		Property<float>  worldLeft;          // World Left border position property
		Property<float>  worldTop;           // World Top border position property
		Property<float>  worldBottom;        // World Bottom border position property

		Property<RectF>  worldRect;          // World rectangle property. Sets the position and size
		Property<RectF>  worldAABB;          // World direction aligned rectangle

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

		// Returns owner actor
		Actor* GetOwnerActor() const;

		// Sets position
		virtual void SetPosition(const Vec2F& position);

		// Returns position
		Vec2F GetPosition() const;

		// Sets size
		virtual void SetSize(const Vec2F& size);

		// Return size
		Vec2F GetSize() const;

		// Sets width
		void SetWidth(float value);

		// Return width
		float GetWidht() const;

		// Sets height
		void SetHeight(float value);

		// Return height
		float GetHeight() const;

		// Sets pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		virtual void SetPivot(const Vec2F& pivot);

		// Return pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		Vec2F GetPivot() const;

		// Sets size pivot, in local space, where (0, 0) - left down corner, (size.x, size.y) - right top
		void SetSizePivot(const Vec2F& relPivot);

		// Returns size pivot, in local space, where (0, 0) - left down corner, (size.x, size.y) - right top
		Vec2F GetSizePivot() const;

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
		virtual void SetBasis(const Basis& basis);

		// Returns basis
		Basis GetBasis() const;

		// Sets basis without size
		virtual void SetNonSizedBasis(const Basis& basis);

		// Returns basis without size
		Basis GetNonSizedBasis() const;

		// Sets rect
		virtual void SetRect(const RectF& rect);

		// Returns rect
		RectF GetRect() const;

		// Sets direction aligned rectangle transformation
		virtual void SetAxisAlignedRect(const RectF& rect);

		// Returns direction aligned rectangle transformation
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

		// Set local right direction
		void SetRightDir(const Vec2F& dir);

		// Returns local right direction
		Vec2F GetRightDir() const;

		// Set local left direction
		void SetLeftDir(const Vec2F& dir);

		// Returns local left direction
		Vec2F GetLeftDir() const;

		// Set local up direction
		void SetUpDir(const Vec2F& dir);

		// Returns local up direction
		Vec2F GetUpDir() const;

		// Set local down direction
		void SetDownDir(const Vec2F& dir);

		// Returns local down direction
		Vec2F GetDownDir() const;

		// Set local right border position
		void SetRight(float value);

		// Returns local right border position
		float GetRight() const;

		// Set local left border position
		void SetLeft(float value);

		// Returns local left border position
		float GetLeft() const;

		// Set local top border position
		void SetTop(float value);

		// Returns local top border position
		float GetTop() const;

		// Set local down border position
		void SetBottom(float value);

		// Returns local down border position
		float GetBottom() const;

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

		// Sets world position
		void SetWorldPosition(const Vec2F& position);

		// Returns world position
		Vec2F GetWorldPosition() const;

		// Sets pivot by world coordinates
		void SetWorldPivot(const Vec2F& pivot);

		// Returns pivot position in world coordinates
		Vec2F GetWorldPivot() const;

		// Sets world rotation angle, in radians
		void SetWorldAngle(float rad);

		// Returns world rotation angle in radians
		float GetWorldAngle() const;

		// Sets world rotation angle, in degrees
		void SetWorldAngleDegree(float deg);

		// Returns world rotation angle in degrees
		float GetWorldAngleDegree() const;

		// Sets world basis
		void SetWorldBasis(const Basis& basis);

		// Returns world basis
		Basis GetWorldBasis() const;

		// Sets world basis without size
		void SetWorldNonSizedBasis(const Basis& basis);

		// Returns world basis without size
		Basis GetWorldNonSizedBasis() const;

		// Sets world rect
		void SetWorldRect(const RectF& rect);

		// Returns world rect
		RectF GetWorldRect() const;

		// Sets world direction aligned rectangle transformation
		void SetWorldAxisAlignedRect(const RectF& rect);

		// Returns world direction aligned rectangle transformation
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

		// Sets World center position
		void SetWorldCenter(const Vec2F& position);

		// Returns World center position
		Vec2F GetWorldCenter() const;

		// Set World right direction
		void SetWorldRightDir(const Vec2F& dir);

		// Returns World right direction
		Vec2F GetWorldRightDir() const;

		// Set World left direction
		void SetWorldLeftDir(const Vec2F& dir);

		// Returns World left direction
		Vec2F GetWorldLeftDir() const;

		// Set World up direction
		void SetWorldUpDir(const Vec2F& dir);

		// Returns World up direction
		Vec2F GetWorldUpDir() const;

		// Set World down direction
		void SetWorldDownDir(const Vec2F& dir);

		// Returns World down direction
		Vec2F GetWorldDownDir() const;

		// Set World right border position
		void SetWorldRight(float value);

		// Returns World right border position
		float GetWorldRight() const;

		// Set World left border position
		void SetWorldLeft(float value);

		// Returns World left border position
		float GetWorldLeft() const;

		// Set World top border position
		void SetWorldTop(float value);

		// Returns World top border position
		float GetWorldTop() const;

		// Set World down border position
		void SetWorldBottom(float value);

		// Returns World down border position
		float GetWorldBottom() const;

		SERIALIZABLE(ActorTransform);

	public:
		class Data: public ISerializable
		{
		public:
			Actor* owner = nullptr;                    // Owner actor

			Vec2F  position;                           // Position @SERIALIZABLE
			Vec2F  size;                               // Size @SERIALIZABLE
			Vec2F  scale;                              // Scale, (1; 1) is default @SERIALIZABLE
			Vec2F  pivot;                              // Pivot: (0; 0) is left bottom corner - (1; 1) is right top corner @SERIALIZABLE
			float  angle = 0;                          // Rotation angle in radians @SERIALIZABLE
			float  shear = 0;                          // Shear @SERIALIZABLE

			RectF  rectangle;                          // The rectangle in local space

			Basis  transform;                          // Final transform basis
			Basis  nonSizedTransform;                  // Final transform basis without size

			Basis  worldNonSizedTransform;             // World transform without size
			Basis  worldTransform;                     // Result world basis

			Basis  parentInvertedTransform;            // Parent world transform inverted
			Basis  parentTransform;                    // Parent world transform
			bool   isParentInvTransformActual = false; // Is mParentInvertedTransform is actual

			SERIALIZABLE(Data);
		};

	protected:
		Data* mData; // @SERIALIZABLE

	protected:
		// Actor transform constructor with specified data
		ActorTransform(Data* data);

		// Sets owner and updates transform
		virtual void SetOwner(Actor* actor);

		// It is called when transform is changing 
		void OnChanged();

		// Check parentInvertedTransform for actual
		void CheckParentInvTransform();

		// It is called when object was deserialized
		void OnDeserialized(const DataNode& node);

		// Initializes properties
		void InitializeProperties();

		friend class Actor;
	};
}
