#pragma once

#include "Application/Application.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/IObject.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Serializable.h"
#include "Utils/TimeStamp.h"

using namespace o2;

namespace CodeTool
{
	class CppSyntaxParser;

	class ParseFileInfo: public ISerializable
	{
	public:
		TimeStamp parsedDate; // @SERIALIZABLE
		String    path;       // @SERIALIZABLE

	public:
		ParseFileInfo();
		ParseFileInfo(const String& path, const TimeStamp& parsedDate);

		bool operator==(const ParseFileInfo& other) const;

		SERIALIZABLE(ParseFileInfo);
	};
	typedef Vector<ParseFileInfo> ParseFilesInfosVec;

	class CodeToolCache: public ISerializable
	{
	public:
		ParseFilesInfosVec parseFilesInfos; // @SERIALIZABLE

		SERIALIZABLE(CodeToolCache);
	};

	// ---------------------
	// Code tool application
	// ---------------------
	class CodeToolApplication: public Application
	{
	public:
		// Default constructor. Initializes all editor components
		CodeToolApplication();

		// Destructor
		~CodeToolApplication();

		// Sets arguments from main()
		void SetArguments(char** args, int nargs);

	protected:
		String           mCachePath = "CodeToolCache.xml";

		String           mSourcesPath;
		String           mMSVCProjectPath;
		String           mXCodeProjectPath;
		bool             mNeedReset;
		
		CppSyntaxParser* mParser;
		CodeToolCache    mCache;

	protected:
		// Calling when application is starting
		void OnStarted();

		// Calling when application is closing
		void OnClosing();

		// Calling on updating
		void OnUpdate(float dt);

		// Calling on drawing
		void OnDraw();

		// Loads files parsing cache
		void LoadCache();

		// Saves files parsing cache
		void SaveCache();

		// Updates code reflection
		void UpdateCodeReflection();

		// Parses sources in folder
		void ParseSourcesFolder(const FolderInfo& folder);

		// Parses source file
		void ParseSource(const FileInfo& file);
	};
}