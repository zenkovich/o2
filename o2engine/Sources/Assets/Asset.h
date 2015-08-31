#pragma once

#include "Assets/Assets.h"
#include "Utils/CustomRTTI.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Property.h"
#include "Utils/Serialization.h"

namespace o2
{
	class Asset
	{
	public:
		class IMetaInfo: public ISerializable
		{
		public:
			virtual TypeId GetAssetType() const;
			virtual bool IsEqual(Ptr<IMetaInfo> other) const;

			SERIALIZABLE_IMPL(IMetaInfo);
			IOBJECT(IMetaInfo) {}
		};

	protected:
		String         mPath;
		UInt           mId;
		Ptr<IMetaInfo> mMeta;

	public:
		DEFINE_TYPE(Asset);

		Property<String>       Path;
		Getter<String>         FullPath;
		Getter<UInt>           Id;
		Getter<TypeId>         AssetType;
		Getter<Ptr<IMetaInfo>> Meta;

		Asset();
		Asset(const String& path);
		Asset(UInt id);
		Asset(const Asset& asset);
		virtual ~Asset();

		Asset& operator=(const Asset& asset);

		String GetPath() const;
		void SetPath(const String& path);
		String GetFullPath() const;

		UInt GetId() const;

		Ptr<IMetaInfo> GetMeta() const;

		void Load();
		void Load(const String& path);
		void Load(UInt id);
		void Save(const String& path, bool rebuildAssetsImmediately = true);
		void Save(bool rebuildAssetsImmediately = true);

		virtual Vector<String> GetExtensions() const = 0;

	protected:
		virtual void LoadData(const String& path) = 0;
		virtual void SaveData(const String& path) = 0;

		virtual void LoadMeta(const String& path);
		virtual void SaveMeta(const String& path);

		void InitializeProperties();
		void GenerateId();
		String GetMetaFullPath() const;

		Ptr<LogStream> GetAssetsLogStream() const;
	};
}