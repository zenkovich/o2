#pragma once

#include "o2/Scene/Components/MeshComponent.h"
#include "o2Editor/Core/Tools/CustomFrameTool.h"
#include "o2Editor/Core/Tools/SplineTool.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorComponentViewer.h"
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
	class MeshComponentViewer: public TActorComponentViewer<MeshComponent>
	{
	public:
		// Default constructor
		MeshComponentViewer();

		// Sets target actors
		void SetTargetComponents(const Vector<Component*>& components) override;

		// Copy operator
		MeshComponentViewer& operator=(const MeshComponentViewer& other);

		IOBJECT(MeshComponentViewer);

	protected:
		SplineTool      mSplineTool; // Spline tool
		CustomFrameTool mFrameTool;  // Mapping frame tool

		IEditTool* mPrevSelectedTool = nullptr; // Previous selected tool, for restore
		bool       mIsEnabled = false;          // Is spline editing enabled

	protected:
		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;
	};
}

CLASS_BASES_META(Editor::MeshComponentViewer)
{
	BASE_CLASS(Editor::TActorComponentViewer<MeshComponent>);
}
END_META;
CLASS_FIELDS_META(Editor::MeshComponentViewer)
{
	FIELD().NAME(mSplineTool).PROTECTED();
	FIELD().NAME(mFrameTool).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrevSelectedTool).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::MeshComponentViewer)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
