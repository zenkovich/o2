#include "o2/stdafx.h"
#include "JavaScriptAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	JavaScriptAsset::JavaScriptAsset()
	{}

	JavaScriptAsset::JavaScriptAsset(const JavaScriptAsset& other):
		AssetWithDefaultMeta<JavaScriptAsset>(other)
	{}

	ScriptParseResult JavaScriptAsset::Parse() const
	{
		return o2Scripts.Parse(o2FileSystem.ReadFile(GetFullPath()), GetAssetsRootPath() + mInfo.path);
	}

	ScriptValue JavaScriptAsset::Run() const
	{
		auto parseRes = Parse();
		if (parseRes.IsOk())
			return o2Scripts.Run(parseRes);

		return ScriptValue();
	}

	JavaScriptAsset& JavaScriptAsset::operator=(const JavaScriptAsset& asset)
	{
		Asset::operator=(asset);
		return *this;
	}

	const char* JavaScriptAsset::GetFileExtensions()
	{
		return "js";
	}

	String JavaScriptAsset::GetEditorIcon()
	{
		return "ui/JSFileIcon.png";
	}

	int JavaScriptAsset::GetEditorSorting()
	{
		return 94;
	}

	void JavaScriptAsset::LoadData(const String& path)
	{
	}

	void JavaScriptAsset::SaveData(const String& path) const
	{
	}
}

DECLARE_TEMPLATE_CLASS(o2::AssetWithDefaultMeta<o2::JavaScriptAsset>);
DECLARE_TEMPLATE_CLASS(o2::DefaultAssetMeta<o2::JavaScriptAsset>);
DECLARE_TEMPLATE_CLASS(o2::Ref<o2::JavaScriptAsset>);

DECLARE_CLASS(o2::JavaScriptAsset);
