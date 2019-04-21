#pragma once

#include "AnimationWindow/Timeline.h"
#include "Scene/UI/Widget.h"
#include "Utils/Editor/DragHandle.h"
#include "ITrackControl.h"

using namespace o2;

namespace Editor
{
	// ---------------------------------------------------------------------
	// Animation control track for mapped keys from children animated values
	// ---------------------------------------------------------------------
	class MapKeyFramesTrackControl : public Widget
	{
	public:
		// Default constructor
		MapKeyFramesTrackControl();

		// Copy-constructor
		MapKeyFramesTrackControl(const MapKeyFramesTrackControl& other);

		// Destructor
		~MapKeyFramesTrackControl();


		// Copy-operator
		MapKeyFramesTrackControl& operator=(const MapKeyFramesTrackControl& other);


		// Draws handles with scissor rect
		void Draw() override;


		// Sets mapped control tracks. Creates handles to control these mapped controltracks
		void SetMappedControlTracks(const Vector<ITrackControl*>& controls);

		// Returns mapped control tracks
		const Vector<ITrackControl*>& GetMappedControlTracks() const;


		// Sets timeline for calculating handles positions
		void SetTimeline(AnimationTimeline* timeline);

		SERIALIZABLE(MapKeyFramesTrackControl);

	private:
		struct KeyHandle
		{
			int keyIdx = 0;
			WidgetDragHandle* handle = nullptr;
			

			bool operator==(const KeyHandle& other) const { return handle == other.handle; }
		};
		typedef Vector<KeyHandle> KeyHandlesVec;

	private:
		KeyHandlesVec      mHandles;                 // List of handles, each for keys
		AnimatedValueType* mAnimatedValue = nullptr; // Editing animated value
		AnimationTimeline* mTimeline = nullptr;      // Timeline used for calculating handles positions

	private:
		void UpdateKeysHandles();
		void UpdateView();

		WidgetDragHandle* CreateHandle();
	};
}