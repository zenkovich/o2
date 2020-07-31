#pragma once
#include "o2/Scene/Actor.h"

namespace o2
{
	// -------------------------------------------------------------------------------
	// Camera actor. Can works with different types, renders layers or itself children
	// -------------------------------------------------------------------------------
	class CameraActor: public Actor
	{
	public:
		enum class Type { Default, FixedSize, FittedSize, PhysicalCorrect };

	public:
		// Default constructor
		CameraActor();

		// Copy constructor
		CameraActor(const CameraActor& other);

		// Copy operator
		CameraActor& operator=(const CameraActor& other);

		// Sets camera to render
		void Setup();

		// Returns rendering camera
		Camera GetRenderCamera() const;

		// Sets default camera
		void SetDefault();

		// Sets camera with fixed size
		void SetFixedSize(const Vec2F& size);

		// Sets camera with fixed aspect
		void SetFittedSize(const Vec2F& size);

		// Sets camera with physical correct units
		void SetPhysicalCorrect(Units units);

		SERIALIZABLE(CameraActor);

	private:
		Type  mType = Type::Default;       // Type of camera @SERIALIZABLE
		Vec2F mFixedOrFittedSize;          // Fitted or fixed types size @SERIALIZABLE
		Units mUnits = Units::Centimeters; // Physical camera units @SERIALIZABLE
	};
}

PRE_ENUM_META(o2::CameraActor::Type);

CLASS_BASES_META(o2::CameraActor)
{
	BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(o2::CameraActor)
{
	PRIVATE_FIELD(mType).DEFAULT_VALUE(Type::Default).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mFixedOrFittedSize).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mUnits).DEFAULT_VALUE(Units::Centimeters).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::CameraActor)
{

	PUBLIC_FUNCTION(void, Setup);
	PUBLIC_FUNCTION(Camera, GetRenderCamera);
	PUBLIC_FUNCTION(void, SetDefault);
	PUBLIC_FUNCTION(void, SetFixedSize, const Vec2F&);
	PUBLIC_FUNCTION(void, SetFittedSize, const Vec2F&);
	PUBLIC_FUNCTION(void, SetPhysicalCorrect, Units);
}
END_META;
