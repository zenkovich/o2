#pragma once

#include "o2Editor/PropertiesWindow/ActorsViewer/IActorTransformViewer.h"

namespace Editor
{
	class Vec2FProperty;
	class FloatProperty;

	// -------------------------------------
	// Default editor actor transform viewer
	// -------------------------------------
	class DefaultActorTransformViewer: public IActorTransformViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorTransformViewer();

		// Virtual destructor
		~DefaultActorTransformViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors) override;

		// Updates properties values
		void Refresh() override;

		IOBJECT(DefaultActorTransformViewer);

	protected:
		Vector<Actor*> mTargetActors;

		Ref<Vec2FProperty> mPositionProperty;
		Ref<Vec2FProperty> mPivotProperty;
		Ref<Vec2FProperty> mScaleProperty;
		Ref<Vec2FProperty> mSizeProperty;
		Ref<FloatProperty> mRotationProperty;
		Ref<FloatProperty> mShearProperty;

		bool               mLayoutEnabled = false;
		Ref<Spoiler>       mLayoutSpoiler;
		Ref<Vec2FProperty> mAnchorRightTopProperty;
		Ref<Vec2FProperty> mAnchorLeftBottomProperty;
		Ref<Vec2FProperty> moffsetRightTopProperty;
		Ref<Vec2FProperty> mOffsetLeftBottomProperty;
		Ref<Vec2FProperty> mMinSizeProperty;
		Ref<Vec2FProperty> mMaxSizeProperty;
		Ref<Vec2FProperty> mWeightProperty;

	protected:
		// Called when some property changed, stores action for undo
		void OnPropertyChangeCompleted(const String& path, const Vector<DataDocument>& prevValue,
									   const Vector<DataDocument>& newValue);
	};
}
// --- META ---

CLASS_BASES_META(Editor::DefaultActorTransformViewer)
{
    BASE_CLASS(Editor::IActorTransformViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorTransformViewer)
{
    FIELD().PROTECTED().NAME(mTargetActors);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPositionProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPivotProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mScaleProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSizeProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRotationProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mShearProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mLayoutEnabled);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayoutSpoiler);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnchorRightTopProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnchorLeftBottomProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(moffsetRightTopProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOffsetLeftBottomProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mMinSizeProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mMaxSizeProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mWeightProperty);
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorTransformViewer)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetActors, const Vector<Actor*>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChangeCompleted, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
// --- END META ---
