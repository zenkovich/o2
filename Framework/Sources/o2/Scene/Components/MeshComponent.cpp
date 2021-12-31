#include "o2/stdafx.h"
#include "MeshComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "3rdPartyLibs/CDT/include/CDT.h"
#include "o2/Application/Input.h"

namespace o2
{
	MeshComponent::MeshComponent():
		DrawableComponent()
	{
		spline.onKeysChanged = THIS_FUNC(UpdateMesh);
	}

	MeshComponent::MeshComponent(const MeshComponent& other):
		DrawableComponent(other), mMesh(other.mMesh), spline(other.spline)
	{
		spline.onKeysChanged = THIS_FUNC(UpdateMesh);
	}

	MeshComponent::~MeshComponent()
	{}

	MeshComponent& MeshComponent::operator=(const MeshComponent& other)
	{
		DrawableComponent::operator=(other);
		spline = other.spline;
		mMesh = other.mMesh;
		return *this;
	}

	void MeshComponent::Draw()
	{
		mMesh.Draw();
		DrawableComponent::OnDrawn();

		if (o2Input.IsKeyDown(VK_F3))
			o2Render.DrawMeshWire(&mMesh, Color4(0, 0, 0, 100));
	}

	bool MeshComponent::IsUnderPoint(const Vec2F& point)
	{
		return false;
	}

	void MeshComponent::SetImage(const ImageAssetRef& image)
	{
		mImageAsset = image;
		UpdateMesh();
	}

	const ImageAssetRef& MeshComponent::GetImage() const
	{
		return mImageAsset;
	}

	void MeshComponent::SetMappingFrame(const RectF& frame)
	{
		mImageMapping = frame;
		UpdateMesh();
	}

	const RectF& MeshComponent::GetMappingFrame() const
	{
		return mImageMapping;
	}

	void MeshComponent::SetColor(const Color4& color)
	{
		mColor = color;
		UpdateMesh();
	}

	const Color4& MeshComponent::GetColor() const
	{
		return mColor;
	}

	String MeshComponent::GetName()
	{
		return "Mesh";
	}

	String MeshComponent::GetCategory()
	{
		return "Render";
	}

	String MeshComponent::GetIcon()
	{
		return "ui/UI4_image_component.png";
	}

	void MeshComponent::OnTransformUpdated()
	{
		auto newTransform = mOwner->transform->GetWorldNonSizedBasis();
		auto delta = newTransform*mTransform.Inverted();
		mTransform = newTransform;

		for (int i = 0; i < mMesh.vertexCount; i++)
		{
			auto v = mMesh.vertices[i];
			Vec2F newPos = v*delta;
			mMesh.vertices[i].Set(newPos, v.z, v.color, v.tu, v.tv);
		}
	}

	void MeshComponent::UpdateMesh()
	{
		if (spline.GetKeys().Count() < 3)
			return;

		std::vector<CDT::V2d<float>> verticies;
		std::vector<CDT::Edge> edges;

		for (int i = 1; i < spline.GetKeys().Count(); i++)
		{
			auto key = spline.GetKey(i);
			for (int j = 0; j < key.GetApproximatedPointsCount() - 1; j++)
			{
				Vec2F worldPoint = key.GetApproximatedPoints()[j].value;
				verticies.push_back(CDT::V2d<float>::make(worldPoint.x, worldPoint.y));

				if (verticies.size() > 1)
					edges.push_back(CDT::Edge(verticies.size() - 2, verticies.size() - 1));
			}
		}

		edges.push_back(CDT::Edge(verticies.size() - 1, 0));

		CDT::Triangulation<float> triangulation(CDT::VertexInsertionOrder::AsProvided);
		triangulation.insertVertices(verticies);
		triangulation.insertEdges(edges);
		triangulation.eraseOuterTriangles();

		mMesh.Resize(triangulation.vertices.size(), triangulation.triangles.size());

		auto texture = mImageAsset->GetAtlasTextureRef();
		Vec2F invTexSize(1.0f, 1.0f);
		if (texture)
			invTexSize.Set(1.0f/texture->GetSize().x, 1.0f/texture->GetSize().y);

		RectF imageRect = mImageAsset->GetAtlasRect();
		RectF imageUV = RectF(imageRect.left*invTexSize.x, imageRect.bottom*invTexSize.y,
							  imageRect.right*invTexSize.x, imageRect.top*invTexSize.y);

		for (int i = 0; i < triangulation.vertices.size(); i++)
		{
			Vec2F p(triangulation.vertices[i].x, triangulation.vertices[i].y);
			mMesh.vertices[i].Set(p*mTransform, 1.0f,
								  mColor.ARGB(),
								  imageUV.left + (p.x - mImageMapping.left)/mImageMapping.Width()*imageUV.Width(),
								  imageUV.bottom + (p.y - mImageMapping.bottom)/mImageMapping.Height()*imageUV.Height());
		}

		for (int i = 0; i < triangulation.triangles.size(); i++)
		{
			mMesh.indexes[i*3] = triangulation.triangles[i].vertices[0];
			mMesh.indexes[i*3 + 1] = triangulation.triangles[i].vertices[1];
			mMesh.indexes[i*3 + 2] = triangulation.triangles[i].vertices[2];
		}

		mMesh.SetTexture(mImageAsset->GetAtlasTextureRef());
		mMesh.vertexCount = triangulation.vertices.size();
		mMesh.polyCount = triangulation.triangles.size();
	}

	void MeshComponent::SetOwnerActor(Actor* actor)
	{
		DrawableComponent::SetOwnerActor(actor);
	}

	void MeshComponent::OnDeserialized(const DataValue& node)
	{
		DrawableComponent::OnDeserialized(node);
		UpdateMesh();
	}

	void MeshComponent::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		DrawableComponent::OnDeserializedDelta(node, origin);
		UpdateMesh();
	}

}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::MeshComponent>);

DECLARE_CLASS(o2::MeshComponent);
