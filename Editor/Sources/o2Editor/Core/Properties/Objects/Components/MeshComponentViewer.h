#pragma once

#include "o2/Scene/Components/MeshComponent.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Tools/CustomFrameTool.h"
#include "o2Editor/Core/Tools/MeshTopologyTool.h"
#include "o2Editor/Core/Tools/SplineTool.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2/Utils/Ref.h"

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
		MeshComponentViewer& operator=(const MeshComponentViewer& other) = default;

		IOBJECT(MeshComponentViewer);

	protected:
		struct SceneLayer : public SceneEditorLayer
		{
			MeshComponentViewer* viewer = nullptr;
			Sprite textureSprite;

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
		SplineTool mSplineTool;        // Spline tool
		CustomFrameTool mFrameTool;         // Mapping frame tool
		MeshTopologyTool mTopologyTool;      // Mesh topology tool
		SceneLayer mFrameTetxureLayer; // Frame texture drawing layer

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