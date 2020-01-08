#include "o2/stdafx.h"
#include "FileSystem.h"

#include "o2/Application/Application.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	FileSystem::FileSystem()
	{
		mLog = mnew LogStream("File System");
		o2Debug.GetLog()->BindStream(mLog);

		mExtensions.Add(FileType::Image, StringsVec());
		mExtensions[FileType::Image].Add("png");
		mExtensions[FileType::Image].Add("jpg");
		mExtensions[FileType::Image].Add("dds");
		mExtensions[FileType::Image].Add("pvr");

		mExtensions.Add(FileType::Config, StringsVec());
		mExtensions[FileType::Config].Add("xml");
		mExtensions[FileType::Config].Add("txt");
		mExtensions[FileType::Config].Add("cfg");

		mExtensions.Add(FileType::Atlas, StringsVec());
		mExtensions[FileType::Atlas].Add("atlas");
	}

	FileSystem::~FileSystem()
	{}

	const String& FileSystem::GetResourcesPath() const
	{
		return mInstance->mResourcesPath;
	}

	String FileSystem::ExtractPathStr(const String& path) const
	{
		auto fnd = path.FindLast("/");
		if (fnd < 0)
			return "";

		return path.SubStr(0, fnd);
	}

	String FileSystem::GetFileExtension(const String& filePath)
	{
		int dotIdx = filePath.FindLast(".");

		if (dotIdx != -1)
			return filePath.SubStr(dotIdx + 1);

		return String();
	}

	String FileSystem::GetFileNameWithoutExtension(const String& filePath)
	{
		return filePath.SubStr(0, filePath.FindLast("."));
	}

	String FileSystem::GetPathWithoutDirectories(const String& path)
	{
		return path.SubStr(Math::Max(path.FindLast("/"), path.FindLast("\\")) + 1);
	}

	String FileSystem::GetParentPath(const String& path)
	{
		int idx = Math::Max(path.FindLast("/"), path.FindLast("\\"));

		if (idx == -1)
			return String();

		return path.SubStr(0, idx);
	}

	String FileSystem::ReadFile(const String& path)
	{
		InFile file(path);
		if (!file.IsOpened())
			return String();

		return file.ReadFullData();
	}

	void FileSystem::WriteFile(const String& path, const String& data)
	{
		OutFile file(path);
		file.WriteData(path.Data(), path.Length());
	}
}
