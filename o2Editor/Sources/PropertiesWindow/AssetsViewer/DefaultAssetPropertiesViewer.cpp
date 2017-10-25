#include "DefaultAssetPropertiesViewer.h"

#include "PropertiesWindow/PropertiesWindow.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	DefaultAssetPropertiesViewer::DefaultAssetPropertiesViewer()
	{
		mPropertiesLayout = mnew UIVerticalLayout();
		mPropertiesLayout->name = "properties";
		mPropertiesLayout->spacing = 5.0f;
		mPropertiesLayout->border = RectF(5, 5, 5, 5);
		mPropertiesLayout->expandHeight = false;
		mPropertiesLayout->expandWidth = true;
		mPropertiesLayout->fitByChildren = true;
		mPropertiesLayout->baseCorner = BaseCorner::Top;
		*mPropertiesLayout->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 10);
	}

	DefaultAssetPropertiesViewer::~DefaultAssetPropertiesViewer()
	{
		delete mPropertiesLayout;
	}

	void DefaultAssetPropertiesViewer::SetTargetAssets(const Vector<AssetRef*>& assets)
	{
		mTargetAssets = assets;
		mFieldProperties.Set(assets.Select<IObject*>([](auto x) { return (IObject*)(*x)->GetMeta(); }));
	}

	const Type* DefaultAssetPropertiesViewer::GetAssetType() const
	{
		return mAssetType;
	}

	UIWidget* DefaultAssetPropertiesViewer::GetWidget() const
	{
		return mPropertiesLayout;
	}

	void DefaultAssetPropertiesViewer::SpecializeAssetType(const Type* type)
	{
		mAssetType = type;
		o2EditorProperties.BuildObjectProperties((UIVerticalLayout*)mPropertiesLayout, type, mFieldProperties, "");

		mPropertiesLayout->name = "asset properties " + type->GetName();
	}

}

CLASS_META(Editor::DefaultAssetPropertiesViewer)
{
	BASE_CLASS(Editor::IAssetPropertiesViewer);

	PROTECTED_FIELD(mPropertiesLayout);
	PROTECTED_FIELD(mTargetAssets);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mAssetType);

	PUBLIC_FUNCTION(void, SetTargetAssets, const Vector<AssetRef*>&);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SpecializeAssetType, const Type*);
}
END_META;
