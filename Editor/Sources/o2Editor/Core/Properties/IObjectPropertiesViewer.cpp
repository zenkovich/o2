#include "o2Editor/stdafx.h"
#include "IObjectPropertiesViewer.h"

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"

namespace Editor
{
	IObjectPropertiesViewer::IObjectPropertiesViewer()
	{
		mOnChildFieldChangeCompleted =
			MakeFunction<IObjectPropertiesViewer, void, const String&,
			const Vector<DataDocument>&, const Vector<DataDocument>&>(this, &IObjectPropertiesViewer::OnFieldChangeCompleted);
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
		mTargetObjects = targetObjets;
		auto spoiler = GetSpoiler();

		if (!mHeaderEnabled || spoiler->IsExpanded())
		{
			bool force = CheckBuildProperties(targetObjets);
			mPropertiesContext->Set(targetObjets, force);
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

	void IObjectPropertiesViewer::SetParentContext(const Ref<PropertiesContext>& context)
	{
		mPropertiesContext->parent = context;
	}

	const Ref<Spoiler>& IObjectPropertiesViewer::GetSpoiler()
	{
		if (!mSpoiler)
		{
			mSpoiler = CreateSpoiler();
			mSpoiler->onExpand = [&]() { 
				Refresh(mTargetObjects);
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

	Ref<Spoiler> IObjectPropertiesViewer::CreateSpoiler()
	{
		return o2UI.CreateWidget<Spoiler>("expand with caption");
	}

	bool IObjectPropertiesViewer::CheckBuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (mPropertiesBuilt)
			return false;

		PushEditorScopeOnStack scope;
		RebuildProperties(targetObjets);

		mPropertiesBuilt = true;

		return true;
	}

	void IObjectPropertiesViewer::OnFieldChangeCompleted(const String& path, const Vector<DataDocument>& before, 
														 const Vector<DataDocument>& after)
	{
		onChangeCompleted(this->path + "/" + path, before, after);
	}

}
// --- META ---

DECLARE_CLASS(Editor::IObjectPropertiesViewer, Editor__IObjectPropertiesViewer);
// --- END META ---
