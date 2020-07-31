#include "o2/stdafx.h"
#include "CameraActor.h"

namespace o2
{
	CameraActor::CameraActor():
		Actor()
	{}

	CameraActor::CameraActor(const CameraActor& other):
		Actor(other), mType(other.mType), mFixedOrFittedSize(other.mFixedOrFittedSize), mUnits(other.mUnits)
	{}

	CameraActor& CameraActor::operator=(const CameraActor& other)
	{
		Actor::operator=(other);

		mType = other.mType;
		mFixedOrFittedSize = other.mFixedOrFittedSize;
		mUnits = other.mUnits;

		return *this;
	}

	void CameraActor::Setup()
	{
		Camera camera = GetRenderCamera();
		transform->size = camera.GetSize();
		o2Render.SetCamera(camera);
	}

	Camera CameraActor::GetRenderCamera() const
	{
		switch (mType)
		{
			case Type::Default: return Camera::Default();
			case Type::FixedSize: return Camera::FixedSize(mFixedOrFittedSize);
			case Type::FittedSize: return Camera::FittedSize(mFixedOrFittedSize);
			case Type::PhysicalCorrect: return Camera::PhysicalCorrect(mUnits);
			default: return Camera::Default();
		}
	}

	void CameraActor::SetDefault()
	{
		mType = Type::Default;
	}

	void CameraActor::SetFixedSize(const Vec2F& size)
	{
		mType = Type::FixedSize;
		mFixedOrFittedSize = size;
	}

	void CameraActor::SetFittedSize(const Vec2F& size)
	{
		mType = Type::FittedSize;
		mFixedOrFittedSize = size;
	}

	void CameraActor::SetPhysicalCorrect(Units units)
	{
		mType = Type::PhysicalCorrect;
		mUnits = units;
	}
}

ENUM_META(o2::CameraActor::Type)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(FittedSize);
	ENUM_ENTRY(FixedSize);
	ENUM_ENTRY(PhysicalCorrect);
}
END_ENUM_META;

DECLARE_CLASS(o2::CameraActor);
