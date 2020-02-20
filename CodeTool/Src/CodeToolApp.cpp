#include "CodeToolApp.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional> 
#include <iostream>
#include <locale>
#include <sstream>
#include <shlwapi.h>
#include <windows.h>
#include <iostream>

#undef GetClassName

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mStartTime);
	mLastElapsedTime = mStartTime.QuadPart;
}

float Timer::GetTime()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)((double)(curTime.QuadPart - (double)mStartTime.QuadPart)/(double)mFrequency.QuadPart);
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

float Timer::GetDeltaTime()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)((double)(curTime.QuadPart - (double)mLastElapsedTime)/(double)mFrequency.QuadPart);
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

CodeToolApplication::CodeToolApplication()
{}

CodeToolApplication::~CodeToolApplication()
{}

void CodeToolApplication::SetArguments(char** args, int nargs)
{
	auto argsMap = ParseArguments(args, nargs);

	mSourcesPath = argsMap["sources"];
	mMSVCProjectPath = argsMap["msvs_project"];
	mXCodeProjectPath = argsMap["xcode_project"];
	mNeedReset = argsMap.find("reset") != argsMap.end() || argsMap.find("r") != argsMap.end();
	mVerbose = argsMap.find("verbose") != argsMap.end() || argsMap.find("v") != argsMap.end();

	mCache.parentProjects = Split(argsMap["parent_projects"], ' ');
}

void CodeToolApplication::Process()
{
	Timer t;

	LoadCache();
	UpdateCodeReflection();
	SaveCache();

	UpdateProjectFilesFilter();

	Log("Code reflection generated for %.3f seconds", t.GetDeltaTime());
}

bool CodeToolApplication::mVerbose = true;

void CodeToolApplication::Log(const char* format, ...)
{
	va_list vlist;
	va_start(vlist, format);

	vprintf(format, vlist);

	va_end(vlist);
}

void CodeToolApplication::VerboseLog(const char* format, ...)
{
	if (!mVerbose)
		return;

	va_list vlist;
	va_start(vlist, format);

	vprintf(format, vlist);

	va_end(vlist);
}

map<string, TimeStamp> CodeToolApplication::GetFolderFiles(const string& path)
{
	map<string, TimeStamp> res;

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile((path + "/*").c_str(), &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
				continue;

			if (f.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				auto subFolderFiles = GetFolderFiles(path + "/" + f.cFileName);
				for (auto x : subFolderFiles)
					res[x.first] = x.second;
			}
			else
			{
				string filePath = path + "/" + f.cFileName;
				res[filePath] = GetFileEditedDate(filePath);
			}
		}
		while (FindNextFile(h, &f));
	}

	FindClose(h);

	return res;
}

TimeStamp CodeToolApplication::GetFileEditedDate(const string& path)
{
	FILETIME creationTime, lastAccessTime, lastWriteTime;
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
							   FILE_FLAG_OVERLAPPED, NULL);

	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
		return TimeStamp();

	if (!GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
	{
		CloseHandle(hFile);
		return TimeStamp();
	}

	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&lastWriteTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	TimeStamp res(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth,
				  stLocal.wYear);

	CloseHandle(hFile);

	return res;
}

map<string, string> CodeToolApplication::ParseArguments(char** args, int nargs)
{
	map<string, string> res;
	string lastArgName = "unnamed";
	for (int i = 0; i < nargs; i++)
	{
		if (args[i][0] == '-')
		{
			lastArgName = args[i] + 1;
			res[lastArgName] = "";
		}
		else
		{
			if (res[lastArgName].empty())
				res[lastArgName] = args[i];
			else
				res[lastArgName] += string(" ") + args[i];
		}
	}

	return res;
}

void CodeToolApplication::WriteFile(const string& path, const string& data) const
{
	ofstream fout;
	fout.open(path.c_str());
	if (!fout.is_open())
		return;
	
	fout.write(data.c_str(), data.length());

	fout.close();
}

string CodeToolApplication::ReadFile(const string& path) const
{
	ifstream fin;
	fin.open(path.c_str());
	if (!fin.is_open())
		return string();

	string res = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

	fin.close();

	return res;
}

