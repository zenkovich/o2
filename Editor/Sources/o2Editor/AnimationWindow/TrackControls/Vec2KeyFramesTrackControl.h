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

		SERIALIZABLE(Vec2KeyFramesTrackControl);

	public:
		struct SplineWrapper: public SplineEditor::ISplineWrapper
		{
			Vec2KeyFramesTrackControl* trackControl;

		public:
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

	private:
		SplineEditor     mSplineEditor; 
		SplineSceneLayer mSceneLayer;

	private:
		void InitializeControls();
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
}
END_META;
CLASS_METHODS_META(Editor::Vec2KeyFramesTrackControl)
{

	PUBLIC_FUNCTION(void, SetActive, bool);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, OnSetTrack);
	PRIVATE_FUNCTION(void, OnKeysChanged);
}
END_META;
