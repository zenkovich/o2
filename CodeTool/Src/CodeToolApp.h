#pragma once

#include <windows.h>
#include "CppSyntaxParser.h"

class Timer
{
public:
	// Default constructor
	Timer();

	// Resets time
	void Reset();

	// Returns time in seconds from last Reset() call
	float GetTime();

	// Returns time in seconds from last Reset() or GetElapsedTime() call
	float GetDeltaTime();

protected:
	LONGLONG      mLastElapsedTime;
	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStartTime;
};

class CodeToolCache
{
public:
	SyntaxFilesVec   files;           // All syntax files list, including parent projects
	SyntaxFilesVec   originalFiles;   // Original syntax files list
	SyntaxSection    globalNamespace; // Global syntax namespace
	SyntaxClassesVec attributes;      // Allattribute classes
	vector<string>   parentProjects;  // Parent projects code tool caches, that used in current project

	// Updates global namespace
	void UpdateGlobalNamespace();

	// Returns is class based on other class
	bool IsClassBasedOn(SyntaxClass* _class, SyntaxClass* baseClass);

	// Returns section by name in global space
	SyntaxSection* FindSection(const string& fullName);

	// Returns section by name in where
	SyntaxSection* FindSection(const string& what, const string& where);

	// Returns section by name in where
	SyntaxSection* FindSection(const string& what, SyntaxSection* where);

	// Saves data to file
	void Save(const string& file) const;

	// Loads data from file
	void Load(const string& file, bool original = true);

protected:
	void AppendSection(SyntaxSection* currentSection, SyntaxSection* newSection);
	void ResolveDependencies(SyntaxSection* section);
	void ResolveBaseClassDependencies(SyntaxSection* section);
	SyntaxSection* FindSection(const string& what, SyntaxSection* where, SyntaxSectionsVec& processedSections);
	void SearchAttributes(SyntaxSection* section, SyntaxClass* attributeClass);
};

// ---------------------
// Code tool application
// ---------------------
class CodeToolApplication
{
public:
	// Default constructor. Initializes all editor components
	CodeToolApplication();

	// Destructor
	~CodeToolApplication();

	// Sets arguments from main()
	void SetArguments(char** args, int nargs);

	// Generates new reflection
	void Process();

	// Outs string to log
	static void Log(const char* format, ...);

	// Outs string to log if verbose move is enabled
	static void VerboseLog(const char* format, ...);

protected:
	string                 mCachePath = "CodeToolCache.xml";
					       
	string                 mSourcesPath;
	string                 mMSVCProjectPath;
	string                 mXCodeProjectPath;
	bool                   mNeedReset = true;
	static bool            mVerbose;
					       
	CppSyntaxParser*       mParser;
	vector<SyntaxFile*>    mParsedFiles;
	CodeToolCache          mCache;
	map<string, TimeStamp> mSourceFiles;

protected:
	// Returns list of all files in path and in sub paths
	map<string, TimeStamp> GetFolderFiles(const string& path);

	// Returns last edited date for file
	TimeStamp GetFileEditedDate(const string& path);

	// Parses startup arguments and puts into map
	map<string, string> ParseArguments(char** args, int nargs);

	// Returns is file exist
	bool IsFileExist(const string& path) const;

	// Writes data to file
	void WriteFile(const string& path, const string& data) const;

	// Reads data from file
	string ReadFile(const string& path) const;

	// Returns path without parent directories
	string GetPathWithoutDirectories(const string& path);

	// Returns parent path
	string GetParentPath(const string& path);

	// Returns relative path
	string GetRelativePath(const string& from, const string& to);

	// Loads files parsing cache
	void LoadCache();

	// Saves files parsing cache
	void SaveCache();

	// Updates project files structure
	void UpdateProjectFilesFilter();

	// Updates code reflection
	void UpdateCodeReflection();

	// Parses source file
	void ParseSource(const string& path, const TimeStamp& editDate);

	// Updates reflection for classes in source
	void UpdateSourceReflection(SyntaxFile* file);

	// Returns class declaration meta
	string GetClassDeclaration(SyntaxClass* cls);

	// Returns class reflection meta
	string GetClassMeta(SyntaxClass* cls);

	// Return enum reflection meta
	string GetEnumMeta(SyntaxEnum* enm);

	// Return enum reflection meta for header
	string GetEnumPreMeta(SyntaxEnum* enm);

	// Builds meta templates parameters for template classes
	void AggregateTemplates(SyntaxSection* sec, string& templates, string& fullName);

	// Returns class full name with template parameters in global space
	string GetClassNormalizedTemplates(const string& name, const string& nspace);

	// Removes class metas from source
	void RemoveMetas(string& data, const char* keyword, const char* endword, bool allowMultiline = true);

	// Returns is function reflectable
	bool IsFunctionReflectable(SyntaxFunction* function, SyntaxSection* owner) const;
};
