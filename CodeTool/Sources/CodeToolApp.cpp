#include "CodeToolApp.h"

#include "Animation/Animate.h"
#include "Render/Render.h"
#include "SyntaxTree/CppSyntaxParser.h"
#include "SyntaxTree/SyntaxTree.h"
#include "UI/HorizontalProgress.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/TaskManager.h"
#include "Utils/Timer.h"

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

		if (nargs > 4)
			mNeedReset = strcmp(args[4], "-reset") == 0;
		else
			mNeedReset = false;
	}

	void CodeToolApplication::OnStarted()
	{
		Timer timer;

		LoadCache();
		UpdateCodeReflection();
		SaveCache();

		o2Debug.Log("Code generation done for %f seconds", timer.GetTime());

		Shutdown();
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

	void CodeToolApplication::LoadCache()
	{
		if (mNeedReset)
			return;

		DataNode data;
		data.LoadFromFile(mSourcesPath + "/" + mCachePath);
		mCache = data;
	}

	void CodeToolApplication::SaveCache()
	{
		DataNode data;
		data = mCache;
		data.SaveToFile(mSourcesPath + "/" + mCachePath);
	}

	void CodeToolApplication::UpdateCodeReflection()
	{
		mParser = mnew CppSyntaxParser();

		FolderInfo sourcesFolderInfo = o2FileSystem.GetFolderInfo(mSourcesPath);
		ParseSourcesFolder(sourcesFolderInfo);

		delete mParser;
	}

	void CodeToolApplication::ParseSourcesFolder(const FolderInfo& folder)
	{
		for (auto& file : folder.mFiles)
		{
			if (!file.mPath.EndsWith(".h"))
				continue;

			ParseSource(file);
		}

		for (auto& childFolder : folder.mFolders)
			ParseSourcesFolder(childFolder);
	}

	void CodeToolApplication::ParseSource(const FileInfo& file)
	{
		// check edit date
		bool isCached = false;
		for (auto& cacheFile : mCache.parseFilesInfos)
		{
			if (file.mPath == cacheFile.path)
			{
				if (file.mEditDate == cacheFile.parsedDate)
					return;

				cacheFile.parsedDate = file.mEditDate;
				isCached = true;

				break;
			}
		}

		// parse source
		SyntaxFile syntaxFile;
		mParser->ParseFile(syntaxFile, file);

		if (!isCached)
			mCache.parseFilesInfos.Add(ParseFileInfo(file.mPath, file.mEditDate));
	}

	bool ParseFileInfo::operator==(const ParseFileInfo& other) const
	{
		return path == other.path;
	}

	ParseFileInfo::ParseFileInfo()
	{}

	ParseFileInfo::ParseFileInfo(const String& path, const TimeStamp& parsedDate):
		path(path), parsedDate(parsedDate)
	{}
}