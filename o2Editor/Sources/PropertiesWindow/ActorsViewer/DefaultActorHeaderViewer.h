#pragma once

#include "Assets/ActorAsset.h"
#include "PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"

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

	template<typename _type>
	class AssetProperty;

	// ---------------------------
	// Default actor header viewer
	// ---------------------------
	class DefaultActorHeaderViewer: public IActorHeaderViewer
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
		UIToggle*                  mEnableToggle;
		UIToggle*                  mLockToggle;
		AssetProperty<ActorAsset>* mPrefabProperty;
		UIButton*                  mPrefabApplyBtn;
		UIButton*                  mPrefabRevertBtn;
		UIButton*                  mPrefabBreakBtn;
		TagsProperty*              mTagsProperty;
		LayerProperty*             mLayerProperty;
	};
}