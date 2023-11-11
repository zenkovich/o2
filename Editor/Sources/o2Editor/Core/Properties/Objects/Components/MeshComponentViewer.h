#pragma once

#include "o2/Scene/Components/MeshComponent.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Tools/CustomFrameTool.h"
#include "o2Editor/Core/Tools/MeshTopologyTool.h"
#include "o2Editor/Core/Tools/SplineTool.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

using namespace o2;

namespace o2
{
	class Spoiler;
}

namespace Editor
{
	// ---------------------
	// Mesh component viewer
	// ---------------------
	class MeshComponentViewer: public TObjectPropertiesViewer<MeshComponent>
	{
	public:
		// Default constructor
		MeshComponentViewer();

		// Destructor
		~MeshComponentViewer();

		// Copy operator
		MeshComponentViewer& operator=(const MeshComponentViewer& other);

		IOBJECT(MeshComponentViewer);

	protected:
		struct SceneLayer : public SceneEditorLayer
		{
			MeshComponentViewer* viewer = nullptr;
			Sprite               textureSprite;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;

			void DrawMeshWire();
		};

	protected:
		SplineTool       mSplineTool;        // Spline tool
		CustomFrameTool  mFrameTool;         // Mapping frame tool
		MeshTopologyTool mTopologyTool;      // Mesh topology tool
		SceneLayer       mFrameTetxureLayer; // Frame texture drawing layer

		IEditTool* mPrevSelectedTool = nullptr; // Previous selected tool, for restore

		Button* mFitAndCenterButton = nullptr; // Fit and centerize button

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;

		// Called when button pressed
		void FitAndCenterize();
	};
}
// --- META ---

CLASS_BASES_META(Editor::MeshComponentViewer)
{
    BASE_CLASS(Editor::TObjectPropertiesViewer<MeshComponent>);
}
END_META;
CLASS_FIELDS_META(Editor::MeshComponentViewer)
{
    FIELD().PROTECTED().NAME(mSplineTool);
    FIELD().PROTECTED().NAME(mFrameTool);
    FIELD().PROTECTED().NAME(mTopologyTool);
    FIELD().PROTECTED().NAME(mFrameTetxureLayer);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPrevSelectedTool);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mFitAndCenterButton);
}
END_META;
CLASS_METHODS_META(Editor::MeshComponentViewer)
{

    typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
    typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, _tmp2);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, FitAndCenterize);
}
END_META;
// --- END META ---
