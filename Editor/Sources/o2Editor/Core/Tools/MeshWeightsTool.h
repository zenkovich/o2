#pragma once
#include "IEditorTool.h"

#include "o2/Render/Mesh.h"
#include "o2/Scene/Components/SkinningMeshBoneComponent.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

namespace Editor
{
	// -------------------------
	// Mesh weights editing tool
	// -------------------------
	struct MeshWeightsTool: public IEditTool
	{
		struct SceneLayer: public SceneEditorLayer
		{
			MeshWeightsTool* tool = nullptr;

		public:
			void DrawScene() override;

			void DrawMeshWire(auto& mesh);

			void Update(float dt) override;

			void UpdateBrush(float dt);

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

		Ref<SkinningMeshBoneComponent> boneComponent; // Reference to selected bone component

		SceneLayer   sceneLayer;        // Scene layer for drawing spline
		bool         isEnabled = false; // Is tool enabled now       

	public:
		// Default constructor
		MeshWeightsTool();

		// Empty copy operator
		MeshWeightsTool& operator=(const MeshWeightsTool& other) { return *this; }

		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Called when tool was enabled
		void OnEnabled() override;

		// Called when tool was disabled
		void OnDisabled() override;

		IOBJECT(MeshWeightsTool);

	private:
		Vec2F mCursosPos;            // Current cursor position, used for brush
		bool  mPressed = false;      // Is brush pressed
		float mBrushReadius = 50;    // Current brush radius
		float mBrushStrength = 1.0f; // Strength of brush

		Mesh mWeightsDebugMesh; // Mesh for drawing debug colors of weights

	private:
		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(Editor::MeshWeightsTool)
{
	BASE_CLASS(Editor::IEditTool);
}
END_META;
CLASS_FIELDS_META(Editor::MeshWeightsTool)
{
	FIELD().PUBLIC().NAME(boneComponent);
	FIELD().PUBLIC().NAME(sceneLayer);
	FIELD().PUBLIC().DEFAULT_VALUE(false).NAME(isEnabled);
	FIELD().PRIVATE().NAME(mCursosPos);
	FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mPressed);
	FIELD().PRIVATE().DEFAULT_VALUE(50).NAME(mBrushReadius);
	FIELD().PRIVATE().DEFAULT_VALUE(1.0f).NAME(mBrushStrength);
	FIELD().PRIVATE().NAME(mWeightsDebugMesh);
}
END_META;
CLASS_METHODS_META(Editor::MeshWeightsTool)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(String, GetPanelIcon);
	FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
}
END_META;
