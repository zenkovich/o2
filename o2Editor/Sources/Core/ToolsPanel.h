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
	Ptr<UIWidget>           mPanelRoot;
				            
	Ptr<UIWidget>           mPlayPanel;
	Ptr<UIToggle>           mPlayToggle;
	Ptr<UIToggle>           mPauseToggle;
	Ptr<UIButton>           mStepButton;
	Ptr<UIDropDown>         mDevicesList;

	Ptr<UIHorizontalLayout> mEditToolsPanel;
	Ptr<UIToggle>           mArrowToolToggle;
	Ptr<UIToggle>           mBrushToolToggle;
	Ptr<UIToggle>           mMoveToolToggle;
	Ptr<UIToggle>           mRotateToolToggle;
	Ptr<UIToggle>           mScaleToolToggle;
	Ptr<UIToggle>           mFrameToolToggle;
				    
	Ptr<UIDropDown> mLayoutSchemesList;

protected:
	ToolsPanel();
	~ToolsPanel();

	void InitializePlayPanel();
	void InitializeLayoutSchemesPanel();
	void InitializeToolsPanel();

	friend class EditorApplication;
	friend class ITemplPtr<ToolsPanel>;
};
