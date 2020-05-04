#include "o2Editor/stdafx.h"
#include "IObjectPropertiesViewer.h"

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/EditorScope.h"

namespace Editor
{
	IObjectPropertiesViewer::IObjectPropertiesViewer()
	{
		mOnChildFieldChangeCompleted =
			MakeFunction<IObjectPropertiesViewer, void, const String&,
			const Vector<DataNode>&, const Vector<DataNode>&>(this, &IObjectPropertiesViewer::OnFieldChangeCompleted);
	}

	void IObjectPropertiesViewer::SetHeaderEnabled(bool enabled)
	{
		mHeaderEnabled = enabled;

		auto spoiler = GetSpoiler();
		if (mHeaderEnabled)
		{
			spoiler->SetHeadHeight(20);
			spoiler->GetLayerDrawable<Text>("caption")->enabled = true;
			spoiler->GetInternalWidget("expand")->enabledForcibly = true;
			spoiler->borderLeft = 10;
			spoiler->borderTop = 2;
		}
		else
		{
			spoiler->SetHeadHeight(0);
			spoiler->GetLayerDrawable<Text>("caption")->enabled = false;
			spoiler->GetInternalWidget("expand")->enabledForcibly = false;
			spoiler->borderLeft = 0;
			spoiler->borderTop = 0;
			spoiler->Expand();
		}

		OnHeaderEnableChanged(enabled);
	}

	bool IObjectPropertiesViewer::IsHeaderEnabled() const
	{
		return mHeaderEnabled;
	}

	void IObjectPropertiesViewer::Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		mTargetObjets = targetObjets;
		auto spoiler = GetSpoiler();

		if (!mHeaderEnabled || spoiler->IsExpanded())
		{
			CheckBuildProperties(targetObjets);
			mPropertiesContext.Set(targetObjets);
		}

		OnRefreshed(targetObjets);
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

	Spoiler* IObjectPropertiesViewer::GetSpoiler()
	{
		if (!mSpoiler)
		{
			mSpoiler = CreateSpoiler();
			mSpoiler->onExpand = [&]() { 
				Refresh(mTargetObjets);
			};
		}

		return mSpoiler;
	}

	void IObjectPropertiesViewer::SetExpanded(bool expanded)
	{
		GetSpoiler()->SetExpanded(expanded);
	}

	bool IObjectPropertiesViewer::IsExpanded() const
	{
		return const_cast<IObjectPropertiesViewer*>(this)->GetSpoiler()->IsExpanded();
	}

	void IObjectPropertiesViewer::SetCaption(const WString& caption)
	{
		GetSpoiler()->SetCaption(caption);
	}

	const WString& IObjectPropertiesViewer::GetCaption() const
	{
		return const_cast<IObjectPropertiesViewer*>(this)->GetSpoiler()->GetCaption();
	}

	bool IObjectPropertiesViewer::IsEmpty() const
	{
		return mSpoiler->GetChildren().IsEmpty();
	}

	Spoiler* IObjectPropertiesViewer::CreateSpoiler()
	{
		return o2UI.CreateWidget<Spoiler>("expand with caption");
	}

	void IObjectPropertiesViewer::CheckBuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (mPropertiesBuilt)
			return;

		PushEditorScopeOnStack scope;
		RebuildProperties(targetObjets);

		mPropertiesBuilt = true;
	}

	void IObjectPropertiesViewer::OnFieldChangeCompleted(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		onChangeCompleted(this->path + "/" + path, before, after);
	}

}

DECLARE_CLASS(Editor::IObjectPropertiesViewer);
