#pragma once

#include "Utils/String.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/Memory/IObject.h"
#include "Utils/Containers/Dictionary.h"

namespace o2
{
	class IClassFieldInfo
	{
	protected:
		String mName;

	protected:
		IClassFieldInfo(const String& name);

	public:
		const String& Name() const;

		virtual DataNode Serialize() = 0;
		virtual void Deserialize(const DataNode& node) = 0;
	};

	template<typename _type>
	class ClassFieldInfo : public IClassFieldInfo
	{
		friend class ClassFieldRegistrator;
		friend class SerializeHelper;

	protected:
		IObject* mOwner;
		_type*   mValuePtr;

	protected:
		ClassFieldInfo(IObject* owner, _type* valuePtr, const String& name);

	public:
		DataNode Serialize();
		void Deserialize(const DataNode& node);
	};


	template<typename _type>
	ClassFieldInfo<_type>::ClassFieldInfo(IObject* owner, _type* valuePtr, const String& name) :
		IClassFieldInfo(name), mOwner(owner), mValuePtr(valuePtr)
	{
	}

	template<typename _type>
	DataNode ClassFieldInfo<_type>::Serialize()
	{
		DataNode res;
		res = *mValuePtr;
		res.SetName(mName);
		return res;
	}

	template<typename _type>
	void ClassFieldInfo<_type>::Deserialize(const DataNode& node)
	{
		*mValuePtr = node;
	}
}