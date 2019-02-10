#include "stdafx.h"
#include "DefaultWidgetLayerPropertiesViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/Button.h"

namespace Editor
{

	DefaultWidgetLayerPropertiesViewer::DefaultWidgetLayerPropertiesViewer()
	{
		PushScopeEnterOnStack scope;
		mFitSizeButton = o2UI.CreateButton("Fit size by drawable", THIS_FUNC(FitLayerByDrawable));
	}

	DefaultWidgetLayerPropertiesViewer::~DefaultWidgetLayerPropertiesViewer()
	{}

	void DefaultWidgetLayerPropertiesViewer::SetTargetLayers(const Vector<WidgetLayer*>& layers)
	{
		mLayers = layers;
		Refresh();
	}

	const Type* DefaultWidgetLayerPropertiesViewer::GetDrawableType() const
	{
		return mDrawableType;
	}

	void DefaultWidgetLayerPropertiesViewer::Refresh()
	{
		mFieldProperties.Set(mLayers.Select<Pair<IObject*, IObject*>>([](WidgetLayer* x) {
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), nullptr);
		}));
	}

	void DefaultWidgetLayerPropertiesViewer::Rebuild()
	{
		PushScopeEnterOnStack scope;

		static Vector<String> hiddenFields = {
			"name", "enabled", "locked", "layout"
		};

		o2EditorProperties.FreeProperties(mFieldProperties);

		auto fields = TypeOf(WidgetLayer).GetFieldsWithBaseClasses();
		if (!o2EditorProperties.IsPrivateFieldsVisible())
			fields.RemoveAll([&](FieldInfo* x) { return hiddenFields.Contains(x->GetName()); });

		o2EditorProperties.BuildObjectProperties(mSpoiler,
												 fields, mFieldProperties, "");

		mFitSizeButton->SetParent(nullptr);
		mFitSizeButton->SetParent(mSpoiler);

		mBuiltWithHidden = o2EditorProperties.IsPrivateFieldsVisible();
	}

	bool DefaultWidgetLayerPropertiesViewer::IsEmpty() const
	{
		return mSpoiler->GetChildren().Count() == 0;
	}

	bool DefaultWidgetLayerPropertiesViewer::IsBuiltWithHiddenFields() const
	{
		return mBuiltWithHidden;
	}

	void DefaultWidgetLayerPropertiesViewer::FitLayerByDrawable()
	{
		for (auto layer : mLayers)
		{
			if (Sprite* sprite = dynamic_cast<Sprite*>(layer->GetDrawable()))
				layer->layout.size = sprite->GetImageAsset()->GetSize();

			if (Text* text = dynamic_cast<Text*>(layer->GetDrawable()))
				layer->layout.size = text->GetRealSize();
		}
	}

}

DECLARE_CLASS(Editor::DefaultWidgetLayerPropertiesViewer);
