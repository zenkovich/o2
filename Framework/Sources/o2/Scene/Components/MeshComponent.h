#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Mesh.h"
#include "o2/Render/TextureRef.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Utils/Types/UID.h"

namespace o2
{
	// ---------------
	// Image component
	// ---------------
	class MeshComponent: public DrawableComponent
	{
	public:
		PROPERTIES(MeshComponent);
		PROPERTY(ImageAssetRef, image, SetImage, GetImage);                   // Image property
		PROPERTY(RectF, mappingFrame, SetMappingFrame, GetMappingFrame);      // Image mapping frame
		PROPERTY(Color4, color, SetColor, GetColor);                          // Color property
		PROPERTY(Vector<Vec2F>, extraPoints, SetExtraPoints, GetExtraPoints); // extra points property

		Spline spline; // Shell spline @SERIALIZABLE

	public:
		// Default constructor
		MeshComponent();

		// Copy-constructor
		MeshComponent(const MeshComponent& other);

		// Destructor
		~MeshComponent();

		// Assign operator
		MeshComponent& operator=(const MeshComponent& other);

		// Draws sprite 
		void Draw() override;

		// Returns true if point is under drawable
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns mesh
		const Mesh& GetMesh() const;

		// Sets extra points
		void SetExtraPoints(const Vector<Vec2F>& points);

		// Returns extra points list
		const Vector<Vec2F>& GetExtraPoints() const;

		// Sets extra point position
		void SetExtraPoint(int idx, const Vec2F& pos);

		// Adds one extra point
		void AddExtraPoint(const Vec2F& point);

		// Removes extra point by index
		void RemoveExtraPoint(int idx);

		// Sets image
		void SetImage(const ImageAssetRef& image);

		// Returns image
		const ImageAssetRef& GetImage() const;

		// Sets image mapping frame
		void SetMappingFrame(const RectF& frame);

		// Returns image mapping frame
		const RectF& GetMappingFrame() const;

		// Sets color
		void SetColor(const Color4& color);

		// Returns color
		const Color4& GetColor() const;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		// Returns name of component icon
		static String GetIcon();

		SERIALIZABLE(MeshComponent);

	protected:
		Mesh   mMesh;      // Drawing mesh, built from spline
		Basis  mTransform; // Transform where mesh was built

		ImageAssetRef mImageAsset;                         // Image asset @SERIALIZABLE
		RectF         mImageMapping = RectF(0, 0, 10, 10); // Image mapping rectangle @SERIALIZABLE

		Vector<Vec2F> mExtraPoints; // Extra topology points @SERIALIZABLE

		Color4 mColor = Color4::White(); // Mesh color @SERIALIZABLE

		bool mNeedUpdateMesh = false; // True, when mesh data is dirty and need to rebuild

	protected:
		// It is called when actor's transform was changed
		void OnTransformUpdated() override;

		// Calculates mesh from spline
		void UpdateMesh();

		// Sets owner actor
		void SetOwnerActor(Actor* actor) override;

		// Calling when deserializing
		void OnDeserialized(const DataValue& node) override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;
	};
}

CLASS_BASES_META(o2::MeshComponent)
{
	BASE_CLASS(o2::DrawableComponent);
}
END_META;
CLASS_FIELDS_META(o2::MeshComponent)
{
	FIELD().NAME(image).PUBLIC();
	FIELD().NAME(mappingFrame).PUBLIC();
	FIELD().NAME(color).PUBLIC();
	FIELD().NAME(extraPoints).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(spline).PUBLIC();
	FIELD().NAME(mMesh).PROTECTED();
	FIELD().NAME(mTransform).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mImageAsset).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(RectF(0, 0, 10, 10)).NAME(mImageMapping).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mExtraPoints).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Color4::White()).NAME(mColor).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mNeedUpdateMesh).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::MeshComponent)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(const Mesh&, GetMesh);
	PUBLIC_FUNCTION(void, SetExtraPoints, const Vector<Vec2F>&);
	PUBLIC_FUNCTION(const Vector<Vec2F>&, GetExtraPoints);
	PUBLIC_FUNCTION(void, SetExtraPoint, int, const Vec2F&);
	PUBLIC_FUNCTION(void, AddExtraPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, RemoveExtraPoint, int);
	PUBLIC_FUNCTION(void, SetImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(const ImageAssetRef&, GetImage);
	PUBLIC_FUNCTION(void, SetMappingFrame, const RectF&);
	PUBLIC_FUNCTION(const RectF&, GetMappingFrame);
	PUBLIC_FUNCTION(void, SetColor, const Color4&);
	PUBLIC_FUNCTION(const Color4&, GetColor);
	PUBLIC_STATIC_FUNCTION(String, GetName);
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PUBLIC_STATIC_FUNCTION(String, GetIcon);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, UpdateMesh);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnDeserializedDelta, const DataValue&, const IObject&);
}
END_META;
