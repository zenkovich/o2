#pragma once

#include "Utils/Singleton.h"
#include "Utils/Types/String.h"

using namespace o2;

namespace o2
{
	class Button;
	class CustomDropDown;
	class DropDown;
	class HorizontalLayout;
	class UIToggle;
	class Widget;
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
		Widget* GetPanelWidget() const;

		// Returns play panel's widget
		Widget* GetPlayPanel() const;

		// Returns tools' panel widget 
		HorizontalLayout* GetToolsPanel() const;

	protected:
		const String        mDefaultSchemeName = "Default";
		const String        mSaveAsSchemeName = "Save as ...";

		Widget*           mPanelRoot;			// Root panel widget

		Widget*           mPlayPanel;			// Play panel widget
		UIToggle*           mPlayToggle;		// Play toggle
		UIToggle*           mPauseToggle;		// Pause toggle
		Button*           mStepButton;		// Step button
		DropDown*         mDevicesList;		// Devices list dropdown

		HorizontalLayout* mEditToolsPanel;	// Tools panel layout
		UIToggle*           mArrowToolToggle;	// Arrow tool
		UIToggle*           mBrushToolToggle;	// Brush tool
		UIToggle*           mMoveToolToggle;	// Move tool
		UIToggle*           mRotateToolToggle;	// Rotate tool
		UIToggle*           mScaleToolToggle;	// Scale tool
		UIToggle*           mFrameToolToggle;	// Frame tool

		DropDown*         mLayoutSchemesList; // Layouts schemes list

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

		// It is called when selected scheme
		void OnSchemeSelected(const WString& name);

		friend class EditorApplication;
		friend class EditorConfig;
	};

}
