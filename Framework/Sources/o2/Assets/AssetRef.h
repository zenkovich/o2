#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/Assets.h"
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

		// Returns is reference is valid
		bool IsValid() const;

		// Returns asset
		Asset* Get();

		// Returns asset
		const Asset* Get() const;

		// Returns asset type
		virtual const Type& GetAssetType() const;

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
		bool   mAssetOwner = false;   // Is this reference owner of asset
		int*   mRefCounter = nullptr; // Reference count pointer
		Asset* mAssetPtr = nullptr;   // Asset pointer   

	protected:
		// Constructor for Assets manager
		AssetRef(Asset* assetPtr, int* refCounter);

		// Beginning serialization callback - writes path and id
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback -  reads path and id and searches asset
		void OnDeserialized(const DataValue& node) override;

		// Updates specialized asset pointer
		virtual void UpdateSpecAsset() {};

		friend class Assets;
	};

	template<typename T>
	class Ref<T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>: public AssetRef
	{
	public:
		// Default constructor, references to null
		Ref(): AssetRef() {}

		// Instance reference asset constructor
		Ref(T* instance): AssetRef(instance) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

		// Copy-constructor
		Ref(const AssetRef& other): AssetRef(other) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

		// Constructor from asset path
		Ref(const String& path): AssetRef(path) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

		// Constructor from asset id
		Ref(const UID& id): AssetRef(id) { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); }

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

			PROTECTED_FIELD(mSpecAssetPtr);
		}

		template<typename _type_processor>
		static void ProcessMethods(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartMethods<_thisType>(object, type);

			PUBLIC_FUNCTION(const Type&, GetAssetType);
			PUBLIC_STATIC_FUNCTION(Ref<T>, CreateAsset);
			PUBLIC_STATIC_FUNCTION(const Type*, GetAssetTypeStatic);
		}

	protected:
		T* mSpecAssetPtr = nullptr;

	protected:
		// Updates specialized asset pointer
		void UpdateSpecAsset() override { mSpecAssetPtr = dynamic_cast<T*>(mAssetPtr); };
	};
}

CLASS_BASES_META(o2::AssetRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetRef)
{
	PROTECTED_FIELD(mAssetOwner).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mRefCounter).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mAssetPtr).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(o2::AssetRef)
{

	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(Asset*, Get);
	PUBLIC_FUNCTION(const Asset*, Get);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
	PUBLIC_FUNCTION(void, SetInstance, Asset*);
	PUBLIC_FUNCTION(void, CreateInstance);
	PUBLIC_FUNCTION(void, RemoveInstance);
	PUBLIC_FUNCTION(void, SaveInstance, const String&);
	PUBLIC_FUNCTION(bool, IsInstance);
	PROTECTED_FUNCTION(void, OnSerialize, DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, UpdateSpecAsset);
}
END_META;
