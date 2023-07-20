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
			&TypeOf(AtlasAssetRef), "Atlas", mOnChildFieldChangeCompleted, [&](IPropertyField* field)
			{
				for (auto& target : mPropertiesContext.targets)
				{
					dynamic_cast<ImageAsset*>(target.first)->SetAtlas(mAtlasProperty->GetCommonValue() ?
																	  mAtlasProperty->GetCommonValue()->GetUID() :
																	  UID::empty);
				}

				onChanged(field);
			}));

		mSpoiler->AddChild(mAtlasProperty);
	}

	void ImageAssetViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (!targetObjets.IsEmpty())
		{
			mSlicesEditor->Setup(ImageAssetRef(dynamic_cast<ImageAsset*>(targetObjets.Last().first)->GetUID()),
								 dynamic_cast<BorderIProperty*>(mPropertiesContext.properties[TypeOf(ImageAsset).GetField("sliceBorder")]));

			mAtlasProperty->SetValue(AtlasAssetRef(dynamic_cast<ImageAsset*>(targetObjets[0].first)->GetAtlas()));
		}
	}

}
// --- META ---

DECLARE_CLASS(Editor::ImageAssetViewer);
// --- END META ---
