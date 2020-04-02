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

	void ObjectViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjects)
	{
		if (targetObjects.IsEmpty())
			return;

		const Type* objectType = !targetObjects.IsEmpty() ? &targetObjects[0].first->GetType() : nullptr;

		for (auto target : targetObjects)
		{
			if (!target.first->GetType().IsBasedOn(*objectType))
				return;
		}

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
			mPropertiesViewer->Refresh(targetObjects);
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
