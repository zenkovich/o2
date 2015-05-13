#pragma once

#include "Utils/CustomRTTI.h"
#include "Utils/FileSystem/FileInfo.h"

namespace o2
{
	class Asset
	{
		friend class Assets;

		DEFINE_TYPE(Asset);

	protected:
		FileLocation mLocation;

	public:
		Asset();
		Asset(const String& location);
		Asset(const FileLocation& location);
		Asset(const Asset& asset);
		virtual ~Asset();

		Asset& operator=(const Asset& asset);

		FileLocation GetLocation() const;
		String GetPath() const;
		UInt GetFileId() const;

		void Save(const String& path, bool rebuildAssetsImmediately = true);

	protected:
		virtual void SaveData() = 0;
	};

	struct AssetInfo: public Serializable
	{
	public:
		enum class Type { File, Image, Atlas, Folder };

		FileLocation mLocation;
		Type         mType;
		WideTime     mWriteTime;

	public:
		AssetInfo();
		AssetInfo(const FileLocation& location, Type type, const WideTime& writeTime);

		bool operator==(const AssetInfo& other) const;

		SERIALIZE_METHODS(AssetInfo);
	};
	typedef Array<AssetInfo> AssetsInfosArr;
}