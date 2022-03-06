#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	// ---------------
	// Asset reference
	// ---------------
	class AssetRef: public ISerializable
	{
	public:
		// Default constructor, references to null
		AssetRef();

		// Copy-constructor
		AssetRef(const AssetRef& other);

		// Instance reference asset constructor
		AssetRef(Asset* instance);

		// Constructor from asset path
		AssetRef(const String& path);

		// Constructor from asset id
		AssetRef(const UID& id);

		// Destructor
		virtual ~AssetRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		AssetRef& operator=(const AssetRef& other);

		// Getter operator
		Asset& operator*();

		// Constant getter operator
		const Asset& operator*() const;

		// Asset members and field operator
		Asset* operator->();

		// Constant asset members and field operator
		const Asset* operator->() const;

		// Check equals operator
		bool operator==(const AssetRef& other) const;

		// Check not equals operator
		bool operator!=(const AssetRef& other) const;

		// Less operator, for map
		bool operator<(const AssetRef& other) const;

		// Returns is reference is valid
		bool IsValid() const;

		// Returns asset
		Asset* Get();

		// Returns asset
		const Asset* Get() const;

		// Returns asset type
		virtual const Type& GetAssetType() const;

		template<typename T, typename E = std::enable_if<std::is_base_of<Asset, T>::value>>
		Ref<T> Cast() const;

		// Sets asset instance
		void SetInstance(Asset* asset);

		// Creates own asset instance. If asset is empty creates empty instance, 
		// copies asset if else
		void CreateInstance();

		// Removes own asset instance
		void RemoveInstance();

		// Saves asset instance
		void SaveInstance(const String& path);

		// Is asset instance owner
		bool IsInstance() const;

		SERIALIZABLE(AssetRef);

	protected:
		bool   mIsInstance = false;   // Is this reference owner of asset
		int*   mRefCounter = nullptr; // Reference count pointer
		Asset* mAssetPtr = nullptr;   // Asset pointer   

	protected:
		// Constructor for Assets manager
		AssetRef(Asset* assetPtr, int* refCounter);

		// Beginning serialization callback - writes path and id
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback -  reads path and id and searches asset
		void OnDeserialized(const DataValue& node) override;

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Updates specialized asset pointer
		virtual void UpdateSpecAsset() {};

		// It is required to process asset reference as single object when searching deltas for prototypes
		static bool IsDeltaAsSingleObject();

		friend class Assets;
	};
}

#include "o2/Assets/Assets.h"

namespace o2
{
	template<typename T>
	class Ref<T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>: public AssetRef
	{
	public:
		// Default constructor, references to null
		Ref(): AssetRef() {}

		// Instance reference asset constructor
		Ref(T* instance): AssetRef(instance) { mSpecAssetPtr = instance; }

		// Copy-constructor
		Ref(const AssetRef& other): AssetRef(other) { UpdateSpecAsset(); }

		// Constructor from asset path
		Ref(const String& path): AssetRef(path) { UpdateSpecAsset(); }

		// Constructor from asset id
		Ref(const UID& id): AssetRef(id) { UpdateSpecAsset(); }

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		Ref<T>& operator=(const Ref<T>& other)
		{
			AssetRef::operator=(other);
			mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr);
			return *this;
		}

		// Getter operator
		T& operator*() { return *mSpecAssetPtr; }

		// Constant getter operator
		const T& operator*() const { return *mSpecAssetPtr; }

		// Asset members and field operator
		T* operator->() { return mSpecAssetPtr; }

		// Constant asset members and field operator
		const T* operator->() const { return mSpecAssetPtr; }

		// Check equals operator
		bool operator==(const Ref<T>& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const Ref<T>& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(T); }

		// Returns asset type
		static const Type* GetAssetTypeStatic() { return &TypeOf(T); }

		// Creates asset and returns reference
		template<typename ... _args>
		static Ref<T> CreateAsset(_args ... args) { return o2Assets.CreateAsset<T>(args ...); }

	public:
		typedef Ref<T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type> _thisType;

		SERIALIZABLE_MAIN(_thisType);

		template<typename _type_processor>
		static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartBases<_thisType>(object, type);

			BASE_CLASS(o2::AssetRef);
		}

		template<typename _type_processor>
		static void ProcessFields(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartFields<_thisType>(object, type);

			FIELD().NAME(mSpecAssetPtr).PUBLIC();
		}

		template<typename _type_processor>
		static void ProcessMethods(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartMethods<_thisType>(object, type);

			FUNCTION().PUBLIC().SIGNATURE(const Type&, GetAssetType);
			FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<T>, CreateAsset);
			FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetAssetTypeStatic);
		}

	protected:
		T* mSpecAssetPtr = nullptr;

	protected:
		// Updates specialized asset pointer
		void UpdateSpecAsset() override 
		{
			mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr);

			if (!mSpecAssetPtr && mAssetPtr)
				*this = Ref<T>();
		};
	};

	template<typename T, typename E>
	Ref<T> AssetRef::Cast() const
	{
		return Ref<T>(*this);
	}

}

CLASS_BASES_META(o2::AssetRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetRef)
{
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsInstance);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRefCounter);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAssetPtr);
}
END_META;
CLASS_METHODS_META(o2::AssetRef)
{

	FUNCTION().PUBLIC().SIGNATURE(bool, IsValid);
	FUNCTION().PUBLIC().SIGNATURE(Asset*, Get);
	FUNCTION().PUBLIC().SIGNATURE(const Asset*, Get);
	FUNCTION().PUBLIC().SIGNATURE(const Type&, GetAssetType);
	FUNCTION().PUBLIC().SIGNATURE(void, SetInstance, Asset*);
	FUNCTION().PUBLIC().SIGNATURE(void, CreateInstance);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveInstance);
	FUNCTION().PUBLIC().SIGNATURE(void, SaveInstance, const String&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsInstance);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateSpecAsset);
	FUNCTION().PROTECTED().SIGNATURE_STATIC(bool, IsDeltaAsSingleObject);
}
END_META;
