#pragma once
#include "o2/Scene/Physics/ICollider.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"

namespace o2
{
	// -----------------------
	// Circle physics collider
	// -----------------------
	class CircleCollider: public ICollider
	{
	public:
		PROPERTIES(CircleCollider);
		PROPERTY(float, radius, SetRadius, GetRadius);           // Collider radius property
		PROPERTY(bool, fitByActor, SetFitByActor, IsFitByActor); // Fit by actor's size property

	public:
		// Default constructor
		CircleCollider();

		// Copy-constructor
		CircleCollider(const CircleCollider& other);

		// Copy operator
		CircleCollider& operator=(const CircleCollider& other);

		// Sets collider radius
		void SetRadius(float radius);

		// Returns collider radius
		float GetRadius() const;

		// Sets collider fit to actor's size
		void SetFitByActor(bool fit);

		// Is collider fits to actor's size
		bool IsFitByActor() const;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		// Is component visible in create menu
		static bool IsAvailableFromCreateMenu();

#if IS_EDITOR
		// It is called when component added from editor; fits collider by actor size
		void OnAddedFromEditor() override;
#endif

		SERIALIZABLE(CircleCollider);

	private:
		float mRadius = 1.0f; // Size of collider @SERIALIZABLE

		bool mFitByActor = true; // Is box size is fitting by actor's size @SERIALIZABLE

		b2CircleShape mShape; // Physics shape

	private:
		// Returns shape with relative position and angle
		b2Shape* GetShape(const Basis& transform) override;

		// It is called when transformation was changed; fits size by actor size when required
		void OnTransformChanged() override;
	};
}

CLASS_BASES_META(o2::CircleCollider)
{
	BASE_CLASS(o2::ICollider);
}
END_META;
CLASS_FIELDS_META(o2::CircleCollider)
{
	PUBLIC_FIELD(radius);
	PUBLIC_FIELD(fitByActor);
	PRIVATE_FIELD(mRadius).DEFAULT_VALUE(1.0f).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mFitByActor).DEFAULT_VALUE(true).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mShape);
}
END_META;
CLASS_METHODS_META(o2::CircleCollider)
{

	PUBLIC_FUNCTION(void, SetRadius, float);
	PUBLIC_FUNCTION(float, GetRadius);
	PUBLIC_FUNCTION(void, SetFitByActor, bool);
	PUBLIC_FUNCTION(bool, IsFitByActor);
	PUBLIC_STATIC_FUNCTION(String, GetName);
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableFromCreateMenu);
	PUBLIC_FUNCTION(void, OnAddedFromEditor);
	PRIVATE_FUNCTION(b2Shape*, GetShape, const Basis&);
	PRIVATE_FUNCTION(void, OnTransformChanged);
}
END_META;
