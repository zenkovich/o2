#pragma once

#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/String.h"

using namespace o2;

namespace o2
{
	class Button;
	class CustomDropDown;
	class DropDown;
	class HorizontalLayout;
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

		// Adds tool to panel
		void AddToolToggle(Toggle* toggle);

		// Removes tool from panel
		void RemoveToolToggle(Toggle* toggle);

		// Updates panel
		void Update(float dt);

	protected:
		const String mDefaultSchemeName = "Default";
		const String mSaveAsSchemeName = "Save as ...";

		Widget* mPanelRoot = nullptr;     // Root panel widget

		Widget*   mPlayPanel = nullptr;	    // Play panel widget
		Toggle*   mPlayToggle = nullptr;	// Play toggle
		Toggle*   mPauseToggle = nullptr;	// Pause toggle
		Button*   mStepButton = nullptr;	// Step button
		DropDown* mDevicesList = nullptr;	// Devices list dropdown

		HorizontalLayout* mToolsPanel = nullptr; // Tools panel layout
		ToggleGroup       mToolsTogglesGroup;    // Group for toggles

		DropDown* mLayoutSchemesList; // Layouts schemes list

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

		// Is is called when play/stop toggle changed
		void OnPlayStopToggled(bool play);

		// Is is called when pause toggle changed
		void OnPauseToggled(bool pause);

		// It is called when step button has pressed
		void OnStepPressed();

		friend class EditorApplication;
		friend class EditorConfig;
	};

}
