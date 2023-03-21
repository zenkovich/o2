#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/Assets.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	// -----------------------------------------------------------------------------------
	// Asset reference. Contains asset pointer. Can contain asset instance owned by itself
	// -----------------------------------------------------------------------------------
	template<typename _asset_type>
	class Ref<_asset_type, typename std::enable_if<std::is_base_of<Asset, _asset_type>::value>::type>: public BaseRef<_asset_type>
	{
	public:
		using Base = BaseRef<_asset_type>;

	public:
		// Base reference implementation
		BASE_REF_IMPLEMETATION(_asset_type);

		// Constructor from asset path
		Ref(const String& path) :
			Ref(o2Assets.GetAssetRef(path))
		{}

		// Constructor from asset id
		Ref(const UID& id) :
			Ref(o2Assets.GetAssetRef(id))
		{}

		// Returns asset type
		const Type& GetAssetType() const { return TypeOf(_asset_type); }

		// Returns asset type
		static const Type* GetAssetTypeStatic() { return &TypeOf(_asset_type); }

		// Sets asset instance
		void SetInstance(Asset* asset)
		{
			*this = Ref(asset);
			mIsInstance = true;
		}

		// Creates own asset instance. If asset is empty creates empty instance, copies asset if else
		void CreateInstance()
		{
			_asset_type* asset;
			if (Base::mPtr)
				asset = Base::mPtr->CloneAs<Asset>();
			else
			{
				auto objectType = dynamic_cast<const ObjectType*>(&GetAssetType());
				asset = dynamic_cast<Asset*>(objectType->DynamicCastToIObject(objectType->CreateSample()));
			}

			SetInstance(asset);
		}

		// Removes own asset instance
		void RemoveInstance()
		{
			if (!mIsInstance)
				return;

			mIsInstance = false;

			*this = nullptr;
		}

		// Saves asset instance
		void SaveInstance(const String& path)
		{
			if (!mIsInstance)
				return;

			Base::mPtr->SetPath(path);
			Base::mPtr->Save();

			*this = Ref<_asset_type>(path);
		}

		// Is asset instance owner
		bool IsInstance() const { return mIsInstance; }

		// Creates asset and returns reference
		template<typename ... _args>
		static Ref<_asset_type> CreateAsset(_args ... args) { return o2Assets.CreateAsset<_asset_type>(args ...); }

	protected:
		bool mIsInstance = false; // Is this reference owner of asset

	protected:
		// Beginning serialization callback - writes path and id
		void OnSerialize(DataValue& node) const override
		{
			if (mIsInstance)
			{
				if (Base::mPtr)
				{
					AssetRef* nonConstThis = const_cast<AssetRef*>(this);
					*nonConstThis = AssetRef(Base::mPtr->CloneAs<Asset>());

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

		// Completion deserialization callback -  reads path and id and searches asset
		void OnDeserialized(const DataValue& node) override
		{
			Base::DecrementRef();

			Base::mPtr = nullptr;
			mIsInstance = false;

			if (auto instanceNode = node.FindMember("instance"))
			{
				mIsInstance = true;
				Base::mPtr = (Asset*)(*instanceNode);

				UID oldUid = Base::mPtr->GetUID();
				Base::mPtr->mInfo.meta = node.GetMember("meta");
				o2Assets.UpdateAssetCache(Base::mPtr, "", oldUid);

				Base::IncrementRef();
			}
			else if (auto idNode = node.FindMember("id"))
				*this = o2Assets.GetAssetRef((UID)(*idNode));
			else if (auto pathNode = node.FindMember("path"))
				*this = o2Assets.GetAssetRef((String)pathNode);
		}

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override { OnSerialize(node); }

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override { OnDeserialized(node); }

		// It is required to process asset reference as single object when searching deltas for prototypes
		static bool IsDeltaAsSingleObject() { return true; }

	public:
		using _this_type = Ref<_asset_type, typename std::enable_if<std::is_base_of<Asset, _asset_type>::value>::type>;

		SERIALIZABLE_MAIN(_this_type);

		template<typename _type_processor>
		static void ProcessBaseTypes(_this_type* object, _type_processor& processor)
		{
			typedef _this_type thisclass;
			processor.template StartBases<_this_type>(object, type);
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
			FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<Asset>&);
			FUNCTION().PUBLIC().CONSTRUCTOR(Asset*);
			FUNCTION().PUBLIC().CONSTRUCTOR(const UID&);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Asset*, Get);
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

	using AssetRef = Ref<Asset>;
}
