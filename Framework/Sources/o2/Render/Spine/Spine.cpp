#include "o2/stdafx.h"
#include "Spine.h"

#include "o2/Application/Input.h"
#include "o2/Render/Render.h"

#include "3rdPartyLibs/Spine/spine-cpp/include/spine/SkeletonRenderer.h"

namespace o2
{
    Spine::Spine()
    {}

    Spine::Spine(const Spine& other):
		Spine(other.mSpineAsset)
    {}

	Spine::Spine(const AssetRef<SpineAsset>& spine)
	{
		Load(spine);
	}

	Spine::~Spine()
    {
        Unload();
    }

    Spine& Spine::operator=(const Spine& other)
    {
        Unload();

        return *this;
    }

	void Spine::Load(const AssetRef<SpineAsset>& spine)
	{
        Unload();

		mSpineAsset = spine;
		if (!mSpineAsset || !mSpineAsset->GetSpineSkeletonData() || !mSpineAsset->GetSpineAnimationStateData())
			return;

		mSkeleton = mnew spine::Skeleton(mSpineAsset->GetSpineSkeletonData());
		mAnimationState = mnew spine::AnimationState(mSpineAsset->GetSpineAnimationStateData());

		mAnimationState->apply(*mSkeleton);
		mSkeleton->updateWorldTransform(spine::Physics::Physics_None);
	}

	void Spine::Unload()
	{
		if (mSkeleton)
		{
			delete mSkeleton;
			mSkeleton = nullptr;
		}

		if (mAnimationState)
		{
			delete mAnimationState;
			mAnimationState = nullptr;
		}
	}

	void Spine::Draw()
	{
		static spine::SkeletonRenderer skeletonRenderer;
		static Map<spine::BlendMode, BlendMode> blendModes = {
			{ spine::BlendMode_Normal, BlendMode::Normal },
			{ spine::BlendMode_Additive, BlendMode::Add },
			{ spine::BlendMode_Multiply, BlendMode::Normal },
			{ spine::BlendMode_Screen, BlendMode::Normal }
		};

		spine::RenderCommand* command = skeletonRenderer.render(*mSkeleton);
		while (command) 
		{
			Vertex vertex;

			float* positions = command->positions;
			float* uvs = command->uvs;
			uint32_t* colors = command->colors;
			uint16_t* indices = command->indices;
			Texture* texture = (Texture*)command->texture;

			mVertices.Clear();
			mIndices.Clear();

			for (int i = 0, j = 0, n = command->numVertices * 2; i < n; ++i, j += 2) 
			{
				Vec2F position(positions[j], positions[j + 1]);
				position = transform * position;

				vertex.x = position.x;
				vertex.y = position.y;
				vertex.tu = uvs[j];
				vertex.tv = 1.0f - uvs[j + 1];
				vertex.color = colors[i];
				mVertices.Add(vertex);
			}

			for (int i = 0, n = command->numIndices; i < n; ++i)
				mIndices.Add(indices[i]);

			BlendMode blendMode = blendModes[command->blendMode];
			TextureRef textureRef = TextureRef((Texture*)command->texture);

			o2Render.DrawBuffer(PrimitiveType::Polygon, mVertices.Data(), command->numVertices, mIndices.Data(), command->numIndices/3, textureRef, blendMode);

			if (o2Input.IsKeyDown(VK_F3))
				o2Render.DrawMeshBufferWire(mVertices.Data(), command->numVertices, mIndices.Data(), command->numIndices/3, Color4::Red());
				
			command = command->next;
		}

        OnDrawn();
    }

	void Spine::Update(float dt)
	{
		if (!mSkeleton || !mAnimationState)
			return;

		mAnimationState->update(dt);
		mAnimationState->apply(*mSkeleton);

		mSkeleton->update(dt);
		mSkeleton->updateWorldTransform(spine::Physics::Physics_None);
	}

}