bool CodeToolApplication::IsFileExist(const string& path) const
{
	DWORD tp = GetFileAttributes(path.c_str());

	if (tp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (tp & FILE_ATTRIBUTE_DIRECTORY)
		return false;

	return true;
}

string CodeToolApplication::GetPathWithoutDirectories(const string& path)
{
	return path.substr(max(path.rfind('/'), path.rfind('\\')) + 1);
}

string CodeToolApplication::GetParentPath(const string& path)
{
	auto a = path.rfind('/');
	auto b = path.rfind("\\");

	int _a = a == string::npos ? -1 : (int)a;
	int _b = b == string::npos ? -1 : (int)b;

	int idx = max(_a, _b);

	if (idx < 0)
		return string();

	return path.substr(0, idx);
}

string CodeToolApplication::GetRelativePath(const string& from, const string& to)
{
	char out[MAX_PATH];
	PathRelativePathTo(out, from.c_str(), FILE_ATTRIBUTE_DIRECTORY, to.c_str(), FILE_ATTRIBUTE_NORMAL);
	return (string)out;
}

void CodeToolApplication::LoadCache()
{
	if (mNeedReset)
	{
		for (auto& file : mCache.parentProjects)
			mCache.Load(file, false);

		return;
	}

	mCache.Load(mSourcesPath + "/" + mCachePath);
}

void CodeToolApplication::SaveCache()
{
	mCache.Save(mSourcesPath + "/" + mCachePath);
}


void CodeToolApplication::UpdateProjectFilesFilter()
{
	if (mMSVCProjectPath.empty())
		return;

	pugi::xml_document doc;
	doc.load_file((mMSVCProjectPath + ".filters").c_str());

	pugi::xml_node projectNode = doc.child("Project");
	if (!projectNode)
		return;

	string MSVCProjectDir = GetParentPath(mMSVCProjectPath);

	vector<string> filters;
	vector<string> files;
	for (auto file : mSourceFiles)
	{
		string filePath = file.first;

		if (!EndsWith(filePath, ".h") && !EndsWith(filePath, ".cpp"))
			continue;

		for (int i = 0; i < filePath.length(); i++)
		{
			if (filePath[i] == '/')
				filePath[i] = '\\';
		}

		string dir = GetRelativePath(MSVCProjectDir, GetParentPath(filePath));

		while (!dir.empty() && dir[0] == '.')
		{
			auto slashPos = dir.find('\\');
			if (slashPos != string::npos)
				dir.erase(0, slashPos + 1);
		}

		if (dir.find("OSX") != string::npos)
			continue;

		if (dir.find("Android") != string::npos)
			continue;

		while (!dir.empty() && find(filters.begin(), filters.end(), dir) == filters.end())
		{
			filters.push_back(dir);
			dir = GetParentPath(dir);
		}

		string relativePath = GetRelativePath(MSVCProjectDir, filePath);

		if (relativePath.length() > 1 && relativePath[0] == '.' && relativePath[1] == '\\')
			relativePath.erase(0, 2);

		files.push_back(relativePath);
	}

	vector<pugi::xml_node> oldFiltersNodes, ignoringGroups;
	vector<string> oldFilters, oldFiles, newFilters, newFiles;

	bool projectStructureChanged = false;
	for (auto groupNode : projectNode)
	{
		for (auto itemNode : groupNode)
		{
			if ((string)"Filter" == itemNode.name())
			{
				string filter = itemNode.attribute("Include").as_string();
				if (find(filters.begin(), filters.end(), filter) != filters.end())
				{
					oldFiltersNodes.push_back(itemNode);
					oldFilters.push_back(filter);
				}
				else
				{
					VerboseLog("Project changed: %s - removed filter\n", filter.c_str());
					projectStructureChanged = true;
				}
			}
			else if ((string)"ClInclude" == itemNode.name() || (string)"ClCompile" == itemNode.name())
			{
				string file = itemNode.attribute("Include").as_string();
				if (find(files.begin(), files.end(), file) != files.end())
					oldFiles.push_back(file);
				else
				{
					VerboseLog("Project changed: %s - removed file\n", file.c_str());
					projectStructureChanged = true;
				}
			}
			else
			{
				ignoringGroups.push_back(groupNode);
				break;
			}
		}
	}

	for (auto& filter : filters)
		if (find(oldFilters.begin(), oldFilters.end(), filter) == oldFilters.end())
			newFilters.push_back(filter);

	for (auto& file : files)
		if (find(oldFiles.begin(), oldFiles.end(), file) == oldFiles.end())
			newFiles.push_back(file);

	projectStructureChanged = projectStructureChanged || !newFiles.empty() || !newFilters.empty() || mNeedReset;

	if (!projectStructureChanged)
	{
		VerboseLog("Project wasn't changed\n");
		return;
	}

	VerboseLog("Project changed. New files:%i, new filters:%i, need reset:%s\n", newFiles.size(), newFilters.size(),
			   (mNeedReset ? "true":"false"));

	// generate new filters file
	pugi::xml_document newDoc;
	pugi::xml_node newProjectNode = newDoc.append_child("Project");
	newProjectNode.append_attribute("ToolsVersion") = projectNode.attribute("ToolsVersion").as_string();
	newProjectNode.append_attribute("xmlns") = projectNode.attribute("xmlns").as_string();

	// filters
	pugi::xml_node filtersNode = newProjectNode.append_child("ItemGroup");
	for (auto& oldFilterNode : oldFiltersNodes)
		filtersNode.append_copy(oldFilterNode);

	for (auto& newFilter : newFilters)
		filtersNode.append_child("Filter").append_attribute("Include") = newFilter.c_str();

	// includes
	pugi::xml_node headersNode = newProjectNode.append_child("ItemGroup");
	for (auto& file : files)
	{
		if (EndsWith(file, ".h"))
		{
			auto node = headersNode.append_child("ClInclude");
			node.append_attribute("Include") = file.c_str();

			auto slashPos = file.find('\\');
			if (slashPos != string::npos)
			{
				while (file[slashPos + 1] == '.')
					slashPos = file.find('\\', slashPos + 1);

				string filter = GetParentPath(file.substr(slashPos + 1));
				if (!filter.empty())
					node.append_child("Filter").append_child(pugi::node_pcdata).set_value(filter.c_str());
			}
		}
	}

	// sources
	pugi::xml_node sourcesNode = newProjectNode.append_child("ItemGroup");
	for (auto& file : files)
	{
		if (EndsWith(file, ".cpp"))
		{
			auto node = sourcesNode.append_child("ClCompile");
			node.append_attribute("Include") = file.c_str();

			auto slashPos = file.find('\\');
			if (slashPos != string::npos)
			{
				while (file[slashPos + 1] == '.')
					slashPos = file.find('\\', slashPos + 1);

				string filter = GetParentPath(file.substr(slashPos + 1));
				if (!filter.empty())
					node.append_child("Filter").append_child(pugi::node_pcdata).set_value(filter.c_str());
			}
		}
	}

	// ignored
	for (auto& x : ignoringGroups)
		newProjectNode.append_copy(x);

	newDoc.save_file((mMSVCProjectPath + ".filters").c_str());

	//and update project
	pugi::xml_document projectDoc, newProjectDoc;
	projectDoc.load_file(mMSVCProjectPath.c_str());

	projectNode = projectDoc.child("Project");
	newProjectNode = newProjectDoc.append_child("Project");
	newProjectNode.append_attribute("DefaultTargets") = projectNode.attribute("DefaultTargets").as_string();
	newProjectNode.append_attribute("ToolsVersion") = projectNode.attribute("ToolsVersion").as_string();
	newProjectNode.append_attribute("xmlns") = projectNode.attribute("xmlns").as_string();

	for (auto node : projectNode)
	{
		if ((string)"ItemGroup" != node.name() || node.attribute("Label") || (!node.child("ClInclude") && !node.child("ClCompile")))
			newProjectNode.append_copy(node);
	}

	// headers
	pugi::xml_node newProjectHeadersGroup = newProjectNode.append_child("ItemGroup");
	for (auto& file : files)
	{
		if (EndsWith(file, ".h"))
			newProjectHeadersGroup.append_child("ClInclude").append_attribute("Include") = file.c_str();
	}

	// sources
	pugi::xml_node newProjectSourcesGroup = newProjectNode.append_child("ItemGroup");
	for (auto& file : files)
	{
		if (EndsWith(file, ".cpp"))
		{
			auto clCompile = newProjectSourcesGroup.append_child("ClCompile");
			clCompile.append_attribute("Include") = file.c_str();

			if (EndsWith(file, "stdafx.cpp"))
			{
				auto headerDebug = clCompile.append_child("PrecompiledHeader");
				headerDebug.append_child(pugi::node_pcdata).set_value("Create");

				auto headerRelease = clCompile.append_child("PrecompiledHeader");
				headerRelease.append_child(pugi::node_pcdata).set_value("Create");
			}
		}
	}

	newProjectDoc.save_file(mMSVCProjectPath.c_str());
}

void CodeToolApplication::UpdateCodeReflection()
{
	mParser = new CppSyntaxParser();

	// get all files in sources path
	mSourceFiles = GetFolderFiles(mSourcesPath);

	// parse all headers
	for (auto fileInfo : mSourceFiles)
	{
		if (!EndsWith(fileInfo.first, ".h"))
			continue;

		ParseSource(fileInfo.first, fileInfo.second);
	}

	// remove old sources from cache
	for (auto parseFileInfo = mCache.originalFiles.begin(); parseFileInfo != mCache.originalFiles.end();)
	{
		auto fnd = find_if(mSourceFiles.begin(), mSourceFiles.end(), [&](auto x) { return x.first == (*parseFileInfo)->GetPath(); });
		if (fnd == mSourceFiles.end())
		{
			delete *parseFileInfo;
			parseFileInfo = mCache.originalFiles.erase(parseFileInfo);
			mCache.files.erase(find(mCache.files.begin(), mCache.files.end(), *parseFileInfo));
		}
		else ++parseFileInfo;
	}

	mCache.UpdateGlobalNamespace();

	// update reflection
	for (auto file : mParsedFiles)
		UpdateSourceReflection(file);

	delete mParser;
}

void CodeToolApplication::ParseSource(const string& path, const TimeStamp& editDate)
{
	// check edit date
	for (auto cacheFile : mCache.originalFiles)
	{
		if (path == cacheFile->GetPath())
		{
			if (editDate == cacheFile->GetLastEditedDate())
				return;

			delete cacheFile;
			mCache.originalFiles.erase(find(mCache.originalFiles.begin(), mCache.originalFiles.end(), cacheFile));
			mCache.files.erase(find(mCache.files.begin(), mCache.files.end(), cacheFile));

			break;
		}
	}

	// parse source
	SyntaxFile* syntaxFile = new SyntaxFile();
	mParser->ParseFile(*syntaxFile, path, editDate);
	mParsedFiles.push_back(syntaxFile);

	mCache.originalFiles.push_back(syntaxFile);
	mCache.files.push_back(syntaxFile);

	VerboseLog("Parsed %s\n", path.c_str());
}

void CodeToolApplication::UpdateSourceReflection(SyntaxFile* file)
{
	string cppSource, cppSourceInitial;
	bool cppLoaded = false;

	string hSource = file->GetData();

	if (hSource.find("@CODETOOLIGNORE") != string::npos)
		return;

	RemoveMetas(hSource, "META_TEMPLATES(", "END_META;");
	RemoveMetas(hSource, "CLASS_BASES_META(", "END_META;");
	RemoveMetas(hSource, "CLASS_FIELDS_META(", "END_META;");
	RemoveMetas(hSource, "CLASS_METHODS_META(", "END_META;");
	RemoveMetas(hSource, "DECLARE_CLASS(", ");", false);
	RemoveMetas(hSource, "PRE_ENUM_META(", ");", false);

	string cppSourcePath = file->GetPath().substr(0, file->GetPath().rfind('.')) + ".cpp";

	SyntaxClass* baseObjectClass = (SyntaxClass*)(mCache.FindSection("o2::IObject"));

	auto checkCppLoad = [&]()
	{
		if (!cppLoaded)
		{
			if (IsFileExist(cppSourcePath))
			{
				cppSource = ReadFile(cppSourcePath);
				cppSourceInitial = cppSource;
				RemoveMetas(cppSource, "ENUM_META(", "END_ENUM_META;");
				RemoveMetas(cppSource, "ENUM_META_(", "END_ENUM_META;");
				RemoveMetas(cppSource, "CLASS_META(", "END_META;");
				RemoveMetas(cppSource, "CLASS_TEMPLATE_META(", "END_META;");
				RemoveMetas(cppSource, "CLASS_BASES_META(", "END_META;");
				RemoveMetas(cppSource, "CLASS_FIELDS_META(", "END_META;");
				RemoveMetas(cppSource, "CLASS_METHODS_META(", "END_META;");
				RemoveMetas(cppSource, "DECLARE_CLASS(", ");", false);
				RemoveMetas(cppSource, "PRE_ENUM_META(", ");", false);
			}
			else cppSource = "#include \"" + GetPathWithoutDirectories(file->GetPath()) + "\"\n\n";

			cppLoaded = true;
		}
	};

	// Enums
	SyntaxEnumsVec allEnums = file->GetGlobalNamespace()->GetAllEnums();
	SyntaxEnumsVec metaEnums;

	for (auto enm : allEnums)
	{
		auto owner = enm->GetOwnerSyntaxSection();
		if (owner && owner->IsClass() && ((SyntaxClass*)owner)->IsTemplate())
			continue;

		metaEnums.push_back(enm);
	}

	if (!metaEnums.empty())
		checkCppLoad();

	for (auto enm : metaEnums)
	{
		cppSource += GetEnumMeta(enm);
		hSource += GetEnumPreMeta(enm);
		VerboseLog("Generated meta for enum:%s\n", enm->GetFullName().c_str());
	}

	// Classes
	auto classes = file->GetGlobalNamespace()->GetAllClasses();
	for (auto cls : classes)
	{
		bool hasIObject = std::find_if(cls->GetFunctions().begin(), cls->GetFunctions().end(), 
									   [](SyntaxFunction* x) { 
			return x->GetName() == "IOBJECT" || x->GetName() == "SERIALIZABLE" || x->GetName() == "ASSET_TYPE";
		}) != cls->GetFunctions().end();

		if ((!mCache.IsClassBasedOn(cls, baseObjectClass) && !cls->IsMetaClass()) || !hasIObject || cls == baseObjectClass)
			continue;

		if (!cls->IsTemplate())
		{
			checkCppLoad();
			cppSource += GetClassDeclaration(cls);
		}
		
		hSource += GetClassMeta(cls);

		VerboseLog("Generated meta for class:%s\n", cls->GetFullName().c_str());
	}

	// Write
	if (cppLoaded && cppSource != cppSourceInitial)
		WriteFile(cppSourcePath, cppSource);

	if (hSource != file->GetData())
	{
		WriteFile(file->GetPath(), hSource);
		file->mLastEditedDate = GetFileEditedDate(file->GetPath());
	}

	VerboseLog("Reflection generated for %s\n", file->GetPath().c_str());
}

string CodeToolApplication::GetClassDeclaration(SyntaxClass* cls)
{
	string res = "\n";

	string nspace;
	int nspaceDelimer = (int)cls->GetFullName().rfind("::");
	if (nspaceDelimer != cls->GetFullName().npos)
		nspace = cls->GetFullName().substr(0, nspaceDelimer);

	res += "DECLARE_CLASS(" + GetClassNormalizedTemplates(cls->GetFullName(), nspace) + ");\n";

	return res;
}

string CodeToolApplication::GetClassMeta(SyntaxClass* cls)
{
	string res = "\n";
	res.reserve(cls->GetData().length()*2);

	string nspace;
	int nspaceDelimer = (int)cls->GetFullName().rfind("::");
	if (nspaceDelimer != cls->GetFullName().npos)
		nspace = cls->GetFullName().substr(0, nspaceDelimer);

	string classDef;
	string templates;

	if (!cls->IsTemplate())
		classDef = GetClassNormalizedTemplates(cls->GetFullName(), nspace);
	else
		AggregateTemplates(cls, templates, classDef);

	// base classes
	res += templates;
	res += "CLASS_BASES_META(" + classDef + ")\n{\n";

	int typedefs = 0;
	for (auto x : cls->GetBaseClasses())
	{
		auto classInfo = mCache.FindSection(x.GetClassName(), nspace);
		auto className = classInfo ? classInfo->GetFullName() : x.GetClassName();
		if (className.find(',') != string::npos)
		{
			typedefs++;
			char buf[256];
			_itoa(typedefs, buf, 10);

			auto newClassName = string("_tmp") + buf;
			res += string("\ttypedef ") + className + ' ' + newClassName + ";\n";
			className = newClassName;
		}
		
		res += string("\tBASE_CLASS(") + className +  +");\n";
	}
	res += "}\nEND_META;\n";

	// fields
	res += templates;
	res += "CLASS_FIELDS_META(" + classDef + ")\n{\n";

	for (auto x : cls->GetFunctions())
	{
		if (x->GetName() == "PROPERTY" || x->GetName() == "SETTER" || x->GetName() == "GETTER" || x->GetName() == "ACCESSOR")
		{
			if (x->GetParameters().size() > 2)
			{
				if (x->GetClassSection() == SyntaxProtectionSection::Public)
					res += "\tPUBLIC_FIELD(" + x->GetParameters()[2]->GetVariableType().GetName() + ");\n";
				else if (x->GetClassSection() == SyntaxProtectionSection::Private)
					res += "\tPRIVATE_FIELD(" + x->GetParameters()[2]->GetVariableType().GetName() + ");\n";
				else if (x->GetClassSection() == SyntaxProtectionSection::Protected)
					res += "\tPROTECTED_FIELD(" + x->GetParameters()[2]->GetVariableType().GetName() + ");\n";
			}
		}
	}

	for (auto x : cls->GetVariables())
	{
		if (x->IsStatic())
			continue;

		if (x->GetClassSection() == SyntaxProtectionSection::Public)
			res += "\tPUBLIC_FIELD(" + x->GetName() + ")";
		else if (x->GetClassSection() == SyntaxProtectionSection::Private)
			res += "\tPRIVATE_FIELD(" + x->GetName() + ")";
		else if (x->GetClassSection() == SyntaxProtectionSection::Protected)
			res += "\tPROTECTED_FIELD(" + x->GetName() + ")";

		// attributes
		string attributes = "";

		// try attributes section
		SyntaxAttributes* synAttributes = nullptr;
		for (auto attr : cls->GetAttributes())
		{
			if (attr->GetLine() == x->GetLine() - 1)
			{
				synAttributes = attr;
				break;
			}
		}

		if (synAttributes)
		{
			for (auto& attributeEntry : synAttributes->GetAttributesList())
			{
				SyntaxClass* attributeClass = dynamic_cast<SyntaxClass*>(mCache.FindSection(attributeEntry, cls));
				if (attributeClass)
				{
					if (!attributeClass->GetAttributeShortDef().empty())
						attributes += string(".") + attributeClass->GetAttributeShortDef();
					else
						attributes += string(".ATTRIBUTE(") + attributeClass->GetFullName() + ")";
				}
				else attributes += string(".ATTRIBUTE(") + attributeEntry + ")";
			}
		}

		// try search comment
		SyntaxComment* synComment = nullptr;
		for (auto comment : cls->GetComments())
		{
			// comment is on same line as variable
			if (comment->GetLine() == x->GetLine())
			{
				synComment = comment;
				break;
			}

			// comment is on up line to variable
			if (comment->GetLine() == x->GetLine() - 1)
			{
				// check other variable on this line
				bool success = true;
				for (auto v : cls->GetVariables())
				{
					if (v->GetLine() == comment->GetLine())
					{
						success = false;
						break;
					}
				}

				if (success)
				{
					synComment = comment;
					break;
				}
			}
		}

		if (synComment)
		{
			for (auto attributeClass : mCache.attributes)
			{
				auto fnd = synComment->GetData().find(attributeClass->GetAttributeCommentDef());
				if (!attributeClass->GetAttributeCommentDef().empty() && fnd != string::npos)
				{
					string parameters = "()";
					auto parametersBegin = fnd + attributeClass->GetAttributeCommentDef().length();
					if (synComment->GetData()[parametersBegin] == '(')
					{
						auto parametersEnd = synComment->GetData().find(')', parametersBegin) + 1;
						parameters = synComment->GetData().substr(parametersBegin, parametersEnd - parametersBegin);
					}

					if (!attributeClass->GetAttributeShortDef().empty())
						attributes += string(".") + attributeClass->GetAttributeShortDef() + parameters;
					else
						attributes += string(".ATTRIBUTE(") + attributeClass->GetFullName() + parameters + ")";
				}
			}
		}

		res += attributes + ";\n";
	}
	res += "}\nEND_META;\n";

	// functions
	res += templates;
	res += "CLASS_METHODS_META(" + classDef + ")\n{\n";

	int supportingTypedefsPos = (int)res.length();
	vector<string> supportingTypedefs;

	bool firstFunction = true;
	for (auto x : cls->GetFunctions())
	{
		if (!IsFunctionReflectable(x, cls))
			continue;

		if (firstFunction)
		{
			firstFunction = false;
			res += "\n";
		}

		if (x->IsStatic())
		{
			if (x->GetClassSection() == SyntaxProtectionSection::Public)
				res += "\tPUBLIC_STATIC_FUNCTION(";
			else if (x->GetClassSection() == SyntaxProtectionSection::Private)
				res += "\tPRIVATE_STATIC_FUNCTION(";
			else if (x->GetClassSection() == SyntaxProtectionSection::Protected)
				res += "\tPROTECTED_STATIC_FUNCTION(";
		}
		else
		{
			if (x->GetClassSection() == SyntaxProtectionSection::Public)
				res += "\tPUBLIC_FUNCTION(";
			else if (x->GetClassSection() == SyntaxProtectionSection::Private)
				res += "\tPRIVATE_FUNCTION(";
			else if (x->GetClassSection() == SyntaxProtectionSection::Protected)
				res += "\tPROTECTED_FUNCTION(";
		}

		auto returnTypeName = (x->GetReturnType().IsConstant() ? "const " : "") + x->GetReturnType().GetName();

		if (returnTypeName.find(',') != returnTypeName.npos)
		{
			char buf[255];
			supportingTypedefs.push_back(returnTypeName);
			returnTypeName = (string)"_tmp" + _itoa((int)supportingTypedefs.size(), buf, 10);
		}

		res += returnTypeName;
		res += string(", ") + x->GetName();

		for (auto y : x->GetParameters())
		{
			string parameterName = (y->GetVariableType().IsConstant() ? "const " : "") + y->GetVariableType().GetName();

			if (parameterName.find(',') != parameterName.npos)
			{
				supportingTypedefs.push_back(parameterName);
				char buf[255];
				parameterName = string("_tmp") + _itoa((int)supportingTypedefs.size(), buf, 10);
			}

			res += string(", ") + parameterName;
		}

		res += ");\n";
	}

	// supporting typedefs
	if (!supportingTypedefs.empty())
	{
		char buf[255];
		string supportingTypedefsStr = "\n";
		for (int i = 0; i < supportingTypedefs.size(); i++)
			supportingTypedefsStr += (string)"\ttypedef " + supportingTypedefs[i] + " _tmp" + _itoa(i + 1, buf, 10) + ";\n";

		res.insert(supportingTypedefsPos, supportingTypedefsStr);
	}

	res += "}\nEND_META;\n";

	return res;
}

string CodeToolApplication::GetEnumMeta(SyntaxEnum* enm)
{
	string res;
	res.reserve(enm->GetEntries().size()*15);

	res += "\nENUM_META(" + enm->GetFullName() + ")\n{\n";

	for (auto e : enm->GetEntries())
		res += "\tENUM_ENTRY(" + e.first + ");\n";

	res += "}\nEND_ENUM_META;\n";

	return res;
}

string CodeToolApplication::GetEnumPreMeta(SyntaxEnum* enm)
{
	string res = "\nPRE_ENUM_META(" + enm->GetFullName() + ");\n";
	return res;
}

void RemoveSubstrs(string& s, string& p)
{
	string::size_type n = p.length();
	for (string::size_type i = s.find(p); i != string::npos; i = s.find(p))
		s.erase(i, n);
}

void CodeToolApplication::AggregateTemplates(SyntaxSection* sec, string& templates, string& fullName)
{
	if (sec->GetParentSection())
		AggregateTemplates(sec->GetParentSection(), templates, fullName);

	if (fullName.empty())
		fullName = sec->GetName();
	else
		fullName += "::" + sec->GetName();

	if (sec->IsClass())
	{
		SyntaxClass* cls = dynamic_cast<SyntaxClass*>(sec);
		if (!cls->GetTemplateParameters().empty())
		{
			templates += "META_TEMPLATES(" + cls->GetTemplateParameters() + ")\n";

			string classTemplates = cls->GetTemplateParameters();
			RemoveSubstrs(classTemplates, (std::string)"typename ");

			fullName += "<" + classTemplates + ">";
		}
	}
}

string CodeToolApplication::GetClassNormalizedTemplates(const string& name, const string& nspace)
{
	string fullName;
	int nameLenght = (int)name.length();
	int fnd = 0;
	int lastFnd = fnd;
	while (fnd >= 0)
	{
		lastFnd = fnd;
		fnd = (int)name.find('<', fnd);

		if (fnd == name.npos)
			break;

		int begin = fnd + 1;
		int braces = 0, trBraces = 0, sqBraces = 0;
		for (bool stop = false; !stop && fnd < nameLenght; fnd++)
		{
			switch (name[fnd])
			{
			case '(': braces++; break;
			case ')': braces--; break;
			case '[': sqBraces++; break;
			case ']': sqBraces--; break;
			case '<': trBraces++; break;
			case '>':
			trBraces--;
			if (trBraces == 0 && braces == 0 && sqBraces == 0)
			{
				stop = true;
				fnd--;
			}
			break;
			}
		}

		string templateParamsStr = name.substr(begin, fnd - begin);
		vector<string> templateParams = Split(templateParamsStr, ',');

		fullName += name.substr(lastFnd, begin - lastFnd);
		bool firstParam = true;
		for (auto& templateParam : templateParams)
		{
			Trim(templateParam);

			if (StartsWith(templateParam, "typename "))
				templateParam.erase(0, strlen("typename "));

			if (!firstParam)
				fullName += ", ";
			else
				firstParam = false;

			auto classInfo = mCache.FindSection(templateParam, nspace);
			fullName += classInfo ? classInfo->GetFullName() : templateParam;
		}
	}

	fullName += name.substr(lastFnd);
	return fullName;
}

void CodeToolApplication::RemoveMetas(string& data, const char* keyword, const char* endword, 
									  bool allowMultiline /*= true*/)
{
	auto isSkipingChar = [](char x) { return x == '\n' || x == '\r' || x == '\t' || x == '\0' || x == ' '; };

	auto caret = data.find(keyword);
	while (caret != string::npos)
	{
		auto end = data.find(endword, caret);
		if (end == string::npos)
			break;

		while (caret > 0 && isSkipingChar(data[caret - 1]))
			caret--;

		if (caret > 0 && isSkipingChar(data[caret]))
			caret--;

		if (!allowMultiline)
		{
			auto newLinePos = data.find("\n", caret + strlen(keyword));
			if (newLinePos != string::npos && newLinePos < end)
				return;
		}

		data.erase(caret + 1, end + strlen(endword) - caret - 1);
		caret = data.find(keyword);
	}

	caret = data.length();
	while (caret > 0 && isSkipingChar(data[caret - 1]))
		caret--;

	data.erase(caret);
	data += '\n';
}

bool CodeToolApplication::IsFunctionReflectable(SyntaxFunction* function, SyntaxSection* owner) const
{
	static vector<string> ignoringNames ={ "SERIALIZABLE", "PROPERTY", "GETTER", "SETTER", "IOBJECT", "ASSET_TYPE", "ATTRIBUTE_COMMENT_DEFINITION", "ATTRIBUTE_SHORT_DEFINITION" };

	return !StartsWith(owner->GetName(), function->GetName()) &&
		!StartsWith(function->GetName(), string("~") + owner->GetName()) &&
		function->GetName().find('~') == string::npos &&
		function->GetName().find("operator") == function->GetName().npos &&
		!function->IsTemplate() &&
		find(ignoringNames.begin(), ignoringNames.end(), function->GetName()) == ignoringNames.end();
}

void CodeToolCache::UpdateGlobalNamespace()
{
	for (auto file : files)
	{
		SyntaxSection* fileGlobalNamespace = file->GetGlobalNamespace();

		globalNamespace.mVariables.insert(globalNamespace.mVariables.end(),
										  fileGlobalNamespace->mVariables.begin(),
										  fileGlobalNamespace->mVariables.end());

		globalNamespace.mFunctions.insert(globalNamespace.mFunctions.end(),
										  fileGlobalNamespace->mFunctions.begin(),
										  fileGlobalNamespace->mFunctions.end());

		globalNamespace.mTypedefs.insert(globalNamespace.mTypedefs.end(),
										 fileGlobalNamespace->mTypedefs.begin(),
										 fileGlobalNamespace->mTypedefs.end());

		globalNamespace.mUsingNamespaces.insert(globalNamespace.mUsingNamespaces.end(),
												fileGlobalNamespace->mUsingNamespaces.begin(),
												fileGlobalNamespace->mUsingNamespaces.end());

		globalNamespace.mEnums.insert(globalNamespace.mEnums.end(),
									  fileGlobalNamespace->mEnums.begin(),
									  fileGlobalNamespace->mEnums.end());

		for (auto childSection : fileGlobalNamespace->mSections)
			AppendSection(&globalNamespace, childSection);
	}

	ResolveDependencies(&globalNamespace);
	ResolveBaseClassDependencies(&globalNamespace);

	SyntaxClass* attributeClass = dynamic_cast<SyntaxClass*>(FindSection("o2::IAttribute"));
	SearchAttributes(&globalNamespace, attributeClass);
}

bool CodeToolCache::IsClassBasedOn(SyntaxClass* _class, SyntaxClass* baseClass)
{
	if (!_class || !baseClass)
		return false;

	if (_class->mSourceClass)
		_class = _class->mSourceClass;

	if (_class == baseClass)
		return true;

	for (auto baseClassDef : _class->GetBaseClasses())
		if (IsClassBasedOn(baseClassDef.GetClass(), baseClass))
			return true;

	return false;
}

SyntaxSection* CodeToolCache::FindSection(const string& fullName)
{
	return FindSection(fullName, &globalNamespace);
}

SyntaxSection* CodeToolCache::FindSection(const string& what, const string& where)
{
	return FindSection(what, FindSection(where));
}

SyntaxSection* CodeToolCache::FindSection(const string& what, SyntaxSection* where)
{
	SyntaxSectionsVec passed;
	return FindSection(what, where, passed);
}

SyntaxSection* CodeToolCache::FindSection(const string& what, SyntaxSection* where, SyntaxSectionsVec& processedSections)
{
	if (!where)
		return nullptr;

	if (find(processedSections.begin(), processedSections.end(), where) != processedSections.end())
		return nullptr;

	processedSections.push_back(where);

	int braces = 0, trBraces = 0, sqBraces = 0;
	int delPos = -1;

	int whatLength = (int)what.length();
	for (int i = 0; i < whatLength - 1; i++)
	{
		switch (what[i])
		{
		case '(': braces++; break;
		case ')': braces--; break;
		case '<': trBraces++; break;
		case '>': trBraces--; break;
		case '[': sqBraces++; break;
		case ']': sqBraces--; break;
		}

		if (what[i] == ':' && what[i + 1] == ':' && braces == 0 && trBraces == 0 && sqBraces == 0)
		{
			delPos = i;
			break;
		}
	}

	string searchName;
	if (delPos < 0)
		searchName = what;
	else
		searchName = what.substr(0, delPos);

	int templatesPos = (int)searchName.find('<');
	if (templatesPos != searchName.npos)
		searchName.erase(templatesPos);

	for (auto child : where->mSections)
	{
		if (child->mName != searchName)
			continue;

		if (delPos < 0)
		{
			if (templatesPos >= 0)
			{
				SyntaxClass* newSpectializedClass = new SyntaxClass();
				newSpectializedClass->mName = what;
				if (child->mParentSection)
				{
					newSpectializedClass->mFullName = child->mParentSection->mFullName + "::" + what;
					child->mParentSection->mSections.push_back(newSpectializedClass);
				}
				else newSpectializedClass->mFullName = what;

				newSpectializedClass->mParentSection = child->mParentSection;
				newSpectializedClass->mSourceClass = (SyntaxClass*)child;

				return newSpectializedClass;
			}

			return child;
		}

		if (auto res = FindSection(what.substr(delPos + 2), child, processedSections))
			return res;
	}

	if (auto res = FindSection(what, where->mParentSection, processedSections))
		return res;

	if (where->IsClass())
	{
		SyntaxClass* whereClass = dynamic_cast<SyntaxClass*>(where);
		for (auto& baseClass : whereClass->mBaseClasses)
		{
			if (baseClass.GetClass())
			{
				if (auto res = FindSection(what, baseClass.GetClass(), processedSections))
					return res;
			}
		}
	}

	for (auto tdef : where->mTypedefs)
	{
		if (tdef->GetNewDefName() != searchName)
			continue;

		if (delPos < 0)
			return tdef->GetWhat();

		if (auto res = FindSection(what.substr(delPos + 2), tdef->GetWhat(), processedSections))
			return res;
	}

	for (auto nspace : where->mUsingNamespaces)
	{
		if (auto res = FindSection(what, nspace->GetUsingNamespace(), processedSections))
			return res;
	}

	return nullptr;
}

void CodeToolCache::SearchAttributes(SyntaxSection* section, SyntaxClass* attributeClass)
{
	for (auto childSection : section->mSections)
	{
		if (childSection->IsClass())
		{
			SyntaxClass* childClass = dynamic_cast<SyntaxClass*>(childSection);
			if (IsClassBasedOn(childClass, attributeClass))
				attributes.push_back(childClass);
		}

		SearchAttributes(childSection, attributeClass);
	}
}

void CodeToolCache::Save(const string& file) const
{
	pugi::xml_document doc;

	pugi::xml_node filesNode = doc.append_child("files");
	for (auto file : originalFiles)
		file->SaveTo(filesNode.append_child("file"));

	pugi::xml_node parentProjsNode = doc.append_child("parentProjects");
	for (auto& proj : parentProjects)
		parentProjsNode.append_child("project").append_attribute("path") = proj.c_str();

	doc.save_file(file.c_str());
}

void CodeToolCache::Load(const string& file, bool original /*= true*/)
{
	pugi::xml_document doc;
	doc.load_file(file.c_str());

	pugi::xml_node filesNode = doc.child("files");
	for (auto x : filesNode)
	{
		SyntaxFile* newFile = new SyntaxFile();
		newFile->LoadFrom(x);
		files.push_back(newFile);

		if (original)
			originalFiles.push_back(newFile);
	}

	if (original)
	{
		for (auto x : parentProjects)
			Load(x, false);
	}
	else
	{
		pugi::xml_node parentProjsNode = doc.child("parentProjects");
		for (auto x : parentProjsNode)
		{
			string path = x.attribute("path").as_string();
			parentProjects.push_back(path);
			Load(path, false);
		}
	}
}

void CodeToolCache::AppendSection(SyntaxSection* currentSection, SyntaxSection* newSection)
{
	if (newSection->IsClass())
	{
		SyntaxClass* childClass = (SyntaxClass*)newSection;

		currentSection->mSections.push_back(childClass);
		childClass->mParentSection = currentSection;

		auto oldSections = newSection->mSections;
		newSection->mSections.clear();
		for (auto childSection : oldSections)
			AppendSection(childClass, childSection);
	}
	else
	{
		SyntaxSection* childNamespace = nullptr;

		auto fnd = find_if(currentSection->mSections.begin(), currentSection->mSections.end(),
						   [=](SyntaxSection* x) {
			return !x->IsClass() && x->GetName() == newSection->GetName();
		});

		if (fnd != currentSection->mSections.end())
			childNamespace = *fnd;

		if (!childNamespace)
		{
			childNamespace = new SyntaxNamespace();
			childNamespace->mName = newSection->mName;
			childNamespace->mFullName = newSection->mFullName;

			currentSection->mSections.push_back(childNamespace);
			childNamespace->mParentSection = currentSection;
		}


		childNamespace->mVariables.insert(childNamespace->mVariables.end(),
										  newSection->mVariables.begin(),
										  newSection->mVariables.end());

		childNamespace->mFunctions.insert(childNamespace->mFunctions.end(),
										  newSection->mFunctions.begin(),
										  newSection->mFunctions.end());

		childNamespace->mTypedefs.insert(childNamespace->mTypedefs.end(),
										 newSection->mTypedefs.begin(),
										 newSection->mTypedefs.end());

		childNamespace->mUsingNamespaces.insert(childNamespace->mUsingNamespaces.end(),
												newSection->mUsingNamespaces.begin(),
												newSection->mUsingNamespaces.end());

		childNamespace->mEnums.insert(childNamespace->mEnums.end(),
									  newSection->mEnums.begin(),
									  newSection->mEnums.end());

		for (auto childSection : newSection->mSections)
			AppendSection(childNamespace, childSection);
	}
}

void CodeToolCache::ResolveDependencies(SyntaxSection* section)
{
	for (auto tdef : section->mTypedefs)
	{
		tdef->mWhatSection = FindSection(tdef->mWhatName, section);

		if (!tdef->mWhatSection)
			CodeToolApplication::VerboseLog("Not found section for typedef: %s\n", tdef->mWhatName.c_str());
	}

	for (auto nspace : section->mUsingNamespaces)
	{
		nspace->mUsingNamespace = FindSection(nspace->mUsingNamespaceName);

		if (!nspace->mUsingNamespace)
			CodeToolApplication::VerboseLog("Not found section for using namespace: %s\n", nspace->mUsingNamespaceName.c_str());
	}

	auto sections = section->mSections;
	for (auto childSection : sections)
		ResolveDependencies(childSection);
}

void CodeToolCache::ResolveBaseClassDependencies(SyntaxSection* section)
{
	if (section->IsClass())
	{
		SyntaxClass* cls = (SyntaxClass*)section;
		for (auto& baseClass : cls->mBaseClasses)
		{
			if (baseClass.GetClassName() == "TAsset<AtlasAsset>")
				int x = 5;

			baseClass.mClass = (SyntaxClass*)FindSection(baseClass.mClassName, section);

			if (!baseClass.mClass)
				CodeToolApplication::VerboseLog("Not found base class: %s\n", baseClass.mClassName.c_str());
		}
	}

	auto sections = section->mSections;
	for (auto childSection : sections)
		ResolveBaseClassDependencies(childSection);
}
