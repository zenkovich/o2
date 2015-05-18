#pragma once

#include "Utils/String.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/Containers/Dictionary.h"

namespace o2
{
	class IClassFieldInfo
	{
	protected:
		String mName;
		void*  mOwner;

	protected:
		IClassFieldInfo(void* owner, const String& name);

	public:
		const String& Name() const;

		virtual DataNode Serialize() = 0;
		virtual void     Deserialize(const DataNode& node) = 0;
	};

	template<typename _type>
	class ClassFieldInfo : public IClassFieldInfo
	{
		friend class ClassFieldRegistrator;
		friend class SerializeHelper;

	protected:
		_type& mValue;

	public:
		ClassFieldInfo(void* owner, _type& value, const String& name);

		DataNode Serialize();
		void     Deserialize(const DataNode& node);
	};


	template<typename _type>
	ClassFieldInfo<_type>::ClassFieldInfo(void* owner, _type& value, const String& name) :
		IClassFieldInfo(owner, name), mValue(value)
	{
	}

	template<typename _type>
	DataNode ClassFieldInfo<_type>::Serialize()
	{
		DataNode res;
		res = mValue;
		res.SetName(mName);
		return res;
	}

	template<typename _type>
	void ClassFieldInfo<_type>::Deserialize(const DataNode& node)
	{
		mValue = node;
	}
}