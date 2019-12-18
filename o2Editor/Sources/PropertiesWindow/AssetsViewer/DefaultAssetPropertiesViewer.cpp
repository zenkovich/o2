#include "stdafx.h"
#include "DefaultAssetPropertiesViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	DefaultAssetPropertiesViewer::DefaultAssetPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mPropertiesLayout = mnew VerticalLayout();
		mPropertiesLayout->name = "properties";
		mPropertiesLayout->spacing = 5.0f;
		mPropertiesLayout->border = BorderF(5, 5, 5, 5);
		mPropertiesLayout->expandHeight = false;
		mPropertiesLayout->expandWidth = true;
		mPropertiesLayout->fitByChildren = true;
		mPropertiesLayout->baseCorner = BaseCorner::Top;
		*mPropertiesLayout->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 10);
	}

	DefaultAssetPropertiesViewer::~DefaultAssetPropertiesViewer()
	{
		delete mPropertiesLayout;
	}

	void DefaultAssetPropertiesViewer::SetTargetAssets(const Vector<AssetRef*>& assets)
	{
		mTargetAssets = assets;
		mPropertiesContext.Set(assets.Select<Pair<IObject*, IObject*>>([](auto x) { 
			return Pair<IObject*, IObject*>((IObject*)(*x)->GetMeta(), nullptr); }));
	}

	const Type* DefaultAssetPropertiesViewer::GetAssetType() const
	{
		return mAssetType;
	}

	Widget* DefaultAssetPropertiesViewer::GetWidget() const
	{
		return mPropertiesLayout;
	}

	void DefaultAssetPropertiesViewer::SpecializeAssetType(const Type* type)
	{
		PushEditorScopeOnStack scope;

		mAssetType = type;
		o2EditorProperties.BuildObjectProperties((VerticalLayout*)mPropertiesLayout, type, mPropertiesContext, "");

		mPropertiesLayout->name = "asset properties " + type->GetName();
	}

}

DECLARE_CLASS(Editor::DefaultAssetPropertiesViewer);
