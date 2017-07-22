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

		// Updates properties values
		void Refresh();

		IOBJECT(DefaultActorHeaderViewer);

	public:
		Vector<Actor*>                mActors;

		UIWidget*                     mDataView;
		BooleanProperty*              mEnableProperty;
		StringProperty*               mNameProperty;
		BooleanProperty*              mLockProperty;
		AssetProperty<ActorAssetRef>* mPrototypeProperty;
		UIButton*                     mPrototypeApplyBtn;
		UIButton*                     mPrototypeRevertBtn;
		UIButton*                     mPrototypeBreakBtn;
		TagsProperty*                 mTagsProperty;
		LayerProperty*                mLayerProperty;

	protected:
		// It is called when apply button pressed, and applies changes to prototype
		void OnApplyPrototypePressed();

		// It is called when revert button pressed and reverts actor to prototype
		void OnRevertPrototypePressed();

		// It is called when break prototype buttons pressed and break prototype link
		void OnBreakPrototypePressed();

		// Returns actor applying roots
		Vector<Actor*> GetRootApplyActors();

		// It is called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue);
	};
}
