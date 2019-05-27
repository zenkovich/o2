#include "stdafx.h"
#include "ITrackControl.h"

namespace Editor
{

	void ITrackControl::Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet)
	{}

	void ITrackControl::SetAnimatedValue(IAnimatedValue* animatedValue)
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

	IPropertyField* ITrackControl::GetPropertyField() const
	{
		return nullptr;
	}

	Button* ITrackControl::GetAddKeyButton() const
	{
		return nullptr;
	}

	void ITrackControl::InsertNewKey(float time)
	{}

	bool ITrackControl::KeyHandle::operator==(const KeyHandle& other) const
	{
		return handle == other.handle;
	}
}

DECLARE_CLASS(Editor::ITrackControl);
