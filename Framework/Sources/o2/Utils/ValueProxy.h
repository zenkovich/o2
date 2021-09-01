#pragma once

#include "o2/Utils/Function.h"

namespace o2
{
	class Type;
	
	class IAbstractValueProxy
	{
	public:
		virtual ~IAbstractValueProxy() {}

		virtual void SetValuePtr(void* value) = 0;
		virtual void GetValuePtr(void* value) const = 0;
		virtual const Type& GetType() const = 0;
	};

	template<typename _type>
	class IValueProxy: public IAbstractValueProxy
	{
	public:
		virtual void SetValue(const _type& value) = 0;
		virtual _type GetValue() const = 0;

		void SetValuePtr(void* value) override;
		void GetValuePtr(void* value) const override;
		const Type& GetType() const override;
	};

	class IPointerValueProxy
	{
	public:
		virtual void* GetValueVoidPointer() const = 0;
	};

	template<typename _type>
	class PointerValueProxy: public IValueProxy<_type>, public IPointerValueProxy
	{
	protected:
		_type* mValuePtr = nullptr;

	public:
		PointerValueProxy() {}
		PointerValueProxy(_type* valuePtr):mValuePtr(valuePtr) {}

		void SetValue(const _type& value) override { *mValuePtr = value; }
		_type GetValue() const override { return *mValuePtr; }

		_type* GetValuePointer() const { return mValuePtr; }
		void* GetValueVoidPointer() const override { return (void*)mValuePtr; }
	};

	template<typename _type>
	class FunctionalValueProxy: public IValueProxy<_type>
	{
	protected:
		Function<void(_type)> mSetter;
		Function<_type()>     mGetter;

	public:
		FunctionalValueProxy() {}
		FunctionalValueProxy(const Function<void(_type)> setter, const Function<_type()> getter):
			mSetter(setter), mGetter(getter) {}

		void SetValue(const _type& value) override { mSetter(value); }
		_type GetValue() const override { return mGetter(); }
	};
	
	template<typename _type, typename _property_type>
	class PropertyValueProxy: public IValueProxy<_type>
	{
		_property_type* mProperty;
	public:
		PropertyValueProxy() {}
		PropertyValueProxy(_property_type* ptr):mProperty(ptr) {}
		
		bool operator==(const PropertyValueProxy<_type, _property_type>& other) const { return mProperty == other.mProperty; }
		
		void SetValue(const _type& value) override { mProperty->Set(value); }
		_type GetValue() const override { return mProperty->Get(); }
	};
}

#include "o2/Utils/Reflection/Type.h"

namespace o2
{
	template<typename _type>
	void IValueProxy<_type>::SetValuePtr(void* value)
	{
		SetValue(*(_type*)value);
	}
	
	template<typename _type>
	void IValueProxy<_type>::GetValuePtr(void* value) const
	{
		*(_type*)value = GetValue();
	}
	
	template<typename _type>
	const Type& IValueProxy<_type>::GetType() const
	{
		return TypeOf(_type);
	}
}
