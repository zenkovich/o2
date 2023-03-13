#include "o2/stdafx.h"
#include "FileSystem.h"

#include "o2/Application/Application.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	FORWARD_REF_IMPL(LogStream);

	FileSystem::FileSystem()
	{
		mLog = mmake<LogStream>("File System");
		o2Debug.GetLog()->BindStream(mLog);
	}

	FileSystem::~FileSystem()
	{}

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
		file.WriteData(data.Data(), data.Length());
	}
}
