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
	FIELD().NAME(mTargetActors).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPositionProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPivotProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mScaleProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSizeProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mRotationProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDepthProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mLayoutEnabled).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayoutSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAnchorRightTopProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAnchorLeftBottomProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(moffsetRightTopProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mOffsetLeftBottomProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mMinSizeProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mMaxSizeProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mWeightProperty).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorTransformViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnPropertyChangeCompleted, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
