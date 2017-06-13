#pragma once

#include "Utils/Singleton.h"
#include "Utils/String.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UICustomDropDown;
	class UIDropDown;
	class UIHorizontalLayout;
	class UIToggle;
	class UIWidget;
}

// Editor tools panel accessor macros
#define o2EditorTools ToolsPanel::Instance()

namespace Editor
{
	// ------------------
	// Editor tools panel
	// ------------------
	class ToolsPanel: public Singleton<ToolsPanel>
	{
	public:
		// Returns panel's widgets
		UIWidget* GetPanelWidget() const;

		// Returns play panel's widget
		UIWidget* GetPlayPanel() const;

		// Returns tools' panel widget 
		UIHorizontalLayout* GetToolsPanel() const;

	protected:
		const String        mDefaultSchemeName = "Default";
		const String        mSaveAsSchemeName = "Save as ...";

		UIWidget*           mPanelRoot;			// Root panel widget

		UIWidget*           mPlayPanel;			// Play panel widget
		UIToggle*           mPlayToggle;		// Play toggle
		UIToggle*           mPauseToggle;		// Pause toggle
		UIButton*           mStepButton;		// Step button
		UIDropDown*         mDevicesList;		// Devices list dropdown

		UIHorizontalLayout* mEditToolsPanel;	// Tools panel layout
		UIToggle*           mArrowToolToggle;	// Arrow tool
		UIToggle*           mBrushToolToggle;	// Brush tool
		UIToggle*           mMoveToolToggle;	// Move tool
		UIToggle*           mRotateToolToggle;	// Rotate tool
		UIToggle*           mScaleToolToggle;	// Scale tool
		UIToggle*           mFrameToolToggle;	// Frame tool

		UIDropDown*         mLayoutSchemesList; // Layouts schemes list

	protected:
		// Default constructor. Initializes all panel
		ToolsPanel();

		// Destructor
		~ToolsPanel();

		// initializes play panel
		void InitializePlayPanel();

		// initializes schemes panel
		void InitializeLayoutSchemesPanel();

		// Initializes tools panel
		void InitializeToolsPanel();

		// Updates schemes list
		void UpdateWndLayoutSchemas();

		// Calls when selected scheme
		void OnSchemeSelected(const WString& name);

		friend class EditorApplication;
		friend class EditorConfig;
	};

}
