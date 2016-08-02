#include "CodeToolApp.h"

#include "Render/Render.h"
#include "SyntaxTree/CppSyntaxParser.h"
#include "SyntaxTree/SyntaxTree.h"
#include "Utils/Debug.h"

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
		SetContentSize(Vec2I(300, 50));
		SetWindowCaption("o2 Code Tool");

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