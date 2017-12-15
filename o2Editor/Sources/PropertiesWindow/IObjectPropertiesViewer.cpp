#include "IObjectPropertiesViewer.h"

#include "UI/Widget.h"
#include "Utils/Data/DataNode.h"

CLASS_META(Editor::IObjectPropertiesViewer)
{
	BASE_CLASS(o2::IObject);

	PROTECTED_FIELD(mContentWidget);

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, Draw);
}
END_META;
