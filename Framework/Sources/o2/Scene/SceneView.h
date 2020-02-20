#pragma once
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Math/Basis.h"

namespace o2
{
	// ----------------------------------------------------------------------------------
	// Basic scene view interface. Returns transformation basis from screen to scene view
	// ----------------------------------------------------------------------------------
	class ISceneView: public ISerializable
	{
	public:
		// Returns transformation basis
		virtual Basis GetTransform(const Vec2F& screenViewSize) const;

		SERIALIZABLE(ISceneView);
	};

	// -------------------------------------------------------------------
	// Screen resolution scene view. Sets resolution 1 to 1 as screen size
	// -------------------------------------------------------------------
	class ScreenResolutionSceneView: public ISceneView
	{
	public:
		// Returns transformation basis
		Basis GetTransform(const Vec2F& screenViewSize) const override;

		SERIALIZABLE(ScreenResolutionSceneView);
	};

	// --------------------------------------------------
	// Fixed resolution scene view. Sets scene resolution
	// --------------------------------------------------
	class FixedResolutionSceneView: public ISceneView
	{
	public:
		Vec2I fixedResolution; // Fixed view resolution // @SERIALIZABLE

		// Returns transformation basis
		Basis GetTransform(const Vec2F& screenViewSize) const override;

		SERIALIZABLE(FixedResolutionSceneView);
	};

	// -------------------------
	// Fit resolution scene view
	// -------------------------
	class FitResolutionSceneView: public ISceneView
	{
	public:
		Vec2I fitResolution; // Fixed view resolution // @SERIALIZABLE

		// Returns transformation basis
		Basis GetTransform(const Vec2F& screenViewSize) const override;

		SERIALIZABLE(FitResolutionSceneView);
	};
}

CLASS_BASES_META(o2::ISceneView)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ISceneView)
{
}
END_META;
CLASS_METHODS_META(o2::ISceneView)
{

	PUBLIC_FUNCTION(Basis, GetTransform, const Vec2F&);
}
END_META;

CLASS_BASES_META(o2::ScreenResolutionSceneView)
{
	BASE_CLASS(o2::ISceneView);
}
END_META;
CLASS_FIELDS_META(o2::ScreenResolutionSceneView)
{
}
END_META;
CLASS_METHODS_META(o2::ScreenResolutionSceneView)
{

	PUBLIC_FUNCTION(Basis, GetTransform, const Vec2F&);
}
END_META;

CLASS_BASES_META(o2::FixedResolutionSceneView)
{
	BASE_CLASS(o2::ISceneView);
}
END_META;
CLASS_FIELDS_META(o2::FixedResolutionSceneView)
{
	PUBLIC_FIELD(fixedResolution).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::FixedResolutionSceneView)
{

	PUBLIC_FUNCTION(Basis, GetTransform, const Vec2F&);
}
END_META;

CLASS_BASES_META(o2::FitResolutionSceneView)
{
	BASE_CLASS(o2::ISceneView);
}
END_META;
CLASS_FIELDS_META(o2::FitResolutionSceneView)
{
	PUBLIC_FIELD(fitResolution).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::FitResolutionSceneView)
{

	PUBLIC_FUNCTION(Basis, GetTransform, const Vec2F&);
}
END_META;
