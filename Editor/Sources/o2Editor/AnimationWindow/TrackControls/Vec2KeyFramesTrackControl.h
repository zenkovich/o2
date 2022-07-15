#pragma once
#include "KeyFramesTrackControl.h"
#include "o2Editor/Core/Tools/SplineTool.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

using namespace o2;

namespace Editor
{
	// -------------------------------------------------
	// Animation control track for Vec2F animation track
	// -------------------------------------------------
	class Vec2KeyFramesTrackControl : public KeyFramesTrackControl<AnimationTrack<Vec2F>>
	{
	public:
		typedef KeyFramesTrackControl<AnimationTrack<Vec2F>> Base;

	public:
		// Default constructor
		Vec2KeyFramesTrackControl();

		// Copy-constructor
		Vec2KeyFramesTrackControl(const Vec2KeyFramesTrackControl& other);

		// Destructor
		~Vec2KeyFramesTrackControl();

		// Copy-operator
		Vec2KeyFramesTrackControl& operator=(const Vec2KeyFramesTrackControl& other);

		// Sets track active when user selects it
		void SetActive(bool active) override;

		// Sets curves edit view mode
		void SetCurveViewEnabled(bool enabled) override;

		// Draws handles with clipping
		void Draw() override;

		SERIALIZABLE(Vec2KeyFramesTrackControl);

	private:
		SplineTool mTool;                       // Other handles locking tool
		IEditTool* mPrevSelectedTool = nullptr; // Previous selected tool, for restore

		Actor* mTrackOwner = nullptr; // Actor which animated in track

		static Vec2KeyFramesTrackControl* mLastActive; // Last active track control of this type; When multiple track
		                                               // controls are activated, only the last one works

	private:
		void InitializeControls();
		void TryFindOwnerTrack();
		void OnSetTrack() override;
		void OnKeysChanged() override;
	};
}

CLASS_BASES_META(Editor::Vec2KeyFramesTrackControl)
{
	BASE_CLASS(Editor::KeyFramesTrackControl<AnimationTrack<Vec2F>>);
}
END_META;
CLASS_FIELDS_META(Editor::Vec2KeyFramesTrackControl)
{
	FIELD().PRIVATE().NAME(mTool);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mPrevSelectedTool);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mTrackOwner);
}
END_META;
CLASS_METHODS_META(Editor::Vec2KeyFramesTrackControl)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vec2KeyFramesTrackControl&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetActive, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCurveViewEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
	FUNCTION().PRIVATE().SIGNATURE(void, TryFindOwnerTrack);
	FUNCTION().PRIVATE().SIGNATURE(void, OnSetTrack);
	FUNCTION().PRIVATE().SIGNATURE(void, OnKeysChanged);
}
END_META;