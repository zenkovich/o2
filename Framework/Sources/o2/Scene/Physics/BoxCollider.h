#pragma once
#include "o2/Scene/Physics/ICollider.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"

namespace o2
{
	// --------------------
	// Box physics collider
	// --------------------
	class BoxCollider: public ICollider
	{
	public:
		PROPERTIES(BoxCollider);
		PROPERTY(Vec2F, size, SetSize, GetSize);                 // Collider size property
		PROPERTY(bool, fitByActor, SetFitByActor, IsFitByActor); // Fit by actor's size property

	public:
		// Default constructor
		BoxCollider();

		// Copy-constructor
		BoxCollider(const BoxCollider& other);

		// Copy operator
		BoxCollider& operator=(const BoxCollider& other);

		// Sets collider size
		void SetSize(const Vec2F& size);

		// Returns collider size
		Vec2F GetSize() const;

		// Sets collider fit to actor's size
		void SetFitByActor(bool fit);

		// Is collider fits to actor's size
		bool IsFitByActor() const;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

#if IS_EDITOR
		// It is called when component added from editor; fits collider by actor size
		void OnAddedFromEditor() override;
#endif

		SERIALIZABLE(BoxCollider);

	private:
		Vec2F mSize = Vec2F(1, 1); // Size of collider @SERIALIZABLE

		bool mFitByActor = true; // Is box size is fitting by actor's size @SERIALIZABLE

		b2PolygonShape mShape; // Physics shape

	private:
		// Returns shape with relative position and angle
		b2Shape* GetShape(const Basis& transform) override;

		// It is called when transformation was changed; fits size by actor size when required
		void OnTransformChanged() override;
	};
}

CLASS_BASES_META(o2::BoxCollider)
{
	BASE_CLASS(o2::ICollider);
}
END_META;
CLASS_FIELDS_META(o2::BoxCollider)
{
	PUBLIC_FIELD(size);
	PUBLIC_FIELD(fitByActor);
	PRIVATE_FIELD(mSize).DEFAULT_VALUE(Vec2F(1, 1)).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mFitByActor).DEFAULT_VALUE(true).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mShape);
}
END_META;
CLASS_METHODS_META(o2::BoxCollider)
{

	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(void, SetFitByActor, bool);
	PUBLIC_FUNCTION(bool, IsFitByActor);
	PUBLIC_STATIC_FUNCTION(String, GetName);
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PUBLIC_FUNCTION(void, OnAddedFromEditor);
	PRIVATE_FUNCTION(b2Shape*, GetShape, const Basis&);
	PRIVATE_FUNCTION(void, OnTransformChanged);
}
END_META;
