#include "o2Editor/stdafx.h"
#include "ImageAssetViewer.h"

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/BorderIntProperty.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/ImageSlicesEditorWidget.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	const Type* ImageAssetViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* ImageAssetViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(ImageAsset);
	}

	void ImageAssetViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		PushEditorScopeOnStack scope;

		mSlicesEditor = mnew ImageSlicesEditorWidget();
		mSpoiler->AddChild(mSlicesEditor);

		mAtlasProperty = dynamic_cast<AssetProperty*>(o2EditorProperties.CreateRegularField(
			&TypeOf(AtlasAssetRef), "Atlas", mOnChildFieldChangeCompleted, onChanged));

		mSpoiler->AddChild(mAtlasProperty);
	}

	void ImageAssetViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (!targetObjets.IsEmpty())
		{
			mSlicesEditor->Setup(ImageAssetRef(dynamic_cast<ImageAsset*>(targetObjets.Last().first)->GetUID()),
								 dynamic_cast<BorderIProperty*>(mPropertiesContext.properties[TypeOf(ImageAsset).GetField("sliceBorder")]));
		}

		mAtlasProxies.Clear();
		for (auto& targets : targetObjets)
		{
			ImageAsset* image = dynamic_cast<ImageAsset*>(targets.first);
			auto proxy = mnew FunctionalValueProxy<AtlasAssetRef>(
				[image](const AtlasAssetRef& value) { image->SetAtlas(value ? value->GetUID() : UID::empty); },
				[image]() { return AtlasAssetRef(image->GetAtlasUID()); }
			);

			mAtlasProxies.Add(proxy);
		}

		mAtlasProperty->SetValueProxy(mAtlasProxies);
	}

}
// --- META ---

DECLARE_CLASS(Editor::ImageAssetViewer, Editor__ImageAssetViewer);
// --- END META ---
