#pragma once

#include "o2/Utils/Editor/Attributes/PrototypeDeltaSearchAttribute.h"
#include "o2/Utils/Math/Transform.h"
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
	class Actor;
	class ActorTransformData;

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
		PROPERTIES(ActorTransform);
		GETTER(Actor*, actor, GetOwnerActor); // Owner actor getter 

		PROPERTY(Vec2F, position, SetPosition, GetPosition);            // Position property @SCRIPTABLE
		PROPERTY(Vec2F, size, SetSize, GetSize);                        // Size property @SCRIPTABLE
		PROPERTY(float, width, SetWidth, GetWidth);                     // Width property @SCRIPTABLE
		PROPERTY(float, height, SetHeight, GetHeight);                  // Width property @SCRIPTABLE
		PROPERTY(Vec2F, scale, SetScale, GetScale);                     // Scale property @SCRIPTABLE
		PROPERTY(Vec2F, pivot, SetPivot, GetPivot);                     // Pivot property, in local space @SCRIPTABLE
		PROPERTY(Vec2F, szPivot, SetSizePivot, GetSizePivot);           // Pivot in size space property @SCRIPTABLE
		PROPERTY(float, angle, SetAngle, GetAngle);                     // Rotation angle in radians @SCRIPTABLE
		PROPERTY(float, angleDegree, SetAngleDegrees, GetAngleDegrees); // Rotation angle in degrees @SCRIPTABLE
		PROPERTY(float, shear, SetShear, GetShear);                     // Shear property @SCRIPTABLE

		PROPERTY(Basis, basis, SetBasis, GetBasis);                         // Transformation basis property @SCRIPTABLE
		PROPERTY(Basis, nonSizedBasis, SetNonSizedBasis, GetNonSizedBasis); // Non sizes transformation basis property @SCRIPTABLE

		PROPERTY(RectF, AABB, SetAxisAlignedRect, GetAxisAlignedRect); // Axis aligned rectangle @SCRIPTABLE
		PROPERTY(RectF, rect, SetRect, GetRect);                       // Rectangle property. Rectangle - transform without angle, scale and shear.  @SCRIPTABLE
											                           // Sets the position and size @SCRIPTABLE

		PROPERTY(Vec2F, leftTop, SetLeftTop, GetLeftTop);             // Left top corner property @SCRIPTABLE
		PROPERTY(Vec2F, leftBottom, SetLeftBottom, GetLeftBottom);    // Left bottom corner property @SCRIPTABLE
		PROPERTY(Vec2F, rightTop, SetRightTop, GetRightTop);          // Right top corner property @SCRIPTABLE
		PROPERTY(Vec2F, rightBottom, SetRightBottom, GetRightBottom); // Right bottom corner property @SCRIPTABLE
		PROPERTY(Vec2F, center, SetCenter, GetCenter);                // Center position property @SCRIPTABLE
		PROPERTY(Vec2F, rightDir, SetRightDir, GetRightDir);          // X axis direction property @SCRIPTABLE
		PROPERTY(Vec2F, leftDir, SetLeftDir, GetLeftDir);             // Negative X axis direction property @SCRIPTABLE
		PROPERTY(Vec2F, upDir, SetUpDir, GetUpDir);                   // Y axis direction property @SCRIPTABLE
		PROPERTY(Vec2F, downDir, SetDownDir, GetDownDir);             // Negative Y axis direction property @SCRIPTABLE
		PROPERTY(float, right, SetRight, GetRight);                   // Right border position property @SCRIPTABLE
		PROPERTY(float, left, SetLeft, GetLeft);                      // Left border position property @SCRIPTABLE
		PROPERTY(float, top, SetTop, GetTop);                         // Top border position property @SCRIPTABLE
		PROPERTY(float, bottom, SetBottom, GetBottom);                // Bottom border position property @SCRIPTABLE

		PROPERTY(Vec2F, worldPosition, SetWorldPosition, GetWorldPosition);                // World position property @SCRIPTABLE
		PROPERTY(Vec2F, worldPivot, SetWorldPivot, GetWorldPivot);                         // Pivot property, in world space @SCRIPTABLE
		PROPERTY(float, worldAngle, SetWorldAngle, GetWorldAngle);                         // World rotation angle in radians @SCRIPTABLE
		PROPERTY(float, worldAngleDegree, SetWorldAngleDegree, GetWorldAngleDegree);       // World rotation angle in degree @SCRIPTABLE
		PROPERTY(Basis, worldBasis, SetWorldBasis, GetWorldBasis);                         // World transformation basis @SCRIPTABLE
		PROPERTY(Basis, worldNonSizedBasis, SetWorldNonSizedBasis, GetWorldNonSizedBasis); // World transformation basis without size @SCRIPTABLE

		PROPERTY(Vec2F, worldLeftTop, SetWorldLeftTop, GetWorldLeftTop);             // World Left top corner property @SCRIPTABLE
		PROPERTY(Vec2F, worldLeftBottom, SetWorldLeftBottom, GetWorldLeftBottom);    // World Left bottom corner property @SCRIPTABLE
		PROPERTY(Vec2F, worldRightTop, SetWorldRightTop, GetWorldRightTop);          // World Right top corner property @SCRIPTABLE
		PROPERTY(Vec2F, worldRightBottom, SetWorldRightBottom, GetWorldRightBottom); // World Right bottom corner property @SCRIPTABLE
		PROPERTY(Vec2F, worldCenter, SetWorldCenter, GetWorldCenter);                // World center property @SCRIPTABLE
		PROPERTY(Vec2F, worldRightDir, SetWorldRightDir, GetWorldRightDir);          // World X axis direction property @SCRIPTABLE
		PROPERTY(Vec2F, worldLeftDir, SetWorldLeftDir, GetWorldLeftDir);             // World Negative X axis direction property @SCRIPTABLE
		PROPERTY(Vec2F, worldUpDir, SetWorldUpDir, GetWorldUpDir);                   // World Y axis direction property @SCRIPTABLE
		PROPERTY(Vec2F, worldDownDir, SetWorldDownDir, GetWorldDownDir);             // World Negative Y axis direction property @SCRIPTABLE

		PROPERTY(float, worldRight, SetWorldRight, GetWorldRight);    // World Right border position property @SCRIPTABLE
		PROPERTY(float, worldLeft, SetWorldLeft, GetWorldLeft);       // World Left border position property @SCRIPTABLE
		PROPERTY(float, worldTop, SetWorldTop, GetWorldTop);          // World Top border position property @SCRIPTABLE
		PROPERTY(float, worldBottom, SetWorldBottom, GetWorldBottom); // World Bottom border position property @SCRIPTABLE

		PROPERTY(RectF, worldRect, SetWorldRect, GetWorldRect);                       // World rectangle property. Sets the position and size @SCRIPTABLE
		PROPERTY(RectF, worldAABB, SetWorldAxisAlignedRect, GetWorldAxisAlignedRect); // World direction aligned rectangle @SCRIPTABLE

	public:
		ActorTransform(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
					   const Vec2F& scale = Vec2F(1.0f, 1.0f), const Vec2F& pivot = Vec2F(0.5f, 0.5f));

		// Copy-constructor
		ActorTransform(const ActorTransform& other);

		// Destructor
		~ActorTransform();

		// Assign operator
		ActorTransform& operator=(const ActorTransform& other);

		// Assign operator
		//ActorTransform& operator=(const Transform& other);

		// Check EqualSid operator
		bool operator==(const ActorTransform& other) const;

		// Returns owner actor
		Actor* GetOwnerActor() const;

		// Sets transform dirty and needed to update @SCRIPTABLE
		virtual void SetDirty(bool fromParent = false);

		// Returns is transform dirty @SCRIPTABLE
		bool IsDirty() const;

		// Updates transformation
		virtual void Update();

		// Sets position
		virtual void SetPosition(const Vec2F& position);

		// Returns position
		Vec2F GetPosition() const;

		// Sets size
		virtual void SetSize(const Vec2F& size);

		// Return size
		virtual Vec2F GetSize() const;

		// Sets width
		virtual void SetWidth(float value);

		// Return width
		virtual float GetWidth() const;

		// Sets height
		virtual void SetHeight(float value);

		// Return height
		virtual float GetHeight() const;

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
		virtual RectF GetRect() const;

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

	protected:
		ActorTransformData* mData; // Data container. Will be stored in optimized storage @DELTA_SEARCH

	protected:
		// Actor transform constructor with specified data
		ActorTransform(ActorTransformData* data);

		// Copies data parameters from other transform
		virtual void CopyFrom(const ActorTransform& other);

		// Sets owner and updates transform
		virtual void SetOwner(Actor* actor);

		// Returns parent rectange, or zero when no parent
		virtual RectF GetParentRectangle() const;

		// Updates world rectangle and transform relative to parent or origin
		void UpdateWorldRectangleAndTransform();

		// Updates local transformation
		void UpdateTransform();

		// Updates local rectangle
		void UpdateRectangle();

		// Check parentInvertedTransform for actual
		void CheckParentInvTransform();

		// Beginning serialization callback, writes data
		void OnSerialize(DataValue& node) const override;

		// It is called when object was deserialized, reads data
		void OnDeserialized(const DataValue& node) override;

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Returns parent world rect position - left bottom corner
		Vec2F GetParentPosition() const;

		friend class Actor;
		friend class WidgetLayout;
	};

	class ActorTransformData : public ISerializable
	{
	public:
		int dirtyFrame = 1;  // Frame index, when layout was marked as dirty
		int updateFrame = 1; // Frame index, when layout was updated

		Vec2F position;            // Position @SERIALIZABLE @SERIALIZE_IF(IsSerializeEnabled)
		Vec2F size;                // Size @SERIALIZABLE @SERIALIZE_IF(IsSerializeEnabled)
		Vec2F scale = Vec2F(1, 1); // Scale, (1, 1) is default @SERIALIZABLE @SERIALIZE_IF(IsSerializeEnabled)
		Vec2F pivot;               // Pivot: (0, 0) is left bottom corner - (1, 1) is right top corner @SERIALIZABLE @SERIALIZE_IF(IsSerializeEnabled)
		float angle = 0.0f;        // Rotation angle in radians @SERIALIZABLE @SERIALIZE_IF(IsSerializeEnabled)
		float shear = 0.0f;        // Shear @SERIALIZABLE @SERIALIZE_IF(IsSerializeEnabled)

		RectF rectangle;              // The rectangle in local space
		RectF parentRectangle;        // The parent rectangle
		Vec2F parentRectangePosition; // The parent rectangle pivot position
		RectF worldRectangle;         // The rectangle in world space

		Basis transform;         // Final transform basis
		Basis nonSizedTransform; // Final transform basis without size

		Basis worldNonSizedTransform; // World transform without size
		Basis worldTransform;         // Result world basis

		Basis parentInvertedTransform;       // Parent world transform inverted
		Basis parentTransform;               // Parent world transform
		int   parentInvTransformActualFrame; // last mParentInvertedTransform actual frame index

		Actor* owner = nullptr; // Owner actor 

		SERIALIZABLE(ActorTransformData);

		// Returns is serialize enabled; used to turn off fields serialization
		virtual bool IsSerializeEnabled() const;
	};
}

