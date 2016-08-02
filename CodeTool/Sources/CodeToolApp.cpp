#include "CodeToolApp.h"

#include "Render/Render.h"
#include "SyntaxTree/CppSyntaxParser.h"
#include "SyntaxTree/SyntaxTree.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/HorizontalProgress.h"
#include "Utils/Debug.h"
#include "Utils/TaskManager.h"
#include "Animation/Animate.h"

namespace CodeTool
{
	CodeToolApplication::CodeToolApplication()
	{}

	CodeToolApplication::~CodeToolApplication()
	{}

	void CodeToolApplication::SetArguments(char** args, int nargs)
	{
		if (nargs > 1)
			mSourcesPath = args[1];

		if (nargs > 2)
			mMSVCProjectPath = args[2];

		if (nargs > 3)
			mXCodeProjectPath = args[3];
	}

	void CodeToolApplication::OnStarted()
	{
		SetContentSize(Vec2I(300, 150));
		SetWindowCaption("o2 Code Tool");

		UIVerticalLayout* layout = o2UI.AddVerLayout();
		layout->baseCorner = BaseCorner::Top;
		layout->expandHeight = true;
		layout->expandWidth = true;
		layout->border = RectF(10, 10, 10, 10);
		layout->layout = UIWidgetLayout::BothStretch();

		//layout->AddChild(o2UI.CreateLabel(String::Format("Code tool: %s", mSourcesPath)));
		layout->AddChild(o2UI.CreateLabel(String::Format("Code tool", mSourcesPath)));
		
		auto progress = o2UI.CreateHorProgress();
		layout->AddChild(progress);

		o2Tasks.Play(Animate(*progress).Change(&progress->value, 0.0f).Then().Change(&progress->value, 1.0f).For(1.0f).PingPong());

		mSyntaxTree = mnew SyntaxTree();
		BeginParse();
	}

	void CodeToolApplication::OnClosing()
	{}

	void CodeToolApplication::OnUpdate(float dt)
	{}

	void CodeToolApplication::OnDraw()
	{
		o2Render.Clear(Color4(192, 196, 204, 255));
		o2Render.camera = Camera::Default();
	}

	void CodeToolApplication::BeginParse()
	{
		CppSyntaxParser parser;
		parser.Parse(*mSyntaxTree, mSourcesPath);
	}

}