#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"

namespace o2
{
	class Toggle;
	class EditBox;
	class Button;
}

namespace Editor
{
	using namespace o2;

	class TagsProperty;
	class SceneLayerRefProperty;
	class BooleanProperty;
	class StringProperty;
	class AssetProperty;
	class FloatProperty;

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
		void SetTargetActors(const Vector<Actor*>& actors) override;

		// Returns data widget
		Widget* GetWidget() const override;

		// Updates properties values
		void Refresh() override;

		IOBJECT(DefaultActorHeaderViewer);

	public: 
		Vector<Actor*> mActors;

		Widget* mDataView = nullptr;

		BooleanProperty*       mEnableProperty = nullptr;
		StringProperty*        mNameProperty = nullptr;
		BooleanProperty*       mLockProperty = nullptr;
		AssetProperty*         mPrototypeProperty = nullptr;
		TagsProperty*          mTagsProperty = nullptr;
		SceneLayerRefProperty* mLayerProperty = nullptr;
		FloatProperty*         mDepthProperty = nullptr;

		Button* mPrototypeApplyBtn = nullptr;
		Button* mPrototypeRevertBtn = nullptr;
		Button* mPrototypeBreakBtn = nullptr;

	protected:
		// Refreshes layer property
		void RefreshLayer();

		// Called when apply button pressed, and applies changes to prototype
		void OnApplyPrototypePressed();

		// Called when revert button pressed and reverts actor to prototype
		void OnRevertPrototypePressed();

		// Called when break prototype buttons pressed and break prototype link
		void OnBreakPrototypePressed();

		// Called when selected inherit from parent layer
		void OnSelectedInheritedLayer();

		// Returns actor applying roots
		Vector<Actor*> GetRootApplyActors();

		// Called when some property changed, stores action for undo
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue, 
							   const Vector<DataDocument>& newValue);
	};
}
// --- META ---

CLASS_BASES_META(Editor::DefaultActorHeaderViewer)
{
	BASE_CLASS(Editor::IActorHeaderViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorHeaderViewer)
{
	FIELD().PUBLIC().NAME(mActors);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mDataView);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mEnableProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mNameProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mLockProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mPrototypeProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mTagsProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mLayerProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mDepthProperty);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mPrototypeApplyBtn);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mPrototypeRevertBtn);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(mPrototypeBreakBtn);
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorHeaderViewer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetTargetActors, const Vector<Actor*>&);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, GetWidget);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PROTECTED().SIGNATURE(void, RefreshLayer);
	FUNCTION().PROTECTED().SIGNATURE(void, OnApplyPrototypePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRevertPrototypePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnBreakPrototypePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelectedInheritedLayer);
	FUNCTION().PROTECTED().SIGNATURE(Vector<Actor*>, GetRootApplyActors);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
// --- END META ---
