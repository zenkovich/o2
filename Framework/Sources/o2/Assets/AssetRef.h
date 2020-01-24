#pragma once

#include "o2/Assets/Asset.h"

namespace o2
{
	// ---------------
	// Asset reference
	// ---------------
	class IAssetRef: public ISerializable
	{
	public:
		// Default constructor, references to null
		IAssetRef();

		// Copy-constructor
		IAssetRef(const IAssetRef& other);

		// Constructor from asset path
		IAssetRef(const String& path);

		// Constructor from asset id
		IAssetRef(const UID& id);

		// Destructor
		virtual ~IAssetRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		IAssetRef& operator=(const IAssetRef& other);

		// Getter operator
		Asset& operator*();

		// Constant getter operator
		const Asset& operator*() const;

		// Asset members and field operator
		Asset* operator->();

		// Constant asset members and field operator
		const Asset* operator->() const;

		// Check equals operator
		bool operator==(const IAssetRef& other) const;

		// Check not equals operator
		bool operator!=(const IAssetRef& other) const;

		// Returns is reference is valid
		bool IsValid() const;

		// Returns asset type
		virtual const Type& GetAssetType() const;

		SERIALIZABLE(IAssetRef);

	protected:
		bool   mAssetOwner = false;   // Is this reference owner of asset
		int*   mRefCounter = nullptr; // Reference count pointer
		Asset* mAssetPtr = nullptr;   // Asset pointer   

	protected:
		// Constructor for Assets manager
		IAssetRef(Asset* assetPtr, int* refCounter);

		// Beginning serialization callback - writes path and id
		void OnSerialize(DataNode& node) const override;

		// Completion deserialization callback -  reads path and id and searchs asset
		void OnDeserialized(const DataNode& node) override;

		friend class Assets;
	};

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	class Ref: public IAssetRef
	{
	public:
		// Default constructor, references to null
		Ref();

		// Copy-constructor
		Ref(const IAssetRef& other);

		// Constructor from asset path
		Ref(const String& path);

		// Constructor from asset id
		Ref(const UID& id);

		// Destructor
		virtual ~Ref();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		Ref<T>& operator=(const Ref<T>& other);

		// Getter operator
		T& operator*();

		// Constant getter operator
		const T& operator*() const;

		// Asset members and field operator
		T* operator->();

		// Constant asset members and field operator
		const T* operator->() const;

		// Check equals operator
		bool operator==(const Ref<T>& other) const;

		// Check not equals operator
		bool operator!=(const Ref<T>& other) const;

		// Returns asset type
		virtual const Type& GetAssetType() const;

		SERIALIZABLE(Ref<T>);

	};

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	Ref<T, std>::Ref()
	{

	}

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	Ref<T, std>::Ref(const IAssetRef& other)
	{

	}

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	Ref<T, std>::Ref(const String& path)
	{

	}

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	Ref<T, std>::Ref(const UID& id)
	{

	}

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	Ref<T, std>::~Ref()
	{

	}
	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	bool Ref<T, std>::operator==(const Ref<T>& other) const
	{

	}

	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	Ref<T>& Ref<T, std>::operator=(const Ref<T>& other)
	{

	}

	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	bool Ref<T, std>::operator!=(const Ref<T>& other) const
	{

	}

	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	const T* Ref<T, std>::operator->() const
	{

	}

	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	T* Ref<T, std>::operator->()
	{

	}

	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	const T& Ref<T, std>::operator*() const
	{

	}

	template<typename T, typename std<std::is_base_of<Asset, T>:value>:type>
	T& Ref<T, std>::operator*()
	{

	}

	template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type>
	Ref<T, std>::operator bool() const
	{

	}
}
