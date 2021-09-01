#include "o2/stdafx.h"
#include "CameraActor.h"

#include "o2/Scene/Scene.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Render/Render.h"

namespace o2
{
	CameraActor::CameraActor():
		Actor()
	{}

	CameraActor::CameraActor(const CameraActor& other):
		Actor(other), mType(other.mType), mFixedOrFittedSize(other.mFixedOrFittedSize), mUnits(other.mUnits)
	{}

	CameraActor::~CameraActor()
	{
		o2Scene.OnCameraRemovedScene(this);
	}

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
		o2Render.SetCamera(GetRenderCamera());
	}

	void CameraActor::SetupAndDraw()
	{
		if (fillBackground)
			o2Render.Clear(fillColor);

		listenersLayer.OnBeginDraw();

		Camera prevCamera = o2Render.GetCamera();
		Setup();

		listenersLayer.camera = o2Render.GetCamera();

		for (auto layer : drawLayers.GetLayers())
		{
			for (auto comp : layer->mEnabledDrawables)
				comp->Draw();
		}

		o2Render.SetCamera(prevCamera);

		listenersLayer.OnEndDraw();
	}

	Camera CameraActor::GetRenderCamera() const
	{
		Camera camera;
		switch (mType)
		{
			case Type::Default: camera = Camera::Default(); break;
			case Type::FreeSize: camera = Camera::FixedSize(transform->GetSize()); break;
			case Type::FixedSize: camera = Camera::FixedSize(mFixedOrFittedSize); break;
			case Type::FittedSize: camera = Camera::FittedSize(mFixedOrFittedSize); break;
			case Type::PhysicalCorrect: camera = Camera::PhysicalCorrect(mUnits); break;
			default: camera = Camera::Default(); break;
		}

		transform->size = camera.GetSize();
		transform->Update();
		camera.basis = transform->worldBasis;

		return camera;
	}

	void CameraActor::SetDefault()
	{
		mType = Type::Default;
	}

	void CameraActor::SetFixedSize(const Vec2F& size)
	{
		mType = Type::FixedSize;
		mFixedOrFittedSize = size;
		mUnits = Units::Pixels;
	}

	void CameraActor::SetFittedSize(const Vec2F& size)
	{
		mType = Type::FittedSize;
		mFixedOrFittedSize = size;
		mUnits = Units::Pixels;
	}

	void CameraActor::SetPhysicalCorrect(Units units)
	{
		mType = Type::PhysicalCorrect;
		mUnits = units;
	}

	CameraActor::Type CameraActor::GetCameraType() const
	{
		return mType;
	}

	const Vec2F& CameraActor::GetFittedOrFixedSize() const
	{
		return mFixedOrFittedSize;
	}

	Units CameraActor::GetUnits() const
	{
		return mUnits;
	}

	void CameraActor::OnAddToScene()
	{
		o2Scene.OnCameraAddedOnScene(this);

		Actor::OnAddToScene();
	}

	void CameraActor::OnRemoveFromScene()
	{
		o2Scene.OnCameraRemovedScene(this);

		Actor::OnRemoveFromScene();
	}

}

ENUM_META(o2::CameraActor::Type)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(FittedSize);
	ENUM_ENTRY(FixedSize);
	ENUM_ENTRY(FreeSize);
	ENUM_ENTRY(PhysicalCorrect);
}
END_ENUM_META;

DECLARE_CLASS(o2::CameraActor);
