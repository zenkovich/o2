#pragma once

#include "o2/Assets/Types/Spine/SpineAsset.h"
#include "o2/Assets/Types/Spine/SpineAtlasAsset.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Basic/ICloneable.h"
#include "o2/Utils/Math/Vertex.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Types/CommonTypes.h"

#include "3rdPartyLibs/Spine/spine-cpp/include/spine/Skeleton.h"
#include "3rdPartyLibs/Spine/spine-cpp/include/spine/AnimationState.h"

namespace o2
{
    // --------------------------------
    // EsotericSoftware Spine animation
    // --------------------------------
    class Spine : public virtual IDrawable, public virtual RefCounterable, public ICloneableRef
    {
    public:
        PROPERTIES(Spine);

    public:
        Basis transform; // Root transform                                       

    public:
        // Constructor
        Spine();

		// Constructor with spine asset
		Spine(const AssetRef<SpineAsset>& spine);

        // Copy-constructor
        Spine(const Spine& other);

        // Destructor
        ~Spine();

        // Assign operator
        Spine& operator=(const Spine& other);

		// Load spine skeleton
		void Load(const AssetRef<SpineAsset>& spine);

		// Unload spine skeleton
		void Unload();

        // Drawing SkinnableMesh
        void Draw() override;

		// Updates spine animation
		void Update(float dt);

        CLONEABLE_REF(Spine);

    protected:
		AssetRef<SpineAsset> mSpineAsset; // Spine asset @SERIALIZABLE @EDITOR_PROPERTY

		spine::Skeleton*       mSkeleton = nullptr;       // Spine skeleton
		spine::AnimationState* mAnimationState = nullptr; // Spine animation state

		Vector<Vertex>      mVertices; // Vertices buffer
		Vector<VertexIndex> mIndices;  // Indices buffer

        friend class Render;
    };
}
