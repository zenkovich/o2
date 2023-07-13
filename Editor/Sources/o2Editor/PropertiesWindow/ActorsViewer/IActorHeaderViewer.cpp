#include "o2Editor/stdafx.h"
#include "IActorHeaderViewer.h"

#include "o2/Scene/UI/Widget.h"

namespace Editor
{
	void IActorHeaderViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IActorHeaderViewer::IsEnabled() const
	{
		return mEnabled;
	}
}
// --- META ---

DECLARE_CLASS(Editor::IActorHeaderViewer);
// --- END META ---
