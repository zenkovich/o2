#pragma once
#include "IEditorTool.h"

#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2/Utils/Editor/FrameHandles.h"

namespace Editor
{
	// --------------------------------------------------------
	// Custom frame tool. Used for special frame editing things
	// --------------------------------------------------------
	struct CustomFrameTool: public IEditTool
	{
		struct SceneLayer: public SceneEditorLayer
		{
			CustomFrameTool* tool = nullptr;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

		FrameHandles frameHandles;     // Frame handles 
		SceneLayer   sceneLayer;       // Scene layer for drawing spline
		bool         isEnabled = true; // Is tool enabled now

		Function<void(const Basis&)> onChanged; // Called when frame changes

	public:
		// Default constructor
		CustomFrameTool();

		// Sets frame
		void SetFrame(const Basis& frame);

		// Returns current frame
		const Basis& GetFrame() const;

		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// It is called when tool was enabled
		void OnEnabled() override;

		// It is called when tool was disabled
		void OnDisabled() override;

	private:
		Basis mBasis; // Current editing basis
	};
}