CLASS_BASES_META(o2::ActorTransform)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ActorTransform)
{
	FIELD().PUBLIC().NAME(actor);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(position);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(size);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(width);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(height);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(scale);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(pivot);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(szPivot);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(angle);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(angleDegree);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(shear);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(basis);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(nonSizedBasis);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(AABB);
	FIELD().PUBLIC().NAME(rect);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(leftTop);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(leftBottom);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(rightTop);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(rightBottom);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(center);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(rightDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(leftDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(upDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(downDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(right);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(left);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(top);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(bottom);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldPosition);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldPivot);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldAngle);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldAngleDegree);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldBasis);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldNonSizedBasis);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldLeftTop);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldLeftBottom);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldRightTop);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldRightBottom);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldCenter);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldRightDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldLeftDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldUpDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldDownDir);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldRight);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldLeft);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldTop);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldBottom);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldRect);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(worldAABB);
	FIELD().PROTECTED().DELTA_SEARCH_ATTRIBUTE().NAME(mData);
}
END_META;
CLASS_METHODS_META(o2::ActorTransform)
{

	FUNCTION().PUBLIC().CONSTRUCTOR(const Vec2F&, const Vec2F&, float, const Vec2F&, const Vec2F&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const ActorTransform&);
	FUNCTION().PUBLIC().SIGNATURE(Actor*, GetOwnerActor);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetDirty, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsDirty);
	FUNCTION().PUBLIC().SIGNATURE(void, Update);
	FUNCTION().PUBLIC().SIGNATURE(void, SetPosition, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetPosition);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSize, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetSize);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWidth, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWidth);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHeight, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetHeight);
	FUNCTION().PUBLIC().SIGNATURE(void, SetPivot, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetPivot);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSizePivot, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetSizePivot);
	FUNCTION().PUBLIC().SIGNATURE(void, SetScale, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetScale);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAngle, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetAngle);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAngleDegrees, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetAngleDegrees);
	FUNCTION().PUBLIC().SIGNATURE(void, SetShear, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetShear);
	FUNCTION().PUBLIC().SIGNATURE(void, SetBasis, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetBasis);
	FUNCTION().PUBLIC().SIGNATURE(void, SetNonSizedBasis, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetNonSizedBasis);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRect, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetRect);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAxisAlignedRect, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetAxisAlignedRect);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLeftTop, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetLeftTop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRightTop, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetRightTop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLeftBottom, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetLeftBottom);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRightBottom, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetRightBottom);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCenter, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetCenter);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRightDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetRightDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLeftDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetLeftDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetUpDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetUpDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDownDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetDownDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRight, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetRight);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLeft, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetLeft);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTop, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetTop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetBottom, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetBottom);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, World2LocalPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, Local2WorldPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, World2LocalDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, Local2WorldDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsPointInside, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldPosition, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldPosition);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldPivot, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldPivot);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldAngle, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWorldAngle);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldAngleDegree, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWorldAngleDegree);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldBasis, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetWorldBasis);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldNonSizedBasis, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetWorldNonSizedBasis);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldRect, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetWorldRect);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldAxisAlignedRect, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetWorldAxisAlignedRect);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldLeftTop, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldLeftTop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldRightTop, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldRightTop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldLeftBottom, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldLeftBottom);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldRightBottom, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldRightBottom);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldCenter, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldCenter);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldRightDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldRightDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldLeftDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldLeftDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldUpDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldUpDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldDownDir, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetWorldDownDir);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldRight, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWorldRight);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldLeft, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWorldLeft);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldTop, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWorldTop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWorldBottom, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWorldBottom);
	FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransformData*);
	FUNCTION().PROTECTED().SIGNATURE(void, CopyFrom, const ActorTransform&);
	FUNCTION().PROTECTED().SIGNATURE(void, SetOwner, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(RectF, GetParentRectangle);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateWorldRectangleAndTransform);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateTransform);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateRectangle);
	FUNCTION().PROTECTED().SIGNATURE(void, CheckParentInvTransform);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(Vec2F, GetParentPosition);
}
END_META;

