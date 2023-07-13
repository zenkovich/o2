#include "o2/stdafx.h"
#include "ImageCompressor.h"

#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	void ImageCompressor::CompressImage(const String& path, const String& outPath, TextureFormat format, int quality)
	{
		o2Debug.Log("Compress image from " + path + " to " + outPath + " format " + o2Reflection.GetEnumName(format));

		String command = mConfig.formatCommands[::GetEnginePlatform()][format];
		if (command.IsEmpty())
			return;

        if (::GetEnginePlatform() == Platform::Windows)
		    command = "\"" + command + "\"";

		command.ReplaceAll("{quality}", String(quality));
		command.ReplaceAll("{input}", path);
		command.ReplaceAll("{output}", outPath);

		o2Debug.Log("Run compress command:" + command);
		int res = system(command.c_str());

        if (res != 0)
            o2Debug.Log("Something wrong, non-zero result");

		o2FileSystem.FileDelete(path);
	}

	void ImageCompressor::LoadConfig(const String& path)
	{
		DataDocument doc;
		o2Debug.Log("Load compressions config: " + path);
		doc.LoadFromFile(path);
		mConfig = doc;
	}

	void ImageCompressor::GenerateDefaultConfig()
	{
		mConfig.formatCommands =
		{
			{ 
				Platform::Windows, 
				{
					{ TextureFormat::DXT5, "\"../../deps/o2/AssetsBuildTool/Bin/nvcompress.exe\" -nomips -bc3 -alpha \"{input}\" \"{output}.dds\"" }
				} 
			}
		};

		DataDocument doc;
		doc = mConfig;
		doc.SaveToFile(::GetProjectRootPath() + String("deps/o2/CompressToolsConfig.json"));
	}

	ImageCompressor::Config ImageCompressor::mConfig;
}
// --- META ---

DECLARE_CLASS(o2::ImageCompressor::Config);
// --- END META ---
