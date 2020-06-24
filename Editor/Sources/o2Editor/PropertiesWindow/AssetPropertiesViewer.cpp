#include "o2Editor/stdafx.h"
#include "AssetPropertiesViewer.h"

namespace Editor
{
	const Type* AssetPropertiesViewer::GetViewingObjectType() const
	{
		return &TypeOf(AssetRef);
	}

	void AssetPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		DefaultPropertiesViewer::SetTargets(targets.Convert<IObject*>([](IObject* x) {
			return dynamic_cast<AssetRef*>(x)->Get();
		}));
	}
}

DECLARE_CLASS(Editor::AssetPropertiesViewer);
