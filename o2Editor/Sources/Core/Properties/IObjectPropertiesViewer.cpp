#include "stdafx.h"
#include "IObjectPropertiesViewer.h"

#include "Core/EditorScope.h"
#include "Scene/UI/Widgets/VerticalLayout.h"

namespace Editor
{

	IObjectPropertiesViewer::IObjectPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mLayout = mnew VerticalLayout();
		mLayout->spacing = 5;
		mLayout->borderTop = 5;
		mLayout->expandHeight = false;
		mLayout->expandWidth = true;
		mLayout->fitByChildren = true;

		mOnChildFieldChangeCompleted =
			MakeFunction<IObjectPropertiesViewer, void, const String&,
			const Vector<DataNode>&, const Vector<DataNode>&>(this, &IObjectPropertiesViewer::OnFieldChangeCompleted);
	}

	void IObjectPropertiesViewer::Refresh(const TargetsVec& targetObjets)
	{
		mPropertiesContext.Set(targetObjets);
	}

	const Type* IObjectPropertiesViewer::GetViewingObjectType() const
	{
		return nullptr;
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
