#pragma once

#include "o2/Assets/Types/Spine/SpineAsset.h"
#include "o2/Assets/Types/Spine/SpineAtlasAsset.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Basic/ICloneable.h"
#include "o2/Utils/Math/Vertex.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Types/CommonTypes.h"

#include "spine/Skeleton.h"
#include "spine/AnimationState.h"

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
		// ---------------------
		// Spine animation track
		// ---------------------
		class Track: public RefCounterable
        {
        public:
			// Default constructor
            Track(const Ref<Spine>& owner, int trackIndex, const String& name);

			// Starts playing animation
            void Play();

			// Stops playing animation
            void Stop();

			// Starts or stops playing animation
			void SetPlaying(bool playing);

            // Returns true if animation is playing
			bool IsPlaying() const; 

			// Sets animation loop
			void SetLoop(bool loop);

			// Returns true if animation is looped
			bool IsLooped() const;

			// Sets animation time
			void SetTime(float time);

			// Returns animation time
			float GetTime() const;

			// Sets animation weight for blending
			void SetWeight(float weight);

			// Returns animation weight
			float GetWeight() const;

		private:
			WeakRef<Spine> mOwner; // Spine reference

			String mAnimationName;   // Animation name
			int    mTrackIndex = -1; // Spine track index

			bool mPlaying = false; // Is animation playing
			bool mLooped = false;  // Is animation looped

			spine::TrackEntry* mTrackEntry = nullptr; // Spine track entry
        };

    public:
        Basis transform; // Root transform                                       

    public:
        // Constructor
        explicit Spine(RefCounter* refCounter);

		// Constructor with spine asset
		Spine(RefCounter* refCounter, const AssetRef<SpineAsset>& spine);

        // Copy-constructor
        Spine(RefCounter* refCounter, const Spine& other);

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

		// Returns spine animation names
		const Vector<String>& GetAnimationNames() const;

		// Returns spine animation track
		Ref<Track> GetTrack(const String& name);

        CLONEABLE_REF(Spine);

    protected:
		AssetRef<SpineAsset> mSpineAsset; // Spine asset @SERIALIZABLE @EDITOR_PROPERTY

		spine::Skeleton*       mSkeleton = nullptr;       // Spine skeleton
		spine::AnimationState* mAnimationState = nullptr; // Spine animation state

		Vector<String> mAnimationNames; // Animation names

		Vector<Vertex>      mVertices; // Vertices buffer
		Vector<VertexIndex> mIndices;  // Indices buffer

        friend class Render;
    };
}
