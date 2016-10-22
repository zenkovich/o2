#pragma once

#include "Assets/ActorAsset.h"
#include "PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"
#include "Events/KeyboardEventsListener.h"

namespace o2
{
	class UIToggle;
	class UIEditBox;
	class UIButton;
}

namespace Editor
{
	using namespace o2;

	class TagsProperty;
	class LayerProperty;
	class BooleanProperty;
	class StringProperty;

	template<typename _type>
	class AssetProperty;

	// ---------------------------
	// Default actor header viewer
	// ---------------------------
	class DefaultActorHeaderViewer: public IActorHeaderViewer, public KeyboardEventsListener
	{
	public:
		// Default constructor
		DefaultActorHeaderViewer();

		// Virtual destructor
		~DefaultActorHeaderViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors);

		// Returns data widget
		UIWidget* GetWidget() const;

		IOBJECT(DefaultActorHeaderViewer);

	public:
		UIWidget*                  mDataView;
		BooleanProperty*           mEnableProperty;
		StringProperty*            mNameProperty;
		BooleanProperty*           mLockProperty;
		AssetProperty<ActorAsset>* mPrototypeProperty;
		UIButton*                  mPrototypeApplyBtn;
		UIButton*                  mPrototypeRevertBtn;
		UIButton*                  mPrototypeBreakBtn;
		TagsProperty*              mTagsProperty;
		LayerProperty*             mLayerProperty;

	protected:
		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);
	};
}