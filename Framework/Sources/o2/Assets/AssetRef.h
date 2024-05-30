#pragma once

#include "o2/Utils/Types/Ref.h"

namespace o2
{
	class Asset;

	// -----------------------------------------------------------------------
	// Basic asset reference interface. All Ref<AssetType> are derived from it
	// -----------------------------------------------------------------------
	class BaseAssetRef : public ISerializable
	{
	public:
		// Returns asset type
		virtual const Type& GetAssetType() const;

		// Returns asset raw pointer
		virtual const Asset* GetAssetBase() const { return nullptr; }

		// Returns asset raw pointer
		virtual Asset* GetAssetBase() { return nullptr; }

		// Sets asset
		virtual void SetAssetBase(Asset* asset) {}

		// Sets asset instance
		virtual void SetInstance(Asset* asset) {}

		// Creates own asset instance. If asset is empty creates empty instance, copies asset if else
		virtual void CreateInstance() {}

		// Removes own asset instance
		virtual void RemoveInstance() {}

		// Saves asset instance
		virtual void SaveInstance(const String& path) {}

		// Is asset instance owner
		virtual bool IsInstance() const { return false; }

		SERIALIZABLE(BaseAssetRef);
	};

#define ENABLE_ASSET typename std::enable_if<IsBaseOf<Asset, _asset_type>::value>::type

	// -----------------------------------------------------------------------------------
	// Asset reference. Contains asset pointer. Can contain asset instance owned by itself
	// -----------------------------------------------------------------------------------
	template<typename _asset_type>
	class Ref<_asset_type, ENABLE_ASSET> : public BaseRef<_asset_type>, public BaseAssetRef
	{
	public:
		using Base = BaseRef<_asset_type>;

	public:
		// Base reference implementation
		BASE_REF_IMPLEMETATION(_asset_type);

		// Constructor from asset path
		explicit Ref(const String& path);

		// Constructor from asset id
		explicit Ref(const UID& id);

		// Returns asset type
		const Type& GetAssetType() const override;

		// Returns asset type
		static const Type* GetAssetTypeStatic();

		// Returns asset raw pointer
		const Asset* GetAssetBase() const override;

		// Returns asset raw pointer
		Asset* GetAssetBase() override;

		// Sets asset
		void SetAssetBase(Asset* asset) override;

		// Sets asset instance
		void SetInstance(Asset* asset) override;

		// Creates own asset instance. If asset is empty creates empty instance, copies asset if else
		void CreateInstance() override;

		// Removes own asset instance
		void RemoveInstance() override;

		// Saves asset instance
		void SaveInstance(const String& path) override;

		// Is asset instance owner
		bool IsInstance() const override;

		// Creates asset and returns reference
		template<typename ... _args>
		static Ref<_asset_type> CreateAsset(_args ... args);

	protected:
		bool mIsInstance = false; // Is this reference owner of asset

	protected:
		// Beginning serialization callback - writes path and id
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback -  reads path and id and searches asset
		void OnDeserialized(const DataValue& node) override;

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// It is required to process asset reference as single object when searching deltas for prototypes
		static bool IsDeltaAsSingleObject();

	public:
		using _this_type = Ref<_asset_type, typename std::enable_if<std::is_base_of<Asset, _asset_type>::value>::type>;

		SERIALIZABLE_MAIN(_this_type);
		IOBJECT_SCRIPTING();

		template<typename _type_processor>
		static void ProcessBaseTypes(_this_type* object, _type_processor& processor)
		{
			typedef _this_type thisclass;
			processor.template StartBases<_this_type>(object, type);

			BASE_CLASS(BaseAssetRef);
		}

		template<typename _type_processor>
		static void ProcessFields(_this_type* object, _type_processor& processor)
		{
			typedef _this_type thisclass;
			processor.template StartFields<_this_type>(object, type);
		}

		template<typename _type_processor>
		static void ProcessMethods(_this_type* object, _type_processor& processor)
		{
			typedef _this_type thisclass;
			processor.template StartMethods<_this_type>(object, type);

			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(const String&);

			FUNCTION().PUBLIC().CONSTRUCTOR();
			FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<_asset_type>&);
			FUNCTION().PUBLIC().CONSTRUCTOR(_asset_type*);
			FUNCTION().PUBLIC().CONSTRUCTOR(const UID&);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(_asset_type*, Get);
			FUNCTION().PUBLIC().SIGNATURE(const Type&, GetAssetType);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetInstance, Asset*);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, CreateInstance);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveInstance);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SaveInstance, const String&);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsInstance);

			FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<_asset_type>, CreateAsset);
			FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetAssetTypeStatic);
		}
	};

	using AssetRef = Ref<o2::Asset>;
}

#include "o2/Assets/Assets.h"

namespace o2
{
	template<typename _asset_type>
	Ref<_asset_type, ENABLE_ASSET>::Ref(const String& path)
	{
		*this = o2Assets.GetAssetRefByType<_asset_type>(path);
	}

