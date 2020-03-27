#include "o2Editor/stdafx.h"
#include "ObjectViewer.h"

#include "IObjectPropertiesViewer.h"
#include "Properties.h"

namespace Editor
{
	ObjectViewer::ObjectViewer()
	{
		spacing = 5;
		expandHeight = false;
		expandWidth = true;
		fitByChildren = true;
		*layout = WidgetLayout::BothStretch();
	}

	void ObjectViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (targetObjets.IsEmpty())
			return;

		const Type* objectType = !targetObjets.IsEmpty() ? &targetObjets[0].first->GetType() : nullptr;

		bool requiredNewViewer = mPropertiesViewer ? 
			!objectType->IsBasedOn(*mPropertiesViewer->GetViewingObjectType()) : 
			objectType != nullptr;

		if (requiredNewViewer)
		{
			if (mPropertiesViewer)
			{
				RemoveChild(mPropertiesViewer->GetLayout(), false);
				o2EditorProperties.FreeObjectViewer(mPropertiesViewer);
			}

			mPropertiesViewer = o2EditorProperties.CreateObjectViewer(objectType, "");

			AddChild(mPropertiesViewer->GetLayout());
		}

		if (mPropertiesViewer)
			mPropertiesViewer->Refresh(targetObjets);
	}

	void ObjectViewer::Refresh(const Vector<IObject*>& targetObjets)
	{
		auto protoTargets = targetObjets.Select<Pair<IObject*, IObject*>>(
			[](IObject* x) { return Pair<IObject*, IObject*>(x, nullptr); });

		Refresh(protoTargets);
	}

	const Type* ObjectViewer::GetViewingObjectType() const
	{
		if (mPropertiesViewer)
			return mPropertiesViewer->GetViewingObjectType();

		return &TypeOf(IObject);
	}

}

DECLARE_CLASS(Editor::ObjectViewer);
