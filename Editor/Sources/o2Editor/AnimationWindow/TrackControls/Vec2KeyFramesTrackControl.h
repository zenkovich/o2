#pragma once
#include "KeyFramesTrackControl.h"
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

	public:
		struct SplineWrapper: public SplineEditor::ISplineWrapper
		{
			Vec2KeyFramesTrackControl* trackControl;

		public:
			Vec2F GetOrigin() const;

			Vec2F WorldToLocal(const Vec2F& point) const override;
			Vec2F LocalToWorld(const Vec2F& point) const override;

			int GetPointsCount() const override;

			void AddPoint(int idx, const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport) override;
			void RemovePoint(int idx) override;

			Vec2F GetPointPos(int idx) const override;
			void SetPointPos(int idx, const Vec2F& pos) override;

			Vec2F GetPointPrevSupportPos(int idx) const override;
			void SetPointPrevSupportPos(int idx, const Vec2F& pos) override;

			Vec2F GetPointNextSupportPos(int idx) const override;
			void SetPointNextSupportPos(int idx, const Vec2F& pos) override;

			Vector<Vec2F> GetDrawPoints() const override;

			const ApproximationVec2F* GetPointApproximation(int idx) const override;
			int GetPointApproximationCount(int idx) const override;

			void OnChanged() override;
		};

		struct SplineSceneLayer: public SceneEditorLayer
		{
			Vec2KeyFramesTrackControl* trackControl;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

		struct SplineTool: public IEditTool
		{
			Vec2KeyFramesTrackControl* trackControl;

		public:
			// Returns toggle in menu panel icon name
			String GetPanelIcon() const override;

			// It is called when tool was enabled
			void OnEnabled() override;

			// It is called when tool was disabled
			void OnDisabled() override;
		};

	private:
		SplineEditor     mSplineEditor; // Animation spline editor
		SplineSceneLayer mSceneLayer;   // Scene layer for drawing spline

		bool mIsEnabled = false; // Is track editing enabled

		SplineTool mTool;                       // Other handles locking tool
		IEditTool* mPrevSelectedTool = nullptr; // Previous selected tool, for restore

		Actor* mTrackOwner = nullptr; // Actor which animated in track

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
	FIELD().NAME(mSplineEditor).PRIVATE();
	FIELD().NAME(mSceneLayer).PRIVATE();
	FIELD().DEFAULT_VALUE(false).NAME(mIsEnabled).PRIVATE();
	FIELD().NAME(mTool).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrevSelectedTool).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTrackOwner).PRIVATE();
}
END_META;
CLASS_METHODS_META(Editor::Vec2KeyFramesTrackControl)
{

	PUBLIC_FUNCTION(void, SetActive, bool);
	PUBLIC_FUNCTION(void, SetCurveViewEnabled, bool);
	PUBLIC_FUNCTION(void, Draw);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, TryFindOwnerTrack);
	PRIVATE_FUNCTION(void, OnSetTrack);
	PRIVATE_FUNCTION(void, OnKeysChanged);
}
END_META;
