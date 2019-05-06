#include "stdafx.h"
#include "ITrackControl.h"

namespace Editor
{
	void ITrackControl::SetAnimatedValue(IAnimatedValue* animatedValue)
	{}

	void ITrackControl::SetTimeline(AnimationTimeline* timeline)
	{}

	void ITrackControl::SetKeyHandlesSheet(KeyHandlesSheet* handlesSheet)
	{}

	void ITrackControl::UpdateHandles()
	{}

	ITrackControl::KeyHandlesVec ITrackControl::GetKeyHandles() const
	{
		return KeyHandlesVec();
	}

	float ITrackControl::GetKeyPosition(int idx) const
	{
		return 0.0f;
	}

	bool ITrackControl::KeyHandle::operator==(const KeyHandle& other) const
	{
		return handle == other.handle;
	}
}

DECLARE_CLASS(Editor::ITrackControl);
