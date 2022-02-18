#include "o2Editor/stdafx.h"
#include "IWidgetLayerHeadViewer.h"
namespace Editor
{
	void IWidgetLayerHeaderViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IWidgetLayerHeaderViewer::IsEnabled() const
	{
		return mEnabled;
	}
}

DECLARE_CLASS(Editor::IWidgetLayerHeaderViewer);
