#include "o2Editor/stdafx.h"
#include "DefaultWidgetLayerHeadViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/BooleanProperty.h"
#include "o2Editor/Core/Properties/Basic/StringProperty.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	DefaultWidgetLayerHeaderViewer::DefaultWidgetLayerHeaderViewer()
	{
		PushEditorScopeOnStack scope;

		mDataView = mnew Widget();
		mDataView->name = "actor head";
		mDataView->layout->minHeight = 21;

		mEnableProperty = o2UI.CreateWidget<BooleanProperty>("actor head enable");
		*mEnableProperty->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, 0));
		mEnableProperty->SetValuePath("enabled");
		mEnableProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mEnableProperty);

		mNameProperty = o2UI.CreateWidget<StringProperty>("actor head name");
		*mNameProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 21, 15, 17, 2);
		mNameProperty->SetValuePath("name");
		mNameProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mNameProperty);

		mLockProperty = o2UI.CreateWidget<BooleanProperty>("actor head lock");
		*mLockProperty->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, -1));
		mLockProperty->SetValuePath("locked");
		mLockProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mLockProperty);
	}

	DefaultWidgetLayerHeaderViewer::~DefaultWidgetLayerHeaderViewer()
	{
		if (mEnableProperty)
			delete mEnableProperty;

		if (mNameProperty)
			delete mNameProperty;

		if (mLockProperty)
			delete mLockProperty;

		if (mDataView)
			delete mDataView;
	}

	void DefaultWidgetLayerHeaderViewer::SetTargetLayers(const Vector<WidgetLayer*>& layers)
	{
		mLayers = layers;

		Vector<WidgetLayer*> prototypes = layers.Convert<WidgetLayer*>([](WidgetLayer* x) { return nullptr; });
// 		auto prototypes = layers.Convert<WidgetLayer*>([](WidgetLayer* x) { 
// 			return x->GetOwnerWidget()->GetPrototypeLink().Get(); });

		mEnableProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayer::enabled)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->enabled; });

		mNameProperty->SelectValueAndPrototypePointers<String, WidgetLayer>(
			layers, prototypes, [](WidgetLayer* x) { return &x->name; });

		mLockProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayer::locked)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->locked; });
	}

	Widget* DefaultWidgetLayerHeaderViewer::GetWidget() const
	{
		return mDataView;
	}

	void DefaultWidgetLayerHeaderViewer::Refresh()
	{
		mEnableProperty->Refresh();
		mNameProperty->Refresh();
		mLockProperty->Refresh();
	}

	void DefaultWidgetLayerHeaderViewer::OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue,
														   const Vector<DataDocument>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}
// --- META ---

DECLARE_CLASS(Editor::DefaultWidgetLayerHeaderViewer, Editor__DefaultWidgetLayerHeaderViewer);
// --- END META ---
