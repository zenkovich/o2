#include "stdafx.h"
#include "IObjectPropertiesViewer.h"

namespace Editor
{

	IObjectPropertiesViewer::IObjectPropertiesViewer()
	{
		mOnChildFieldChangeCompleted =
			MakeFunction<IObjectPropertiesViewer, void, const String&,
			const Vector<DataNode>&, const Vector<DataNode>&>(this, &IObjectPropertiesViewer::OnFieldChangeCompleted);
	}

	void IObjectPropertiesViewer::Refresh(const TargetsVec& targetObjets)
	{}

	const Type* IObjectPropertiesViewer::GetViewingObjectType() const
	{
		return nullptr;
	}

	Widget* IObjectPropertiesViewer::GetViewWidget() const
	{
		return mViewWidget;
	}

	void IObjectPropertiesViewer::OnFieldChangeCompleted(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		onChangeCompleted(this->path + "/" + path, before, after);
	}

}

DECLARE_CLASS(Editor::IObjectPropertiesViewer);
