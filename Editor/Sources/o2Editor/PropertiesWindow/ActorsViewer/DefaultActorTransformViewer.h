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
		void SetTargetActors(const Vector<Actor*>& actors);

		// Updates properties values
		void Refresh();

		IOBJECT(DefaultActorTransformViewer);

	protected:
		Vector<Actor*> mTargetActors;

		Vec2FProperty* mPositionProperty = nullptr;
		Vec2FProperty* mPivotProperty = nullptr;
		Vec2FProperty* mScaleProperty = nullptr;
		Vec2FProperty* mSizeProperty = nullptr;
		FloatProperty* mRotationProperty = nullptr;
		FloatProperty* mDepthProperty = nullptr;

		bool           mLayoutEnabled = false;
		Spoiler*       mLayoutSpoiler = nullptr;
		Vec2FProperty* mAnchorRightTopProperty = nullptr;
		Vec2FProperty* mAnchorLeftBottomProperty = nullptr;
		Vec2FProperty* moffsetRightTopProperty = nullptr;
		Vec2FProperty* mOffsetLeftBottomProperty = nullptr;
		Vec2FProperty* mMinSizeProperty = nullptr;
		Vec2FProperty* mMaxSizeProperty = nullptr;
		Vec2FProperty* mWeightProperty = nullptr;

	protected:
		// It is called when some property changed, stores action for undo
		void OnPropertyChangeCompleted(const String& path, const Vector<DataDocument>& prevValue,
									   const Vector<DataDocument>& newValue);
	};
}

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
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDepthProperty);
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
