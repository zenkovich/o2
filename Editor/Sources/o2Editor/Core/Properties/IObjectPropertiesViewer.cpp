#include "o2Editor/stdafx.h"
#include "IObjectPropertiesViewer.h"

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/EditorScope.h"

namespace Editor
{

	IObjectPropertiesViewer::IObjectPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mLayout = mnew VerticalLayout();
		mLayout->spacing = 5;
		mLayout->borderTop = 3;
		mLayout->expandHeight = false;
		mLayout->expandWidth = true;
		mLayout->fitByChildren = true;

		mOnChildFieldChangeCompleted =
			MakeFunction<IObjectPropertiesViewer, void, const String&,
			const Vector<DataNode>&, const Vector<DataNode>&>(this, &IObjectPropertiesViewer::OnFieldChangeCompleted);
	}

	void IObjectPropertiesViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		mPropertiesContext.Set(targetObjets);
	}

	const Type* IObjectPropertiesViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* IObjectPropertiesViewer::GetViewingObjectTypeStatic()
	{
		return nullptr;
	}

	void IObjectPropertiesViewer::SetParentContext(PropertiesContext* context)
	{
		mPropertiesContext.parent = context;
	}

	VerticalLayout* IObjectPropertiesViewer::GetLayout() const
	{
		return mLayout;
	}

	bool IObjectPropertiesViewer::IsEmpty() const
	{
		return mLayout->GetChildren().IsEmpty();
	}

	void IObjectPropertiesViewer::OnFieldChangeCompleted(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		onChangeCompleted(this->path + "/" + path, before, after);
	}

}

DECLARE_CLASS(Editor::IObjectPropertiesViewer);