	template<typename _asset_type>
	Ref<_asset_type, ENABLE_ASSET>::Ref(const UID& id)
	{
		*this = o2Assets.GetAssetRefByType<_asset_type>(id);
	}

	template<typename _asset_type>
	const Type& Ref<_asset_type, ENABLE_ASSET>::GetAssetType() const
	{
		return TypeOf(_asset_type);
	}

	template<typename _asset_type>
	const Type* Ref<_asset_type, ENABLE_ASSET>::GetAssetTypeStatic()
	{
		return &TypeOf(_asset_type);
	}

	template<typename _asset_type>
	const Asset* Ref<_asset_type, ENABLE_ASSET>::GetAssetBase() const
	{
		return BaseRef<_asset_type>::Get();
	}

	template<typename _asset_type>
	Asset* Ref<_asset_type, ENABLE_ASSET>::GetAssetBase()
	{
		return BaseRef<_asset_type>::Get();
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::SetAssetBase(Asset* asset)
	{
		*this = Ref(dynamic_cast<_asset_type*>(asset));
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::SetInstance(Asset* asset)
	{
		*this = Ref(dynamic_cast<_asset_type*>(asset));
		mIsInstance = true;
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::CreateInstance()
	{
		_asset_type* asset;
		if (Base::mPtr)
			asset = dynamic_cast<_asset_type*>(Base::mPtr->CloneAs<Asset>());
		else
		{
			auto objectType = dynamic_cast<const ObjectType*>(&GetAssetType());
			asset = dynamic_cast<_asset_type*>(objectType->DynamicCastToIObject(objectType->CreateSample()));
		}

		SetInstance(asset);
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::RemoveInstance()
	{
		if (!mIsInstance)
			return;

		mIsInstance = false;

		*this = nullptr;
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::SaveInstance(const String& path)
	{
		if (!mIsInstance)
			return;

		Base::mPtr->SetPath(path);
		Base::mPtr->Save();

		*this = Ref<_asset_type>(path);
	}

	template<typename _asset_type>
	bool Ref<_asset_type, ENABLE_ASSET>::IsInstance() const
	{
		return mIsInstance;
	}

	template<typename _asset_type>
	template<typename ... _args>
	Ref<_asset_type> Ref<_asset_type, ENABLE_ASSET>::CreateAsset(_args ... args)
	{
		return DynamicCast<_asset_type>(o2Assets.CreateAsset<_asset_type>(args ...));
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::OnSerialize(DataValue& node) const
	{
		if (mIsInstance)
		{
			if (Base::mPtr)
			{
				node["instance"] = Base::mPtr;
				node["meta"] = Base::mPtr->GetMeta();
			}
		}
		else if (Base::mPtr)
		{
			node["id"] = Base::mPtr->GetUID().ToString();
			node["path"] = Base::mPtr->GetPath();
		}
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::OnDeserialized(const DataValue& node)
	{
		Base::DecrementRef();

		Base::mPtr = nullptr;
		mIsInstance = false;

		if (auto instanceNode = node.FindMember("instance"))
		{
			mIsInstance = true;
			Base::mPtr = *instanceNode;

			UID oldUid = Base::mPtr->GetUID();
			node.GetMember("meta").Get(Base::mPtr->mInfo.meta);
			o2Assets.UpdateAssetCache(Base::mPtr, "", oldUid);

			Base::IncrementRef();
		}
		else if (auto idNode = node.FindMember("id"))
			*this = o2Assets.GetAssetRefByType<_asset_type>((UID)(*idNode));
		else if (auto pathNode = node.FindMember("path"))
			*this = o2Assets.GetAssetRefByType<_asset_type>((String)pathNode);
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::OnSerializeDelta(DataValue& node, const IObject& origin) const
	{
		OnSerialize(node);
	}

	template<typename _asset_type>
	void Ref<_asset_type, ENABLE_ASSET>::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		OnDeserialized(node);
	}

	template<typename _asset_type>
	bool Ref<_asset_type, ENABLE_ASSET>::IsDeltaAsSingleObject()
	{
		return true;
	}
}
// --- META ---

CLASS_BASES_META(o2::BaseAssetRef)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::BaseAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::BaseAssetRef)
{

    FUNCTION().PUBLIC().SIGNATURE(const Type&, GetAssetType);
    FUNCTION().PUBLIC().SIGNATURE(const Asset*, GetAssetBase);
    FUNCTION().PUBLIC().SIGNATURE(Asset*, GetAssetBase);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAssetBase, Asset*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInstance, Asset*);
    FUNCTION().PUBLIC().SIGNATURE(void, CreateInstance);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveInstance);
    FUNCTION().PUBLIC().SIGNATURE(void, SaveInstance, const String&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsInstance);
}
END_META;
// --- END META ---
