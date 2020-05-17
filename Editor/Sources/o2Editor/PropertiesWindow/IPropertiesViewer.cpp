#include "o2Editor/stdafx.h"
#include "IPropertiesViewer.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Serialization/DataValue.h"

namespace Editor
{
	IPropertiesViewer::~IPropertiesViewer()
	{
		delete mContentWidget;
	}

	const Type* IPropertiesViewer::GetViewingObjectType() const
	{
		return nullptr;
	}

}

DECLARE_CLASS(Editor::IPropertiesViewer);
