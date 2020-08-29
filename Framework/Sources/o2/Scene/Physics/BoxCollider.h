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
		PROPERTY(Vec2F, size, SetSize, GetSize); // COllider size property

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

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		SERIALIZABLE(BoxCollider);

	private:
		Vec2F mSize = Vec2F(1, 1); // Size of collider @SERIALIZABLE

		b2PolygonShape mShape; // Physics shape

	private:
		// Returns shape with relative position and angle
		b2Shape* GetShape(const Basis& transform) override;
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
	PRIVATE_FIELD(mSize).DEFAULT_VALUE(Vec2F(1, 1)).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mShape);
}
END_META;
CLASS_METHODS_META(o2::BoxCollider)
{

	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_STATIC_FUNCTION(String, GetName);
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PRIVATE_FUNCTION(b2Shape*, GetShape, const Basis&);
}
END_META;
