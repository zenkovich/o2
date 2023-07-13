#pragma once
#include "IEditorTool.h"

#include "o2/Utils/Math/Spline.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

namespace Editor
{
	// ------------------------------------------------
	// Spline editing on scene tool
	// Contains scene editor layer and spline editor
	// Works around spline origin, provided from getter
	// ------------------------------------------------
	struct SplineTool: public IEditTool
	{
		struct SplineWrapper: public SplineEditor::ISplineWrapper
		{
			SplineTool* tool = nullptr;
			Spline* spline = nullptr;
			Function<Vec2F()> getOrigin;

		public:
			Vec2F GetOrigin() const;

			Vec2F WorldToLocal(const Vec2F& point) const override;
			Vec2F LocalToWorld(const Vec2F& point) const override;

			int GetPointsCount() const override;

			bool IsClosed() const override;

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
			SplineTool* tool = nullptr;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

	public:
		Function<void()> onChanged; // Called when spline was changed

	public:

		SplineEditor     splineEditor;     // Animation spline editor
		SplineSceneLayer sceneLayer;       // Scene layer for drawing spline
		bool             isEnabled = true; // Is tool enabled now

	public:
		// Default constructor
		SplineTool();

		// Empty copy operator
		SplineTool& operator=(const SplineTool& other) { return *this; }

		// Sets editing spline
		void SetSpline(Spline* spline, const Function<Vec2F()>& getOrigin);

		// Resets spline and callbacks
		void Reset();

		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Called when tool was enabled
		void OnEnabled() override;

		// Called when tool was disabled
		void OnDisabled() override;

		IOBJECT(SplineTool);
	};
}
// --- META ---

CLASS_BASES_META(Editor::SplineTool)
{
	BASE_CLASS(Editor::IEditTool);
}
END_META;
CLASS_FIELDS_META(Editor::SplineTool)
{
	FIELD().PUBLIC().NAME(onChanged);
	FIELD().PUBLIC().NAME(splineEditor);
	FIELD().PUBLIC().NAME(sceneLayer);
	FIELD().PUBLIC().DEFAULT_VALUE(true).NAME(isEnabled);
}
END_META;
CLASS_METHODS_META(Editor::SplineTool)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetSpline, Spline*, const Function<Vec2F()>&);
	FUNCTION().PUBLIC().SIGNATURE(void, Reset);
	FUNCTION().PUBLIC().SIGNATURE(String, GetPanelIcon);
	FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
}
END_META;
// --- END META ---
