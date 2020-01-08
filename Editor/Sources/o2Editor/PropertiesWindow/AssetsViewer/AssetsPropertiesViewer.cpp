#include "o2Editor/stdafx.h"
#include "AssetsPropertiesViewer.h"

#include "o2/Assets/Assets.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/PropertiesWindow/AssetsViewer/DefaultAssetPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/AssetsViewer/IAssetPropertiesViewer.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace Editor
{

	AssetsPropertiesViewer::AssetsPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		auto viewersTypes = TypeOf(IAssetPropertiesViewer).GetDerivedTypes();
		for (auto type : viewersTypes)
		{
			if (*type != TypeOf(DefaultAssetPropertiesViewer))
				mAvailableAssetViewers.Add((IAssetPropertiesViewer*)(type->CreateSample()));
		}

		InitializeHeadWidget();
	}

	AssetsPropertiesViewer::~AssetsPropertiesViewer()
	{
		for (auto viewer : mAvailableAssetViewers)
			delete viewer;
	}

	const Type* AssetsPropertiesViewer::GetViewingObjectType() const
	{
		return &TypeOf(AssetRef);
	}

	void AssetsPropertiesViewer::AddAssetViewer(IAssetPropertiesViewer* viewer)
	{
		mAvailableAssetViewers.Add(viewer);
	}

	void AssetsPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		PushEditorScopeOnStack scope;

		mTargetAssets = targets.Cast<AssetRef*>();

		if (mTargetAssets.IsEmpty())
			return;

		bool typesAreSame = true;
		const Type* commonType = &(*mTargetAssets[0])->GetType();
		for (int i = 1; i < mTargetAssets.Count(); i++)
		{
			const Type* assetType = &(*mTargetAssets[i])->GetType();

			if (assetType != commonType)
			{
				typesAreSame = false;
				break;
			}
		}

		if (!typesAreSame)
		{
			if (mCurrentViewer)
				mContentWidget->RemoveChild(mCurrentViewer->GetWidget(), false);

			mCurrentViewer = nullptr;
			return;
		}

		IAssetPropertiesViewer* viewer = mAvailableAssetViewers.FindMatch([&](IAssetPropertiesViewer* x) {
			return commonType->IsBasedOn(*x->GetAssetType());
		});

		if (!viewer)
		{
			auto defaultViewer = mnew DefaultAssetPropertiesViewer();
			defaultViewer->SpecializeAssetType(&(*mTargetAssets[0])->GetMeta()->GetType());
			mAvailableAssetViewers.Add(defaultViewer);
			viewer = defaultViewer;
		}

		if (viewer != mCurrentViewer)
		{
			if (mCurrentViewer)
				mContentWidget->RemoveChild(mCurrentViewer->GetWidget(), false);

			mCurrentViewer = viewer;

			if (mCurrentViewer)
			{
				mContentWidget->AddChild(mCurrentViewer->GetWidget());
				mCurrentViewer->SetTargetAssets(mTargetAssets);
			}
		}
		else if (mCurrentViewer)
			mCurrentViewer->SetTargetAssets(mTargetAssets);
	}

	void AssetsPropertiesViewer::OnEnabled()
	{}

	void AssetsPropertiesViewer::OnDisabled()
	{
		mTargetAssets.Clear();
	}

	void AssetsPropertiesViewer::Update(float dt)
	{}

	void AssetsPropertiesViewer::Draw()
	{}

	void AssetsPropertiesViewer::InitializeHeadWidget()
	{
		auto contentWidget = o2UI.CreateVerLayout();
		contentWidget->name          = "asset viewer layout";
		contentWidget->spacing       = 0.0f;
		contentWidget->border        = BorderF();
		contentWidget->expandHeight  = false;
		contentWidget->expandWidth   = true;
		contentWidget->fitByChildren = true;
		contentWidget->baseCorner    = BaseCorner::Top;
		*contentWidget->layout       = WidgetLayout::BothStretch();

		mContentWidget = contentWidget;
	}

	void AssetsPropertiesViewer::OnNameTextEditChanged(const WString& value)
	{
		if (mTargetAssets.Count() != 1)
			return;

		String ext = FileSystem::GetFileExtension((*mTargetAssets.Last())->GetPath());

		if (ext.IsEmpty())
			o2Assets.RenameAsset(*mTargetAssets.Last(), value);
		else
			o2Assets.RenameAsset(*mTargetAssets.Last(), value + "." + ext);
	}
}

DECLARE_CLASS(Editor::AssetsPropertiesViewer);
