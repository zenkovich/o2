#include "stdafx.h"
#include "DefaultWidgetLayerPropertiesViewer.h"

#include "Core/Properties/Properties.h"
#include "Core/UI/SpoilerWithHead.h"

namespace Editor
{

	DefaultWidgetLayerPropertiesViewer::DefaultWidgetLayerPropertiesViewer()
	{}

	DefaultWidgetLayerPropertiesViewer::~DefaultWidgetLayerPropertiesViewer()
	{}

	void DefaultWidgetLayerPropertiesViewer::SetTargetLayers(const Vector<UIWidgetLayer*>& layers)
	{
		mLayers = layers;
		Refresh();
	}

	const o2::Type* DefaultWidgetLayerPropertiesViewer::GetDrawableType() const
	{
		return mDrawableType;
	}

	void DefaultWidgetLayerPropertiesViewer::Refresh()
	{
		mFieldProperties.Set(mLayers.Select<Pair<IObject*, IObject*>>([](UIWidgetLayer* x) {
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), nullptr);
		}));
	}

	void DefaultWidgetLayerPropertiesViewer::Rebuild()
	{
		static Vector<String> hiddenFields = {
			"name", "enabled", "locked", "layout"
		};

		o2EditorProperties.FreeProperties(mFieldProperties);

		auto fields = TypeOf(UIWidgetLayer).GetFieldsWithBaseClasses();
		if (!o2EditorProperties.IsPrivateFieldsVisible())
			fields.RemoveAll([&](FieldInfo* x) { return hiddenFields.Contains(x->GetName()); });

		o2EditorProperties.BuildObjectProperties(mSpoiler,
												 fields, mFieldProperties, "");

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

}

DECLARE_CLASS(Editor::DefaultWidgetLayerPropertiesViewer);
