#include "o2Editor/stdafx.h"
#include "AssetPropertiesViewer.h"

#include "o2Editor/Core/Properties/ObjectViewer.h"

namespace Editor
{

	AssetPropertiesViewer::AssetPropertiesViewer()
	{
		mAssetHeader = mmake<Widget>();
		mAssetHeader->layout = make_ref<WidgetLayout>(WidgetLayout::HorStretch(VerAlign::Top, 5, 5, 25, 5));

		mAssetNameLabel = o2UI.CreateLabel("asset path");
		mAssetNameLabel->layout = make_ref<WidgetLayout>(WidgetLayout::HorStretch(VerAlign::Top, 0, 55, 20));
		mAssetNameLabel->horAlign = HorAlign::Left;
		mAssetHeader->AddChild(mAssetNameLabel);

		mSaveButton = o2UI.CreateButton("Save");
		mSaveButton->layout = make_ref<WidgetLayout>(WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(50, 17), Vec2F(0, 6)));
		mSaveButton->onClick = make_ref<Button::OnClickDelegate>(THIS_FUNC(OnSavePressed));
		mAssetHeader->AddChild(mSaveButton);

		auto separatorImg = o2UI.CreateImage("ui/UI4_Separator.png");
		separatorImg->layout = make_ref<WidgetLayout>(WidgetLayout::HorStretch(VerAlign::Bottom, -6, -15, 5, -4));
		mAssetHeader->AddChild(separatorImg);

		mContentWidget->AddChild(mAssetHeader, 0);
		mViewer->layout = make_ref<WidgetLayout>(WidgetLayout::BothStretch(5, 0, 5, 35));
	}

	const Type* AssetPropertiesViewer::GetViewingObjectType() const
	{
		return &TypeOf(Ref<Asset>);
	}

	void AssetPropertiesViewer::SetTargets(const Vector<IObject*>& targets)
	{
		DefaultPropertiesViewer::SetTargets(targets.Convert<IObject*>([](const IObject* x) {
			return DynamicCast<Ref<Asset>>(x)->Get();
		}));

		if (targets.Count() == 1)
		{
			if (const Ref<Asset>* assetRef = DynamicCast<const Ref<Asset>>(targets[0]))
				mAssetNameLabel->text = assetRef->Get()->GetPath();
		}
		else
		{
			mAssetNameLabel->text = "Multiple assets selected";
		}
	}

	void AssetPropertiesViewer::OnSavePressed()
	{
		for (IObject* target : mTargets)
		{
			if (Asset* asset = DynamicCast<Asset>(target))
				asset->Save();
		}

		o2Assets.RebuildAssets();
	}

}
// --- META ---

DECLARE_CLASS(Editor::AssetPropertiesViewer, Editor__AssetPropertiesViewer);
// --- END META ---