CLASS_BASES_META(o2::ActorTransformData)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ActorTransformData)
{
	FIELD().PUBLIC().DEFAULT_VALUE(1).NAME(dirtyFrame);
	FIELD().PUBLIC().DEFAULT_VALUE(1).NAME(updateFrame);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().SERIALIZE_IF_ATTRIBUTE(IsSerializeEnabled).NAME(position);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().SERIALIZE_IF_ATTRIBUTE(IsSerializeEnabled).NAME(size);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().SERIALIZE_IF_ATTRIBUTE(IsSerializeEnabled).DEFAULT_VALUE(Vec2F(1, 1)).NAME(scale);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().SERIALIZE_IF_ATTRIBUTE(IsSerializeEnabled).NAME(pivot);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().SERIALIZE_IF_ATTRIBUTE(IsSerializeEnabled).DEFAULT_VALUE(0.0f).NAME(angle);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().SERIALIZE_IF_ATTRIBUTE(IsSerializeEnabled).DEFAULT_VALUE(0.0f).NAME(shear);
	FIELD().PUBLIC().NAME(rectangle);
	FIELD().PUBLIC().NAME(parentRectangle);
	FIELD().PUBLIC().NAME(parentRectangePosition);
	FIELD().PUBLIC().NAME(worldRectangle);
	FIELD().PUBLIC().NAME(transform);
	FIELD().PUBLIC().NAME(nonSizedTransform);
	FIELD().PUBLIC().NAME(worldNonSizedTransform);
	FIELD().PUBLIC().NAME(worldTransform);
	FIELD().PUBLIC().NAME(parentInvertedTransform);
	FIELD().PUBLIC().NAME(parentTransform);
	FIELD().PUBLIC().NAME(parentInvTransformActualFrame);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(owner);
}
END_META;
CLASS_METHODS_META(o2::ActorTransformData)
{

	FUNCTION().PUBLIC().SIGNATURE(bool, IsSerializeEnabled);
}
END_META;
