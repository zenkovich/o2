#pragma once

#include "Utils/Singleton.h"

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

class ToolsPanel: public Singleton<ToolsPanel>
{
public:

protected:
	UIWidget*           mPanelRoot;
				            
	UIWidget*           mPlayPanel;
	UIToggle*           mPlayToggle;
	UIToggle*           mPauseToggle;
	UIButton*           mStepButton;
	UIDropDown*         mDevicesList;

	UIHorizontalLayout* mEditToolsPanel;
	UIToggle*           mArrowToolToggle;
	UIToggle*           mBrushToolToggle;
	UIToggle*           mMoveToolToggle;
	UIToggle*           mRotateToolToggle;
	UIToggle*           mScaleToolToggle;
	UIToggle*           mFrameToolToggle;
				    
	UIDropDown*         mLayoutSchemesList;

protected:
	ToolsPanel();
	~ToolsPanel();

	void InitializePlayPanel();
	void InitializeLayoutSchemesPanel();
	void InitializeToolsPanel();

	friend class EditorApplication;
};
