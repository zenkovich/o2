#pragma once

#include "o2/Scene/Components/MeshComponent.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Tools/CustomFrameTool.h"
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

	// ---------------------------------------------------------
	// Mesh component topology tool. Adds extra points into mesh-
	// --------------------------------------------------------
	struct MeshTopologyTool: public IEditTool, public SelectableDragHandlesGroup, public CursorAreaEventsListener, public KeyboardEventsListener
	{
		struct SceneLayer: public SceneEditorLayer
		{
			MeshTopologyTool* tool = nullptr;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

		SceneLayer sceneLayer;        // Scene layer for drawing spline
		bool       isEnabled = false; // Is tool enabled now

		MeshComponent* mesh = nullptr; // Editing mesh

		Function<void()> onChanged; // Called when frame changes     

	public:
		// Default constructor
		MeshTopologyTool();

		// Destructor
		~MeshTopologyTool();

		// Setup editing mesh
		void SetMeshComponent(MeshComponent* mesh);

		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Called when tool was enabled
		void OnEnabled() override;

		// Called when tool was disabled
		void OnDisabled() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

	private:
		DragHandle          mHandleSample;
		Vector<DragHandle*> mHandles;

		Sprite mSelectionSprite;       // Selection sprite
		Vec2F  mSelectingPressedPoint; // Point, where cursor was pressed, selection starts here, in local space

		Vector<DragHandle*> mSelectingHandlesBuf; // Potentially selecting handles while selecting

		FrameHandles mTransformFrame;                      // Keys transformation frame
		bool         mTransformFrameVisible = false;       // Is transform frame visible. it visible when 2 or more main handles was selected
		Basis        mTransformFrameBasis;                 // Basis of transform frame in screen space
		Vec2F        mTransformBasisOffet = Vec2F(10, 10); // Border between side points and frame


	private:
		void InitializeHandles();
		void ClearHandles();

		void OnHandleMoved(int i, const Vec2F& pos);

		Vec2F WorldToLocal(const Vec2F& point) const;
		Vec2F LocalToWorld(const Vec2F& point) const;

		// Draw selection sprite
		void DrawSelection();

		// Draws transformation frame
		void DrawTransformFrame();

		// Updates transformation frame by selected handles
		void UpdateTransformFrame();

		// Returns is transform frame visible. it will be visible when 2 or more main handles was selected
		bool IsTransformFrameVisible() const;

		// Called when transform frame was transformed
		void OnTransformFrameTransformed(const Basis& basis);

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when cursor double clicked; creates new point
		void OnCursorDblClicked(const Input::Cursor& cursor) override;
	};

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
